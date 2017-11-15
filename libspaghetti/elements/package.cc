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

#include "core/registry.h"
#include "elements/logic/clock.h"
#include "elements/package.h"

#define TRACE_LOGIC 0

namespace elements {

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
  std::cout << __PRETTY_FUNCTION__ << '>' << std::endl;

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
    std::cerr << "from " << fromId << "@" << static_cast<int>(fromSocket) << " to " << toId << "@"
              << static_cast<int>(toSocket) << std::endl;

    connect(fromId, fromSocket, toId, toSocket);
  }

  std::cout << __PRETTY_FUNCTION__ << '<' << std::endl;
}

Element *Package::add(string::hash_t a_hash)
{
  core::Registry &registry{ core::Registry::get() };

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

  if (element->isUpdatable()) m_addUpdatableQueue.enqueue(element);

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

  std::cerr << "source_id: " << a_sourceId << "@" << static_cast<int>(a_outputId) << " "
            << "target_id: " << a_targetId << "@" << static_cast<int>(a_inputId) << std::endl;
  if (a_sourceId != 0 && a_targetId != 0) {
    std::cerr << "normal connect, element to element" << std::endl;
    assert(target->m_inputs[a_inputId].type == source->m_outputs[a_outputId].type);
    target->m_inputs[a_inputId].id = a_sourceId;
    target->m_inputs[a_inputId].slot = a_outputId;
    target->m_inputs[a_inputId].value = &source->m_outputs[a_outputId].value;
  } else if (a_sourceId == 0) {
    std::cerr << "package connect, package input to element input" << std::endl;
    target->m_inputs[a_inputId].id = a_sourceId;
    target->m_inputs[a_inputId].slot = a_outputId;
    target->m_inputs[a_inputId].value = source->m_inputs[a_outputId].value;
  } else if (a_targetId == 0) {
    std::cerr << "package connect, element output to package output" << std::endl;
  }

#if TRACE_LOGIC
  std::cerr << "Notifying " << a_targetId << " (" << target->name() << ")";
  std::cerr << "@" << static_cast<int32_t>(a_inputId) << " when " << a_sourceId << " (" << source->name() << ")";
  std::cerr << "@" << static_cast<int32_t>(a_outputId) << " changes.." << std::endl;
#endif
  m_connections.emplace_back(Connection{ a_sourceId, a_outputId, a_targetId, a_inputId });
  m_callbacks[a_sourceId].insert(a_targetId);

  if (target->calculate()) elementChanged(a_targetId);

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
    Element *temp{};
    while (m_addUpdatableQueue.try_dequeue(temp)) m_updatables.push_back(temp);
    while (m_removeUpdatableQueue.try_dequeue(temp))
      std::remove(std::begin(m_updatables), std::end(m_updatables), temp);

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
    std::cerr << "Waiting for updates thread join.." << std::endl;
    m_updatesThread.join();
    std::cerr << "After updates thread join.." << std::endl;
  }
  if (m_dispatchThread.joinable()) {
    std::cerr << "Waiting for dispatch thread join.." << std::endl;
    m_dispatchThread.join();
    std::cerr << "After dispatch thread join.." << std::endl;
  }
}

bool Package::tryDispatch()
{
  size_t id{};
  bool const dequeued{ m_queue.try_dequeue(id) };
  if (dequeued) {
#if TRACE_LOGIC
    std::cerr << "Dequeued id: " << id << std::endl;
#endif
    dispatch(id);
  }

  return dequeued;
}

void Package::dispatch(size_t a_id)
{
  Element *const source{ get(a_id) };
  if (source->m_callback) source->m_callback(source);

  if (m_callbacks.find(a_id) == std::end(m_callbacks)) {
#if TRACE_LOGIC
    std::cerr << "Callbacks list for id: " << a_id << " (" << source->name() << ")"
              << " don't exist." << std::endl;
#endif
    return;
  }

  if (m_callbacks[a_id].empty()) {
#if TRACE_LOGIC
    std::cerr << "Callbacks list for id: " << a_id << " (" << source->name() << ")"
              << " is empty." << std::endl;
#endif
    return;
  }

#if TRACE_LOGIC
  std::cerr << "Dispatching dependencies for id: " << a_id << " (" << source->name() << ")" << std::endl;
#endif
  for (auto id : m_callbacks[a_id]) {
    Element *const element{ get(id) };
#if TRACE_LOGIC
    std::cerr << "Recalculating id: " << id << " (" << element->name() << ")"
              << " because id: " << a_id << " (" << source->name() << ")"
              << " changed." << std::endl;
#endif
    if (element->calculate()) elementChanged(id);
  }
}

void Package::elementChanged(size_t a_id)
{
  m_queue.enqueue(a_id);
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

} // namespace elements
