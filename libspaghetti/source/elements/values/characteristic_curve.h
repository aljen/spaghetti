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

class CharacteristicCurve : public Element {
 public:
  using Series = std::vector<vec2f>;

  static constexpr char const *const TYPE{ "values/characteristic_curve" };
  static constexpr string::hash_t const HASH{ string::hash(TYPE) };

  CharacteristicCurve();

  char const *type() const noexcept override { return TYPE; }
  string::hash_t hash() const noexcept override { return HASH; }

  void calculate() override;

  void serialize(Json &a_json) override;
  void deserialize(Json const &a_json) override;

  void setSeriesCount(size_t const a_seriesCount);
  size_t seriesCount() const { return m_series.size(); }

  void setXMin(float const a_xMin) { m_xRange.x = a_xMin; }
  float xMin() const { return m_xRange.x; }

  void setXMax(float const a_xMax) { m_xRange.y = a_xMax; }
  float xMax() const { return m_xRange.y; }

  void setYMin(float const a_yMin) { m_yRange.x = a_yMin; }
  float yMin() const { return m_yRange.x; }

  void setYMax(float const a_yMax) { m_yRange.y = a_yMax; }
  float yMax() const { return m_yRange.y; }

  Series &series() { return m_series; }
  Series const &series() const { return m_series; }

  void clearSeries();

  auto value() const { return m_currentValue; }

 protected:
  Series m_series{};
  vec2f m_currentValue{};
  vec2f m_xRange{ 0.f, 100.f };
  vec2f m_yRange{ 0.f, 200.f };
  vec2 m_xTicks{ 5, 0 };
  vec2 m_yTicks{ 5, 0 };
  float m_lastValue{ 0.0 };
};

} // namespace spaghetti::elements::values

#endif // ELEMENTS_VALUES_CHARACTERISTIC_CURVE_H
