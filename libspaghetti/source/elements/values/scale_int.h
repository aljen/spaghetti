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
#ifndef ELEMENTS_VALUES_SCALE_INT_H
#define ELEMENTS_VALUES_SCALE_INT_H

#include "spaghetti/element.h"

namespace spaghetti::elements::values {

class ScaleInt final : public Element {
 public:
  static constexpr char const *const TYPE{ "values/scale_int" };
  static constexpr string::hash_t const HASH{ string::hash(TYPE) };

  ScaleInt();

  char const *type() const noexcept override { return TYPE; }
  string::hash_t hash() const noexcept override { return HASH; }

  void calculate() override;

  void serialize(Json &a_json) override;
  void deserialize(Json const &a_json) override;

  void setSeriesCount(size_t const a_seriesCount);
  size_t seriesCount() const { return m_series.size(); }

  void setXMin(int32_t const a_xMin) { m_xMin = a_xMin; }
  int32_t xMin() const { return m_xMin; }

  void setXMax(int32_t const a_xMax) { m_xMax = a_xMax; }
  int32_t xMax() const { return m_xMax; }

  void setYMin(int32_t const a_yMin) { m_yMin = a_yMin; }
  int32_t yMin() const { return m_yMin; }

  void setYMax(int32_t const a_yMax) { m_yMax = a_yMax; }
  int32_t yMax() const { return m_yMax; }

 private:
  std::vector<vec2> m_series{ { 0, 0 }, { 100, 200 } };
  int32_t m_lastValue{};
  int32_t m_xMin{ 0 };
  int32_t m_xMax{ 100 };
  int32_t m_yMin{ 0 };
  int32_t m_yMax{ 200 };
};

} // namespace spaghetti::elements::values

#endif // ELEMENTS_VALUES_SCALE_INT_H
