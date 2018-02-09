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

#pragma once
#ifndef ELEMENTS_VALUES_CHARACTERISTIC_CURVE_H
#define ELEMENTS_VALUES_CHARACTERISTIC_CURVE_H

#include "spaghetti/element.h"
#include "spaghetti/utils.h"

namespace spaghetti::elements::values {

template<typename A, typename B>
class CharacteristicCurve : public Element {
 public:
  using Series = std::vector<Vec2<A, B>>;

  void calculate() override
  {
    A const INPUT_VALUE{ std::get<A>(m_inputs[0].value) };
    A const VALUE{ std::clamp(INPUT_VALUE, m_xRange.x, m_xRange.y) };

    if (VALUE == m_lastValue) return;

    m_lastValue = VALUE;

    Vec2<A, B> *previousPtr{ &m_series[0] };
    Vec2<A, B> *currentPtr{};

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
    auto const PREVIOUS{ *previousPtr };
    auto const CURRENT{ *currentPtr };

    float value{};

    if (VALUE == PREVIOUS.x)
      value = static_cast<float>(PREVIOUS.y);
    else if (VALUE == CURRENT.x)
      value = static_cast<float>(CURRENT.y);
    else {
      float const PERCENT{ static_cast<float>(VALUE - PREVIOUS.x) / static_cast<float>(CURRENT.x - PREVIOUS.x) };
      value = lerp(static_cast<float>(PREVIOUS.y), static_cast<float>(CURRENT.y), PERCENT);
    }

    m_currentValue.x = VALUE;
    m_currentValue.y = static_cast<B>(value);

    m_outputs[0].value = static_cast<B>(value);
  }

  void serialize(Json &a_json) override
  {
    Element::serialize(a_json);

    auto &properties = a_json["properties"];

    properties["series"] = m_series;
    properties["x_range"] = m_xRange;
    properties["x_ticks"] = m_xTicks;
    properties["y_range"] = m_yRange;
    properties["y_ticks"] = m_yTicks;
  }

  void deserialize(Json const &a_json) override
  {
    Element::deserialize(a_json);

    auto const &PROPERTIES = a_json["properties"];

    m_series = PROPERTIES["series"].get<Series>();
    m_xRange = PROPERTIES["x_range"].get<vec2>();
    m_xTicks = PROPERTIES["x_ticks"].get<vec2>();
    m_yRange = PROPERTIES["y_range"].get<vec2>();
    m_yTicks = PROPERTIES["y_ticks"].get<vec2>();
  }

  void setSeriesCount(size_t const a_seriesCount)
  {
    if (a_seriesCount < 2) return;
    m_series.resize(a_seriesCount);
  }
  size_t seriesCount() const { return m_series.size(); }

  void setXMin(A const a_xMin) { m_xRange.x = a_xMin; }
  A xMin() const { return m_xRange.x; }

  void setXMax(A const a_xMax) { m_xRange.y = a_xMax; }
  A xMax() const { return m_xRange.y; }

  void setYMin(B const a_yMin) { m_yRange.x = a_yMin; }
  B yMin() const { return m_yRange.x; }

  void setYMax(B const a_yMax) { m_yRange.y = a_yMax; }
  B yMax() const { return m_yRange.y; }

  Series &series() { return m_series; }
  Series const &series() const { return m_series; }

  void clearSeries()
  {
    m_series.clear();
    m_series.push_back({ m_xRange.x, m_yRange.x });
    m_series.push_back({ m_xRange.y, m_yRange.y });
  }

  auto value() const { return m_currentValue; }

 protected:
  Series m_series{ { 0, 0 }, { 100, 200 } };
  Vec2<A, B> m_currentValue{};
  A m_lastValue{ -1 };
  Vec2<A, B> m_xRange{ 0, 100 };
  Vec2<A, B> m_yRange{ 0, 200 };
  Vec2<A, B> m_xTicks{ 5, 0 };
  Vec2<A, B> m_yTicks{ 5, 0 };
};

} // namespace spaghetti::elements::values

#endif // ELEMENTS_VALUES_CHARACTERISTIC_CURVE_H
