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

#include <spaghetti/elements/values/characteristic_curve.h>

namespace spaghetti::elements::values {

CharacteristicCurve::CharacteristicCurve()
  : Element{}
{
  setMinInputs(1);
  setMaxInputs(1);
  setMinOutputs(1);
  setMaxOutputs(1);

  addInput(ValueType::eInt, "Value", IOSocket::eCanHoldInt | IOSocket::eCanHoldFloat | IOSocket::eCanChangeName);

  addOutput(ValueType::eInt, "Scaled", IOSocket::eCanHoldInt | IOSocket::eCanHoldFloat | IOSocket::eCanChangeName);

  setIconifyingHidesCentralWidget(true);

  clearSeries();
}

void CharacteristicCurve::calculate()
{
  bool const IN_FLOAT{ m_inputs[0].type == ValueType::eFloat };
  bool const OUT_FLOAT{ m_outputs[0].type == ValueType::eFloat };
  float const INPUT_VALUE = (IN_FLOAT ? std::get<float>(m_inputs[0].value)
                                     : static_cast<float>(std::get<int32_t>(m_inputs[0].value)));
  float const VALUE{ std::clamp(INPUT_VALUE, m_xRange.x, m_xRange.y) };

  if (nearly_equal(VALUE, m_lastValue)) return;

  m_lastValue = VALUE;

  vec2f *previousPtr{ &m_series[0] };
  vec2f *currentPtr{};

  size_t const SIZE = m_series.size();
  for (size_t i = 0; i < SIZE; ++i) {
    currentPtr = &m_series[i];
    if (currentPtr->x < VALUE)
      previousPtr = currentPtr;
    else
      break;
  }

  assert(previousPtr);
  assert(currentPtr);
  auto const PREVIOUS = *previousPtr;
  auto const CURRENT = *currentPtr;

  float value{};

  if (nearly_equal(VALUE, PREVIOUS.x))
    value = static_cast<float>(PREVIOUS.y);
  else if (nearly_equal(VALUE, CURRENT.x))
    value = static_cast<float>(CURRENT.y);
  else {
    float const PERCENT{ static_cast<float>(VALUE - PREVIOUS.x) / static_cast<float>(CURRENT.x - PREVIOUS.x) };
    value = lerp(static_cast<float>(PREVIOUS.y), static_cast<float>(CURRENT.y), PERCENT);
  }

  m_currentValue.x = VALUE;
  m_currentValue.y = value;

  if (OUT_FLOAT)
    m_outputs[0].value = value;
  else
    m_outputs[0].value = static_cast<int32_t>(value);
}

void CharacteristicCurve::serialize(Json &a_json)
{
  Element::serialize(a_json);

  auto &properties = a_json["properties"];

  properties["series"] = m_series;
  properties["x_range"] = m_xRange;
  properties["x_ticks"] = m_xTicks;
  properties["y_range"] = m_yRange;
  properties["y_ticks"] = m_yTicks;
}

void CharacteristicCurve::deserialize(Json const &a_json)
{
  Element::deserialize(a_json);

  auto const &PROPERTIES = a_json["properties"];

  m_series = PROPERTIES["series"].get<Series>();
  m_xRange = PROPERTIES["x_range"].get<vec2f>();
  m_xTicks = PROPERTIES["x_ticks"].get<vec2>();
  m_yRange = PROPERTIES["y_range"].get<vec2f>();
  m_yTicks = PROPERTIES["y_ticks"].get<vec2>();
}

void CharacteristicCurve::setSeriesCount(size_t const a_seriesCount)
{
  if (a_seriesCount < 2) return;
  m_series.resize(a_seriesCount);
}

void CharacteristicCurve::clearSeries()
{
  m_series.clear();
  m_series.push_back({ m_xRange.x, m_yRange.x });
  m_series.push_back({ m_xRange.y, m_yRange.y });
}

} // namespace spaghetti::elements::values
