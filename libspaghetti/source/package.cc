// MIT License
//
// Copyright (c) 2017 Artur Wyszyński, aljen at hitomi dot pl
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <fstream>
#include <iostream>
#include <string_view>

#include "spaghetti/package.h"

#include "elements/logic/clock.h"
#include "spaghetti/logger.h"
#include "spaghetti/registry.h"

namespace spaghetti {

Package::Package()
  : Element{}
{
  m_data.push_back(this);

  addInput(ValueType::eBool, "#1");
  addInput(ValueType::eBool, "#2");
  addInput(ValueType::eBool, "#3");
  addOutput(ValueType::eBool, "#1");
}

Package::~Package()
{
  size_t const SIZE{ m_data.size() };
  for (size_t i = 1; i < SIZE; ++i) delete m_data[i];
}

void Package::serialize(Element::Json &a_json)
{
  Element::serialize(a_json);

  auto &jsonNode = a_json["node"];
  jsonNode["inputs_position"]["x"] = m_inputsPosition.x;
  jsonNode["inputs_position"]["y"] = m_inputsPosition.y;
  jsonNode["outputs_position"]["x"] = m_outputsPosition.x;
  jsonNode["outputs_position"]["y"] = m_outputsPosition.y;

  auto &jsonPackage = a_json["package"];
  jsonPackage["description"] = m_packageDescription;
  jsonPackage["path"] = m_packagePath;
  jsonPackage["icon"] = m_packageIcon;

  auto jsonElements = Json::array();
  // TODO(aljen): fix holes
  size_t const DATA_SIZE{ m_data.size() };
  for (size_t i = 1; i < DATA_SIZE; ++i) {
    auto const element = m_data[i];
    if (element == nullptr) continue;

    Json jsonElement{};
    element->serialize(jsonElement);
    jsonElements.push_back(jsonElement);
  }
  jsonPackage["elements"] = jsonElements;

  auto jsonConnections = Json::array();
  for (auto const &connection : m_connections) {
    Json jsonConnection{}, jsonConnect{}, jsonTo{};

    jsonConnect["id"] = connection.from_id;
    jsonConnect["socket"] = connection.from_socket;
    jsonTo["id"] = connection.to_id;
    jsonTo["socket"] = connection.to_socket;

    jsonConnection["connect"] = jsonConnect;
    jsonConnection["to"] = jsonTo;
    jsonConnections.push_back(jsonConnection);
  }
  jsonPackage["connections"] = jsonConnections;
}

void Package::deserialize(Json const &a_json)
{
  Element::deserialize(a_json);

  auto const &jsonNode = a_json["node"];
  auto const jsonInputsPosition = jsonNode["inputs_position"];
  auto const jsonInputsPositionX = jsonInputsPosition["x"].get<double>();
  auto const jsonInputsPositionY = jsonInputsPosition["y"].get<double>();
  auto const jsonOutputsPosition = jsonNode["outputs_position"];
  auto const jsonOutputsPositionX = jsonOutputsPosition["x"].get<double>();
  auto const jsonOutputsPositionY = jsonOutputsPosition["y"].get<double>();

  auto const &jsonPackage = a_json["package"];
  auto const &jsonElements = jsonPackage["elements"];
  auto const &jsonConnections = jsonPackage["connections"];
  auto const &jsonDescription = jsonPackage["description"].get<std::string>();
  auto const &jsonIcon = jsonPackage["icon"].get<std::string>();
  auto const &jsonPath = jsonPackage["path"].get<std::string>();

  setPackageDescription(jsonDescription);
  setPackageIcon(jsonIcon);
  setPackagePath(jsonPath);
  setInputsPosition(jsonInputsPositionX, jsonInputsPositionY);
  setOutputsPosition(jsonOutputsPositionX, jsonOutputsPositionY);

  for (auto const &temp : jsonElements) {
    auto const &elementGroup = temp["element"];
    auto const elementType = elementGroup["type"].get<std::string>();
    auto const element = add(elementType.c_str());
    element->deserialize(temp);
  }

  for (auto const &connection : jsonConnections) {
    auto const &from = connection["connect"];
    auto const &to = connection["to"];
    auto const &fromId = from["id"].get<size_t>();
    auto const &fromSocket = from["socket"].get<uint8_t>();
    auto const &toId = to["id"].get<size_t>();
    auto const &toSocket = to["socket"].get<uint8_t>();
    connect(fromId, fromSocket, toId, toSocket);
  }
}

Element *Package::add(string::hash_t a_hash)
{
  spaghetti::Registry &registry{ spaghetti::Registry::get() };

  Element *const element{ registry.createElement(a_hash) };
  assert(element);

  size_t index{};
  if (m_free.empty()) {
    index = m_data.size();
    m_data.emplace_back(element);
  } else {
    index = m_free.back();
    assert(m_data[index] == nullptr);
    m_data[index] = element;
    m_free.pop_back();
  }

  element->m_package = this;
  element->m_id = index;
  element->reset();

  return element;
}

void Package::remove(size_t a_id)
{
  assert(a_id > 0);
  assert(a_id < m_data.size());
  assert(std::find(std::begin(m_free), std::end(m_free), a_id) == std::end(m_free));

  if (m_data[a_id]->isUpdatable()) std::remove(std::begin(m_updatables), std::end(m_updatables), m_data[a_id]);

  delete m_data[a_id];
  m_data[a_id] = nullptr;
  m_free.emplace_back(a_id);
}

Element *Package::get(size_t a_id) const
{
  assert(a_id < m_data.size());
  assert(std::find(std::begin(m_free), std::end(m_free), a_id) == std::end(m_free));
  return m_data[a_id];
}

bool Package::connect(size_t a_sourceId, uint8_t a_outputId, size_t a_targetId, uint8_t a_inputId)
{
  Element *const source{ get(a_sourceId) };
  Element *const target{ get(a_targetId) };

  spaghetti::log::trace("Connecting source: {}@{} to target: {}@{}", a_sourceId, static_cast<int>(a_outputId),
                        a_targetId, static_cast<int>(a_inputId));

  if (a_sourceId != 0 && a_targetId != 0) {
    spaghetti::log::trace("Normal connect, element to element");
    assert(target->m_inputs[a_inputId].type == source->m_outputs[a_outputId].type);
    target->m_inputs[a_inputId].id = a_sourceId;
    target->m_inputs[a_inputId].slot = a_outputId;
  } else if (a_sourceId == 0) {
    spaghetti::log::trace("Package connect, package input to element input");
    target->m_inputs[a_inputId].id = a_sourceId;
    target->m_inputs[a_inputId].slot = a_outputId;
  } else if (a_targetId == 0) {
    spaghetti::log::trace("Package connect, element output to package output");
  }

  spaghetti::log::trace("Notifying {}({})@{} when {}({})@{} changes..", a_targetId, target->name(),
                        static_cast<int32_t>(a_inputId), a_sourceId, source->name(), static_cast<int32_t>(a_outputId));

  m_connections.emplace_back(Connection{ a_sourceId, a_outputId, a_targetId, a_inputId });

  auto &dependencies = m_dependencies[a_sourceId];
  auto const it = std::find(std::begin(dependencies), std::end(dependencies), a_targetId);
  if (it == std::end(dependencies)) dependencies.push_back(a_targetId);

  return true;
}

bool Package::disconnect(size_t a_sourceId, uint8_t a_outputId, size_t a_targetId, uint8_t a_inputId)
{
  Element *const target{ get(a_targetId) };

  spaghetti::log::trace("Disconnecting source: {}@{} from target: {}@{}", a_sourceId, static_cast<int>(a_outputId),
                        a_targetId, static_cast<int>(a_inputId));

  target->m_inputs[a_inputId].id = 0;
  target->m_inputs[a_inputId].slot = 0;

  auto it = std::remove_if(std::begin(m_connections), std::end(m_connections), [=](Connection &a_connection) {
    return a_connection.from_id == a_sourceId && a_connection.from_socket == a_outputId &&
           a_connection.to_id == a_targetId && a_connection.to_socket == a_inputId;
  });
  m_connections.erase(it, std::end(m_connections));

  auto &dependencies = m_dependencies[a_sourceId];
  dependencies.erase(std::find(std::begin(dependencies), std::end(dependencies), a_targetId), std::end(dependencies));

  return true;
}

void Package::dispatchThreadFunction()
{
  while (!m_quit) tryDispatch();
}

void Package::updatesThreadFunction()
{
  using clock_t = std::chrono::high_resolution_clock;

  auto last = clock_t::now();

  while (!m_quit) {
    auto const now = clock_t::now();
    auto const delta = now - last;
    for (auto &&updatable : m_updatables) updatable->update(delta);
    last = now;

    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }
}

void Package::startDispatchThread()
{
  m_updatesThread = std::thread(&Package::updatesThreadFunction, this);
  m_dispatchThread = std::thread(&Package::dispatchThreadFunction, this);
}

void Package::quitDispatchThread()
{
  m_quit = true;
  if (m_updatesThread.joinable()) {
    spaghetti::log::debug("Waiting for updates thread join..");
    m_updatesThread.join();
    spaghetti::log::debug("After updates thread join..");
  }
  if (m_dispatchThread.joinable()) {
    spaghetti::log::debug("Waiting for dispatch thread join..");
    m_dispatchThread.join();
    spaghetti::log::debug("After dispatch thread join..");
  }
}

bool Package::tryDispatch()
{
  size_t id{};
  bool const dequeued{ m_queue.try_dequeue(id) };
  if (dequeued) {
    spaghetti::log::trace("Dequeued id: {}", id);
    dispatch(id);
  }

  return dequeued;
}

void Package::dispatch(size_t a_id)
{
  Element *const source{ get(a_id) };
  if (source->m_callback) source->m_callback(source);

  if (m_dependencies.find(a_id) == std::end(m_dependencies)) {
    spaghetti::log::trace("Dependencies list for id: {}({}) don't exist.", a_id, source->name());
    return;
  }

  if (m_dependencies[a_id].empty()) {
    spaghetti::log::trace("Dependencies list for id: {}({}) is empty.", a_id, source->name());
    return;
  }

  spaghetti::log::trace("Dispatching dependencies for id: {}({}) (callbacks: {})", a_id, source->name(),
                        m_dependencies[a_id].size());

  auto const &dependencies = m_dependencies[a_id];
  size_t const SIZE = dependencies.size();
  for (size_t i = 0; i < SIZE; ++i) {
    size_t const ID = dependencies[i];
    Element *const element{ get(ID) };
    spaghetti::log::trace("Recalculating id: {}({}) because id: {}({}) changed. (callbacks: {})", ID, element->name(),
                          a_id, source->name(), m_dependencies[a_id].size());
    element->calculate();
  }
}

void Package::open(std::string const &a_filename)
{
  std::ifstream file{ a_filename };
  if (!file.is_open()) return;

  Json json{};
  file >> json;

  deserialize(json);
}

void Package::save(std::string const &a_filename)
{
  Json json{};
  serialize(json);

  std::ofstream file{ a_filename };
  file << json.dump(2);
}

} // namespace spaghetti
