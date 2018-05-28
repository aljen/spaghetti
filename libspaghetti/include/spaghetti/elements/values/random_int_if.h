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
#ifndef SPAGHETTI_ELEMENTS_VALUES_RANDOM_INT_IF_H
#define SPAGHETTI_ELEMENTS_VALUES_RANDOM_INT_IF_H

#include <random>

#include <spaghetti/element.h>

namespace spaghetti::elements::values {

class SPAGHETTI_API RandomIntIf final : public Element {
 public:
  static constexpr char const *const TYPE{ "values/random_int_if" };
  static constexpr string::hash_t const HASH{ string::hash(TYPE) };

  RandomIntIf();

  char const *type() const noexcept override { return TYPE; }
  string::hash_t hash() const noexcept override { return HASH; }

  void serialize(Json &a_json) override;
  void deserialize(Json const &a_json) override;

  void update(duration_t const &a_delta) override;
  void calculate() override;

  void setEnabledMin(int32_t const a_min) { setEnabledRange(a_min, enabledMax()); }
  int32_t enabledMin() const { return m_enabledMin; }
  void setEnabledMax(int32_t const a_max) { setEnabledRange(enabledMin(), a_max); }
  int32_t enabledMax() const { return m_enabledMax; }

  void setEnabledRange(int32_t const a_min, int32_t const a_max)
  {
    m_enabledMin = std::min<int32_t>(a_min, a_max);
    m_enabledMax = std::max<int32_t>(a_min, a_max);

    updateDistributions();
  }

  void setDisabledMin(int32_t const a_min) { setDisabledRange(a_min, disabledMax()); }
  int32_t disabledMin() const { return m_disabledMin; }
  void setDisabledMax(int32_t const a_max) { setDisabledRange(disabledMin(), a_max); }
  int32_t disabledMax() const { return m_disabledMax; }

  void setDisabledRange(int32_t const a_min, int32_t const a_max)
  {
    m_disabledMin = std::min<int32_t>(a_min, a_max);
    m_disabledMax = std::max<int32_t>(a_min, a_max);

    updateDistributions();
  }

 private:
  void updateDistributions()
  {
    m_enabledDistrib = std::uniform_int_distribution<int32_t>(m_enabledMin, m_enabledMax);
    m_disabledDistrib = std::uniform_int_distribution<int32_t>(m_disabledMin, m_disabledMax);
  }

 private:
  int32_t m_enabledMin{ 0 };
  int32_t m_enabledMax{ 100 };
  int32_t m_disabledMin{ 200 };
  int32_t m_disabledMax{ 300 };
  duration_t m_elapsed{};
  duration_t m_enabledInterval{};
  duration_t m_disabledInterval{};
  std::uniform_int_distribution<int32_t> m_enabledDistrib{ m_enabledMin, m_enabledMax };
  std::uniform_int_distribution<int32_t> m_disabledDistrib{ m_disabledMin, m_disabledMax };
  int32_t m_value{};
  bool m_enabled{};
};

} // namespace spaghetti::elements::values

#endif // SPAGHETTI_ELEMENTS_VALUES_RANDOM_INT_IF_H
