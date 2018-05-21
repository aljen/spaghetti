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
#ifndef SPAGHETTI_ELEMENTS_VALUES_CHARACTERISTIC_CURVE_H
#define SPAGHETTI_ELEMENTS_VALUES_CHARACTERISTIC_CURVE_H

#include <spaghetti/element.h>
#include <spaghetti/utils.h>

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

  void setXMinimum(float const a_xMin) { m_xRange.x = a_xMin; }
  float xMinimum() const { return m_xRange.x; }

  void setXMaximum(float const a_xMax) { m_xRange.y = a_xMax; }
  float xMaximum() const { return m_xRange.y; }

  void setYMinimum(float const a_yMin) { m_yRange.x = a_yMin; }
  float yMinimum() const { return m_yRange.x; }

  void setYMaximum(float const a_yMax) { m_yRange.y = a_yMax; }
  float yMaximum() const { return m_yRange.y; }

  void setXMajorTicks(int32_t const a_xMajorTicks) { m_xTicks.x = a_xMajorTicks; }
  int32_t xMajorTicks() const { return m_xTicks.x; }

  void setXMinorTicks(int32_t const a_xMinorTicks) { m_xTicks.y = a_xMinorTicks; }
  int32_t xMinorTicks() const { return m_xTicks.y; }

  void setYMajorTicks(int32_t const a_yMajorTicks) { m_yTicks.x = a_yMajorTicks; }
  int32_t yMajorTicks() const { return m_yTicks.x; }

  void setYMinorTicks(int32_t const a_yMinorTicks) { m_yTicks.y = a_yMinorTicks; }
  int32_t yMinorTicks() const { return m_yTicks.y; }

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
  float m_lastValue{ std::numeric_limits<float>::quiet_NaN() };
};

} // namespace spaghetti::elements::values

#endif // SPAGHETTI_ELEMENTS_VALUES_CHARACTERISTIC_CURVE_H
