// MIT License
//
// Copyright (c) 2017-2018 Artur Wyszyński, aljen at hitomi dot pl
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

#include "spaghetti/logger.h"
#include "spaghetti/registry.h"

namespace spaghetti {

Package::Package()
  : Element{}
{
  m_elements.push_back(this);

  setDefaultNewInputFlags(IOSocket::eDefaultFlags);
  setDefaultNewOutputFlags(IOSocket::eDefaultFlags);
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

  if (!m_isExternal) {
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
    for (auto const &CONNECTION : m_connections) {
      Json jsonConnection{}, jsonConnect{}, jsonTo{};

      jsonConnect["id"] = CONNECTION.from_id;
      jsonConnect["socket"] = CONNECTION.from_socket;
      jsonTo["id"] = CONNECTION.to_id;
      jsonTo["socket"] = CONNECTION.to_socket;

      jsonConnection["connect"] = jsonConnect;
      jsonConnection["to"] = jsonTo;
      jsonConnections.push_back(jsonConnection);
    }
    jsonPackage["connections"] = jsonConnections;
  }
}

void Package::deserialize(Json const &a_json)
{
  Element::deserialize(a_json);

  auto const IS_ROOT = m_package == nullptr;

  auto const &NODE = a_json["node"];
  auto const INPUTS_POSITION = NODE["inputs_position"];
  auto const INPUTS_POSITION_X = INPUTS_POSITION["x"].get<double>();
  auto const INPUTS_POSITION_Y = INPUTS_POSITION["y"].get<double>();
  auto const OUTPUTS_POSITION = NODE["outputs_position"];
  auto const OUTPUTS_POSITION_X = OUTPUTS_POSITION["x"].get<double>();
  auto const OUTPUTS_POSITION_Y = OUTPUTS_POSITION["y"].get<double>();

  auto const &PACKAGE = a_json["package"];
  auto const &DESCRIPTION = PACKAGE["description"].get<std::string>();
  auto const &ICON = PACKAGE["icon"].get<std::string>();
  auto const &PATH = PACKAGE["path"].get<std::string>();

  m_isExternal = !IS_ROOT && !PATH.empty();

  spaghetti::log::debug("deserialize root? {} isExternal? {}", IS_ROOT, m_isExternal);

  Json json{};

  if (m_isExternal) {
    log::debug("Package is external one, looking for real one registered as '{}'", PATH);

    auto const &REGISTRY = Registry::get();
    auto const &PACKAGES = REGISTRY.packages();

    std::string filename{};

    for (auto const &PACKAGE_INFO : PACKAGES) {
      if (PACKAGE_INFO.second.path == PATH) {
        filename = PACKAGE_INFO.second.filename;
        log::debug("Found one, '{}' is at '{}'", PATH, filename);
        break;
      }
    }

    assert(!filename.empty() && "Can't find external package");

    std::ifstream file{ filename };
    if (!file.is_open()) return;

    file >> json;
  }

  auto const &REAL_PACKAGE = m_isExternal ? json["package"] : PACKAGE;
  auto const &ELEMENTS = REAL_PACKAGE["elements"];
  auto const &CONNECTIONS = REAL_PACKAGE["connections"];

  setPackageDescription(DESCRIPTION);
  setPackageIcon(ICON);
  setPackagePath(PATH);
  setInputsPosition(INPUTS_POSITION_X, INPUTS_POSITION_Y);
  setOutputsPosition(OUTPUTS_POSITION_X, OUTPUTS_POSITION_Y);

  std::map<size_t, size_t> remappedIds{};

  for (auto const &TEMP : ELEMENTS) {
    auto const &ELEMENT_GROUP = TEMP["element"];
    auto const ELEMENT_TYPE = ELEMENT_GROUP["type"].get<std::string>();
    auto const element = add(ELEMENT_TYPE.c_str());
    auto const ELEMENT_ID = ELEMENT_GROUP["id"].get<size_t>();
    auto const ADDED_ID = element->id();
    element->deserialize(TEMP);

    remappedIds[ELEMENT_ID] = ADDED_ID;
  }

  for (auto const &CONNECTION : CONNECTIONS) {
    auto const &FROM = CONNECTION["connect"];
    auto const &TO = CONNECTION["to"];
    auto const &FROM_ID = remappedIds[FROM["id"].get<size_t>()];
    auto const &FROM_SOCKET = FROM["socket"].get<uint8_t>();
    auto const &TO_ID = remappedIds[TO["id"].get<size_t>()];
    auto const &TO_SOCKET = TO["socket"].get<uint8_t>();
    connect(FROM_ID, FROM_SOCKET, TO_ID, TO_SOCKET);
  }
}

void Package::calculate()
{
  for (auto &&connection : m_connections) {
    Element *const source{ get(connection.from_id) };
    Element *const target{ get(connection.to_id) };

    auto const IS_SOURCE_SELF = connection.from_id == 0;
    auto const IS_TARGET_SELF = connection.to_id == 0;

    auto const &SOURCE_IO = IS_SOURCE_SELF ? source->inputs() : source->outputs();
    auto &targetIO = IS_TARGET_SELF ? target->outputs() : target->inputs();
    targetIO[connection.to_socket].value = SOURCE_IO[connection.from_socket].value;
  }

  for (auto &&element : m_elements) {
    if (!element || element == this) continue;

    element->update(m_delta);
    element->calculate();
  }
}

Element *Package::add(string::hash_t const a_hash)
{
  pauseDispatchThread();

  spaghetti::log::debug("Adding element..");

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

  resumeDispatchThread();

  return element;
}

void Package::remove(size_t const a_id)
{
  pauseDispatchThread();

  spaghetti::log::debug("Removing element {}..", a_id);

  assert(a_id > 0);
  assert(a_id < m_elements.size());
  assert(std::find(std::begin(m_free), std::end(m_free), a_id) == std::end(m_free));

  delete m_elements[a_id];
  m_elements[a_id] = nullptr;
  m_free.emplace_back(a_id);

  resumeDispatchThread();
}

Element *Package::get(size_t const a_id) const
{
  assert(a_id < m_elements.size());
  assert(std::find(std::begin(m_free), std::end(m_free), a_id) == std::end(m_free));
  return m_elements[a_id];
}

bool Package::connect(size_t const a_sourceId, uint8_t const a_outputId, size_t const a_targetId,
                      uint8_t const a_inputId)
{
  pauseDispatchThread();

  Element *const source{ get(a_sourceId) };
  Element *const target{ get(a_targetId) };

  spaghetti::log::debug("Connecting source: {}@{} to target: {}@{}", a_sourceId, static_cast<int>(a_outputId),
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
  auto const IT = std::find(std::begin(dependencies), std::end(dependencies), a_targetId);
  if (IT == std::end(dependencies)) dependencies.push_back(a_targetId);

  resumeDispatchThread();

  return true;
}

bool Package::disconnect(size_t const a_sourceId, uint8_t const a_outputId, size_t const a_targetId,
                         uint8_t const a_inputId)
{
  pauseDispatchThread();

  Element *const target{ get(a_targetId) };

  spaghetti::log::debug("Disconnecting source: {}@{} from target: {}@{}", a_sourceId, static_cast<int>(a_outputId),
                        a_targetId, static_cast<int>(a_inputId));

  auto &targetInput = target->m_inputs[a_inputId];
  targetInput.id = 0;
  targetInput.slot = 0;
  resetIOSocketValue(targetInput);

  auto it = std::remove_if(std::begin(m_connections), std::end(m_connections), [=](Connection &a_connection) {
    return a_connection.from_id == a_sourceId && a_connection.from_socket == a_outputId &&
           a_connection.to_id == a_targetId && a_connection.to_socket == a_inputId;
  });
  m_connections.erase(it, std::end(m_connections));

  auto &dependencies = m_dependencies[a_sourceId];
  dependencies.erase(std::find(std::begin(dependencies), std::end(dependencies), a_targetId), std::end(dependencies));

  resumeDispatchThread();

  return true;
}

void Package::dispatchThreadFunction()
{
  using clock_t = std::chrono::high_resolution_clock;
  auto last = clock_t::now();

  auto const ONE_MILLISECOND = std::chrono::milliseconds(1);

  while (!m_quit) {
    auto const NOW = clock_t::now();
    auto const DELTA = NOW - last;

    update(DELTA);
    calculate();

    last = NOW;

    auto const WAIT_START = clock_t::now();
    while ((clock_t::now() - WAIT_START) < ONE_MILLISECOND) std::this_thread::sleep_for(ONE_MILLISECOND);

    if (m_pause) {
      spaghetti::log::trace("Pause requested..");
      m_paused = true;
      spaghetti::log::trace("Pausing..");
      while (m_pause) std::this_thread::yield();
      m_paused = false;
      spaghetti::log::trace("Pause stopped..");
    }
  }
}

void Package::startDispatchThread()
{
  if (m_dispatchThreadStarted) return;

  spaghetti::log::trace("Starting dispatch thread..");
  m_dispatchThread = std::thread(&Package::dispatchThreadFunction, this);
  m_dispatchThreadStarted = true;
}

void Package::quitDispatchThread()
{
  if (!m_dispatchThreadStarted) return;

  spaghetti::log::trace("Quitting dispatch thread..");

  if (m_pause) {
    spaghetti::log::trace("Dispatch thread paused, waiting..");
    while (m_pause) std::this_thread::yield();
  }

  m_quit = true;
  if (m_dispatchThread.joinable()) {
    spaghetti::log::trace("Waiting for dispatch thread join..");
    m_dispatchThread.join();
    spaghetti::log::trace("After dispatch thread join..");
  }
  m_dispatchThreadStarted = false;
}

void Package::pauseDispatchThread()
{
  if (!m_dispatchThreadStarted) return;

  m_pauseCount++;

  spaghetti::log::trace("Trying to pause dispatch thread ({})..", m_pauseCount.load());

  if (m_pauseCount > 1) return;

  m_pause = true;

  spaghetti::log::trace("Pausing dispatch thread ({})..", m_pauseCount.load());
  while (!m_paused) std::this_thread::yield();
}

void Package::resumeDispatchThread()
{
  if (!m_dispatchThreadStarted) return;

  m_pauseCount--;

  spaghetti::log::trace("Trying to resume dispatch thread ({})..", m_pauseCount.load());

  if (m_pauseCount > 0) return;

  spaghetti::log::trace("Resuming dispatch thread ({})..", m_pauseCount.load());

  m_pause = false;
}

void Package::open(std::string const &a_filename)
{
  spaghetti::log::debug("Opening package {}", a_filename);

  std::ifstream file{ a_filename };
  if (!file.is_open()) return;

  pauseDispatchThread();

  Json json{};
  file >> json;

  deserialize(json);

  m_isExternal = m_package != nullptr;
  spaghetti::log::debug("{} Is external: {}", a_filename, m_isExternal);

  resumeDispatchThread();
}

void Package::save(std::string const &a_filename)
{
  spaghetti::log::debug("Saving package {}", a_filename);

  pauseDispatchThread();

  Json json{};
  serialize(json);

  std::ofstream file{ a_filename };
  file << json.dump(2);

  resumeDispatchThread();
}

Registry::PackageInfo Package::getInfoFor(std::string const &a_filename)
{
  Registry::PackageInfo type{};

  std::ifstream file{ a_filename };
  if (!file.is_open()) return type;

  Json json{};
  file >> json;

  type.filename = a_filename;
  type.icon = json["package"]["icon"];
  type.path = json["package"]["path"];

  return type;
}

} // namespace spaghetti
