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
  a_json["id"] = m_id;
  a_json["name"] = m_name;
  a_json["type"] = type();
  auto &node = a_json["node"];
  node["position"]["x"] = m_position.x;
  node["position"]["y"] = m_position.y;
  node["iconify"] = m_isIconified;
  auto &sockets = a_json["sockets"];
  auto &inputs = sockets["inputs"];
  inputs["min"] = m_minInputs;
  inputs["max"] = m_maxInputs;
  auto &outputs = sockets["outputs"];
  outputs["min"] = m_minOutputs;
  outputs["max"] = m_maxOutputs;
}

void Element::deserialize(Json const &a_json)
{
  std::cout << __PRETTY_FUNCTION__ << '>' << std::endl;

  auto const tempId = a_json["id"].get<size_t>();
  auto const tempName = a_json["name"].get<std::string>();
  auto const tempPosition = a_json["position"];
  auto const tempIconify = a_json["iconify"].get<bool>();
  auto const tempPositionX = tempPosition["x"].get<double>();
  auto const tempPositionY = tempPosition["y"].get<double>();
  auto const tempInputs = a_json["inputs"];
  auto const tempMinInputs = tempInputs["min"].get<uint8_t>();
  auto const tempMaxInputs = tempInputs["max"].get<uint8_t>();
  auto const tempInputsSockets = tempInputs["sockets"];
  auto const tempOutputs = a_json["outputs"];
  auto const tempMinOutputs = tempOutputs["min"].get<uint8_t>();
  auto const tempMaxOutputs = tempOutputs["max"].get<uint8_t>();
  auto const tempOutputsSockets = tempOutputs["sockets"];

  assert(id() == tempId);
  setName(tempName);
  setPosition(tempPositionX, tempPositionY);
  clearInputs();
  clearOutputs();
  setMinInputs(tempMinInputs);
  setMaxInputs(tempMaxInputs);
  setMinOutputs(tempMinOutputs);
  setMaxOutputs(tempMaxOutputs);
  iconify(tempIconify);

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
  for (auto &&socket : tempInputsSockets) add_socket(socket, true, inputsCount);
  for (auto &&socket : tempOutputsSockets) add_socket(socket, false, outputsCount);

  std::cout << __PRETTY_FUNCTION__ << '<' << std::endl;
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
  m_outputs.emplace_back(output);

  return true;
}

void Element::clearOutputs()
{
  m_outputs.clear();
}

bool Element::connect(size_t const a_sourceId, uint8_t const a_outputId, uint8_t const a_inputId)
{
  return m_package->connect(a_sourceId, a_outputId, m_id, a_inputId);
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
