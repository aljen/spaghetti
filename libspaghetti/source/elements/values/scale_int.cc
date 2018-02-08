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

#include <algorithm>

#include "spaghetti/logger.h"
#include "spaghetti/utils.h"
#include "elements/values/scale_int.h"

namespace spaghetti::elements::values {

ScaleInt::ScaleInt()
{
  setMinInputs(1);
  setMaxInputs(1);
  setMinOutputs(1);
  setMaxOutputs(1);

  addInput(ValueType::eInt, "Value", IOSocket::eCanHoldInt | IOSocket::eCanChangeName);

  addOutput(ValueType::eInt, "Scaled", IOSocket::eCanHoldInt | IOSocket::eCanChangeName);
}

void ScaleInt::serialize(Json &a_json)
{
  Element::serialize(a_json);

  auto &properties = a_json["properties"];
  properties["x_min"] = m_xMin;
  properties["x_max"] = m_xMax;
  properties["y_min"] = m_yMin;
  properties["y_max"] = m_yMax;
}

void ScaleInt::deserialize(const Json &a_json)
{
  Element::deserialize(a_json);

  auto const &PROPERTIES = a_json["properties"];
  m_xMin = PROPERTIES["x_min"].get<int32_t>();
  m_xMax = PROPERTIES["x_max"].get<int32_t>();
  m_yMin = PROPERTIES["y_min"].get<int32_t>();
  m_yMax = PROPERTIES["y_max"].get<int32_t>();
}

void ScaleInt::calculate()
{
  int32_t const INPUT_VALUE{ std::get<int32_t>(m_inputs[0].value) };
  int32_t const VALUE{ std::clamp(INPUT_VALUE, m_xMin, m_xMax) };

  if (VALUE == m_lastValue) return;

  log::info(" input value: {}", INPUT_VALUE);
  log::info("  last value: {}", m_lastValue);
  log::info("       value: {}", VALUE);

  m_lastValue = VALUE;

  vec2 *previousPtr{ &m_series[0] };
  vec2 *currentPtr{};

  size_t const SIZE = m_series.size();
  log::info(" series count: {}", SIZE);
  for (size_t i = 0; i < SIZE; ++i) {
    currentPtr = &m_series[i];
    if (currentPtr->x < VALUE)
      previousPtr = currentPtr;
    else
      break;
  }

  assert(previousPtr);
  assert(currentPtr);
  vec2 const PREVIOUS{ *previousPtr };
  vec2 const CURRENT{ *currentPtr };

  log::info(" previous: {}, {}", PREVIOUS.x, PREVIOUS.y);
  log::info("  current: {}, {}", CURRENT.x, CURRENT.y);

  float value{};
  float percent{};

  if (VALUE == PREVIOUS.x)
    value = static_cast<float>(PREVIOUS.y);
  else if (VALUE == CURRENT.x)
    value = static_cast<float>(CURRENT.y);
  else {
    percent = static_cast<float>(VALUE - PREVIOUS.x) / static_cast<float>(CURRENT.x - PREVIOUS.x);
    value = lerp(static_cast<float>(PREVIOUS.y), static_cast<float>(CURRENT.y), percent);
  }
  log::info(" value: {}, percent: {}", value, percent);

  m_outputs[0].value = static_cast<int32_t>(value);
}

void ScaleInt::setSeriesCount(size_t const a_seriesCount)
{
  if (a_seriesCount < 2) return;

  m_series.resize(a_seriesCount);
}

} // namespace spaghetti::elements::values
