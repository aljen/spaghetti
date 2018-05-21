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
#ifndef SPAGHETTI_ELEMENTS_VALUES_RANDOM_INT_H
#define SPAGHETTI_ELEMENTS_VALUES_RANDOM_INT_H

#include <random>

#include <spaghetti/element.h>

namespace spaghetti::elements::values {

class RandomInt final : public Element {
 public:
  static constexpr char const *const TYPE{ "values/random_int" };
  static constexpr string::hash_t const HASH{ string::hash(TYPE) };

  RandomInt();

  char const *type() const noexcept override { return TYPE; }
  string::hash_t hash() const noexcept override { return HASH; }

  void serialize(Json &a_json) override;
  void deserialize(Json const &a_json) override;

  void calculate() override;

  void setMin(int32_t const a_min) { setRange(a_min, max()); }
  int32_t min() const { return m_min; }
  void setMax(int32_t const a_max) { setRange(min(), a_max); }
  int32_t max() const { return m_max; }

  void setRange(int32_t const a_min, int32_t const a_max)
  {
    m_min = std::min<int32_t>(a_min, a_max);
    m_max = std::max<int32_t>(a_min, a_max);

    updateDistribution();
  }

 private:
  void updateDistribution() { m_distrib = std::uniform_int_distribution<int32_t>(m_min, m_max); }

 private:
  int32_t m_min{ 0 };
  int32_t m_max{ 100 };
  std::uniform_int_distribution<int32_t> m_distrib{ m_min, m_max };
  bool m_state{};
};

} // namespace spaghetti::elements::values

#endif // SPAGHETTI_ELEMENTS_VALUES_RANDOM_INT_H
