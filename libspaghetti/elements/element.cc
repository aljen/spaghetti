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

#include "elements/element.h"

#include <cassert>
#include <iostream>

#include "elements/package.h"

namespace elements {

void Element::serialize(Element::Json &a_json)
{
  auto &jsonElement = a_json["element"];
  jsonElement["id"] = m_id;
  jsonElement["name"] = m_name;
  jsonElement["type"] = type();
  jsonElement["min_inputs"] = m_minInputs;
  jsonElement["max_inputs"] = m_maxInputs;
  jsonElement["min_outputs"] = m_minOutputs;
  jsonElement["max_outputs"] = m_maxOutputs;

  auto getSocketType = [](ValueType const a_type) {
    switch (a_type) {
      case ValueType::eBool: return "bool";
      case ValueType::eInt: return "int";
      case ValueType::eFloat: return "float";
    }
    assert(false && "Wrong socket type");
    return "unknown";
  };

  auto jsonInputs = Json::array();
  size_t const INPUTS_COUNT{ m_inputs.size() };
  for (size_t i = 0; i < INPUTS_COUNT; ++i) {
    Json socket{};
    socket["socket"] = i;
    socket["type"] = getSocketType(m_inputs[i].type);
    socket["name"] = m_inputs[i].name;
    jsonInputs.push_back(socket);
  }

  auto jsonOutputs = Json::array();
  size_t const OUTPUTS_COUNT{ m_outputs.size() };
  for (size_t i = 0; i < OUTPUTS_COUNT; ++i) {
    Json socket{};
    socket["socket"] = i;
    socket["type"] = getSocketType(m_outputs[i].type);
    socket["name"] = m_outputs[i].name;
    jsonOutputs.push_back(socket);
  }

  auto &jsonIo = jsonElement["io"];
  jsonIo["inputs"] = jsonInputs;
  jsonIo["outputs"] = jsonOutputs;

  auto &jsonNode = a_json["node"];
  jsonNode["position"]["x"] = m_position.x;
  jsonNode["position"]["y"] = m_position.y;
  jsonNode["iconify"] = m_isIconified;
}

void Element::deserialize(Json const &a_json)
{
  auto const &jsonElement = a_json["element"];
  auto const jsonId = jsonElement["id"].get<size_t>();
  auto const jsonName = jsonElement["name"].get<std::string>();
  auto const jsonMinInputs = jsonElement["min_inputs"].get<uint8_t>();
  auto const jsonMaxInputs = jsonElement["max_inputs"].get<uint8_t>();
  auto const jsonMinOutputs = jsonElement["min_outputs"].get<uint8_t>();
  auto const jsonMaxOutputs = jsonElement["max_outputs"].get<uint8_t>();

  auto const &jsonIo = jsonElement["io"];
  auto const &jsonInputs = jsonIo["inputs"];
  auto const &jsonOutputs = jsonIo["outputs"];

  auto const &jsonNode = a_json["node"];
  auto const jsonIconify = jsonNode["iconify"].get<bool>();
  auto const &jsonPosition = jsonNode["position"];
  auto const jsonPositionX = jsonPosition["x"].get<double>();
  auto const jsonPositionY = jsonPosition["y"].get<double>();

  assert(id() == jsonId);
  setName(jsonName);
  setPosition(jsonPositionX, jsonPositionY);
  clearInputs();
  clearOutputs();
  setMinInputs(jsonMinInputs);
  setMaxInputs(jsonMaxInputs);
  setMinOutputs(jsonMinOutputs);
  setMaxOutputs(jsonMaxOutputs);
  iconify(jsonIconify);

  auto add_socket = [&](Json const &a_socket, bool const a_input, uint8_t &a_socketCount) {
    auto const socketId = a_socket["socket"].get<uint8_t>();
    auto const socketStringType = a_socket["type"].get<std::string>();
    auto const socketName = a_socket["name"].get<std::string>();

    assert(a_socketCount == socketId);

    ValueType const socketType = [](std::string_view const a_type) {
      if (a_type == "bool")
        return ValueType::eBool;
      else if (a_type == "int")
        return ValueType::eInt;
      else if (a_type == "float")
        return ValueType::eFloat;
      assert(false && "Wrong socket type");
      return ValueType::eBool;
    }(socketStringType);

    a_input ? addInput(socketType, socketName) : addOutput(socketType, socketName);
    a_socketCount++;
  };

  uint8_t inputsCount{}, outputsCount{};
  for (auto &&socket : jsonInputs) add_socket(socket, true, inputsCount);
  for (auto &&socket : jsonOutputs) add_socket(socket, false, outputsCount);

}

bool Element::addInput(Element::ValueType const a_type, std::string const a_name)
{
  if (m_inputs.size() + 1 > m_maxInputs) return false;

  Input input{};
  input.name = a_name;
  input.type = a_type;
  m_inputs.emplace_back(input);

  return true;
}

void Element::setInputName(uint8_t a_input, std::string const a_name)
{
  m_inputs[a_input].name = a_name;
}

void Element::removeInput()
{
  m_inputs.pop_back();
}

void Element::clearInputs()
{
  m_inputs.clear();
}

bool Element::addOutput(Element::ValueType const a_type, std::string const a_name)
{
  if (m_outputs.size() + 1 > m_maxOutputs) return false;

  Output output{};
  output.name = a_name;
  output.type = a_type;
  switch (a_type) {
    case ValueType::eBool: output.value = false; break;
    case ValueType::eInt: output.value = 0; break;
    case ValueType::eFloat: output.value = 0.0f; break;
  }
  m_outputs.emplace_back(output);

  return true;
}

void Element::setOutputName(uint8_t a_output, std::string const a_name)
{
  m_outputs[a_output].name = a_name;
}

void Element::removeOutput()
{
  m_outputs.pop_back();
}

void Element::clearOutputs()
{
  m_outputs.clear();
}

bool Element::connect(size_t const a_sourceId, uint8_t const a_outputId, uint8_t const a_inputId)
{
  return m_package->connect(a_sourceId, a_outputId, m_id, a_inputId);
}

bool Element::allInputsConnected() const
{
  for (size_t i = 0; i < m_inputs.size(); ++i)
    if (!m_inputs[i].value) return false;

  return true;
}

void Element::setMinInputs(uint8_t const a_min)
{
  if (a_min > m_maxInputs) return;
  m_minInputs = a_min;
}

void Element::setMaxInputs(uint8_t const a_max)
{
  if (a_max < m_minInputs) return;
  m_maxInputs = a_max;
}

void Element::setMinOutputs(uint8_t const a_min)
{
  if (a_min > m_maxOutputs) return;
  m_minOutputs = a_min;
}

void Element::setMaxOutputs(uint8_t const a_max)
{
  if (a_max < m_minOutputs) return;
  m_maxOutputs = a_max;
}

} // namespace elements
