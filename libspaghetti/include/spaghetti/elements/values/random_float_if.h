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
#ifndef SPAGHETTI_ELEMENTS_VALUES_RANDOM_FLOAT_IF_H
#define SPAGHETTI_ELEMENTS_VALUES_RANDOM_FLOAT_IF_H

#include <random>

#include <spaghetti/element.h>

namespace spaghetti::elements::values {

class RandomFloatIf final : public Element {
 public:
  static constexpr char const *const TYPE{ "values/random_float_if" };
  static constexpr string::hash_t const HASH{ string::hash(TYPE) };

  RandomFloatIf();

  char const *type() const noexcept override { return TYPE; }
  string::hash_t hash() const noexcept override { return HASH; }

  void serialize(Json &a_json) override;
  void deserialize(Json const &a_json) override;

  void update(duration_t const &a_delta) override;
  void calculate() override;

  void setEnabledMin(float const a_min) { setEnabledRange(a_min, enabledMax()); }
  float enabledMin() const { return m_enabledMin; }
  void setEnabledMax(float const a_max) { setEnabledRange(enabledMin(), a_max); }
  float enabledMax() const { return m_enabledMax; }

  void setEnabledRange(float const a_min, float const a_max)
  {
    m_enabledMin = std::min<float>(a_min, a_max);
    m_enabledMax = std::max<float>(a_min, a_max);

    updateDistributions();
  }

  void setDisabledMin(float const a_min) { setDisabledRange(a_min, disabledMax()); }
  float disabledMin() const { return m_disabledMin; }
  void setDisabledMax(float const a_max) { setDisabledRange(disabledMin(), a_max); }
  float disabledMax() const { return m_disabledMax; }

  void setDisabledRange(float const a_min, float const a_max)
  {
    m_disabledMin = std::min<float>(a_min, a_max);
    m_disabledMax = std::max<float>(a_min, a_max);

    updateDistributions();
  }

 private:
  void updateDistributions()
  {
    m_enabledDistrib = std::uniform_real_distribution<float>(m_enabledMin, m_enabledMax);
    m_disabledDistrib = std::uniform_real_distribution<float>(m_disabledMin, m_disabledMax);
  }

 private:
  float m_enabledMin{ 0 };
  float m_enabledMax{ 100 };
  float m_disabledMin{ 200 };
  float m_disabledMax{ 300 };
  duration_t m_elapsed{};
  duration_t m_enabledInterval{};
  duration_t m_disabledInterval{};
  std::uniform_real_distribution<float> m_enabledDistrib{ m_enabledMin, m_enabledMax };
  std::uniform_real_distribution<float> m_disabledDistrib{ m_disabledMin, m_disabledMax };
  float m_value{};
  bool m_enabled{};
};

} // namespace spaghetti::elements::values

#endif // SPAGHETTI_ELEMENTS_VALUES_RANDOM_FLOAT_IF_H
