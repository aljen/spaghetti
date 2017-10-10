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

#pragma once
#ifndef ELEMENTS_LOGIC_CLOCK_H
#define ELEMENTS_LOGIC_CLOCK_H

#include <chrono>

#include "elements/element.h"
#include "elements/types.h"

namespace elements::logic {

class Clock final : public Element {
 public:
  static core::MetaData &metaData();
  using clock_t = std::chrono::high_resolution_clock;
  using time_point_t = std::chrono::high_resolution_clock::time_point;
  using duration_t = std::chrono::milliseconds;

  Clock();
  bool calculate() override;
  size_t type() const noexcept override { return Types::eClock; }

  void setDuration(duration_t a_duration) { m_duration = a_duration; }
  void reset(time_point_t a_timePoint) { m_lastTimePoint = a_timePoint; }
  void update(time_point_t const &a_timePoint);

 private:
  time_point_t m_lastTimePoint{};
  duration_t m_duration{ 500 };
};

} // namespace elements::logic

#endif // ELEMENTS_LOGIC_CLOCK_H
