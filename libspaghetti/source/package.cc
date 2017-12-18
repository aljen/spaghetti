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
  m_elements.push_back(this);

  addInput(ValueType::eBool, "#1");
  addInput(ValueType::eBool, "#2");
  addInput(ValueType::eBool, "#3");
  addOutput(ValueType::eBool, "#1");
}

Package::~Package()
{
  size_t const SIZE{ m_elements.size() };
  for (size_t i = 1; i < SIZE; ++i) delete m_elements[i];
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
  size_t const DATA_SIZE{ m_elements.size() };
  for (size_t i = 1; i < DATA_SIZE; ++i) {
    auto const element = m_elements[i];
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
    index = m_elements.size();
    m_elements.emplace_back(element);
  } else {
    index = m_free.back();
    assert(m_elements[index] == nullptr);
    m_elements[index] = element;
    m_free.pop_back();
  }

  element->m_package = this;
  element->m_id = index;
  element->reset();

  return element;
}

void Package::remove(size_t const a_id)
{
  assert(a_id > 0);
  assert(a_id < m_elements.size());
  assert(std::find(std::begin(m_free), std::end(m_free), a_id) == std::end(m_free));

  delete m_elements[a_id];
  m_elements[a_id] = nullptr;
  m_free.emplace_back(a_id);
}

Element *Package::get(size_t a_id) const
{
  assert(a_id < m_elements.size());
  assert(std::find(std::begin(m_free), std::end(m_free), a_id) == std::end(m_free));
  return m_elements[a_id];
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
    // TODO(aljen): Handle this case
    spaghetti::log::trace("Package connect, package input to element input");
    target->m_inputs[a_inputId].id = a_sourceId;
    target->m_inputs[a_inputId].slot = a_outputId;
  } else if (a_targetId == 0) {
    // TODO(aljen): Handle this case
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
  using clock_t = std::chrono::high_resolution_clock;
  auto last = clock_t::now();

  while (!m_quit) {
    auto const now = clock_t::now();
    auto const delta = now - last;
    for (auto &&connection : m_connections) {
      Element *const source{ get(connection.from_id) };
      Element *const target{ get(connection.to_id) };

      auto const &sourceOutputs = source->outputs();
      auto &targetInputs = target->inputs();
      targetInputs[connection.to_socket].value = sourceOutputs[connection.from_socket].value;
    }

    for (auto &&element : m_elements) {
      element->update(delta);
      element->calculate();
    }

//    for (auto &&element : m_elements) {
//      auto &elementInputs = element->inputs();
//      for (auto &&input : elementInputs) {
//        switch (input.type) {
//          case ValueType::eBool: input.value = false; break;
//          case ValueType::eFloat: input.value = 0.0f; break;
//          case ValueType::eInt: input.value = 0; break;
//        }
//      }
//    }

    last = now;
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }
}

void Package::startDispatchThread()
{
  m_dispatchThread = std::thread(&Package::dispatchThreadFunction, this);
}

void Package::quitDispatchThread()
{
  m_quit = true;
  if (m_dispatchThread.joinable()) {
    spaghetti::log::debug("Waiting for dispatch thread join..");
    m_dispatchThread.join();
    spaghetti::log::debug("After dispatch thread join..");
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
