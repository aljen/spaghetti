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

#include <iostream>

#include "elements/logic/clock.h"
#include "elements/package.h"

namespace elements::logic {

core::MetaData &Clock::metaData()
{
  static core::MetaData metaData{ "Clock (ms)", "logic/clock", ":/elements/logic/clock.png" };

  return metaData;
}

Clock::Clock()
  : Element{}
{
  setMinInputs(0);
  setMaxInputs(0);
  setMinOutputs(1);
  setMaxOutputs(1);
  addOutput(Type::eBool, "#1");
}

bool Clock::calculate()
{
  return false;
}

void Clock::update(time_point_t const &a_timePoint)
{
  auto const diff = a_timePoint - m_lastTimePoint;
  if (diff >= m_duration) {
    bool const value = !std::get<bool>(m_outputs[0].value);
    m_outputs[0].value = value;
    m_lastTimePoint = a_timePoint;
    m_package->elementChanged(id());
  }
}

} // namespace elements::logic
