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

#include <spaghetti/elements/timers/delta_time.h>

namespace spaghetti::elements::timers {

DeltaTime::DeltaTime()
  : Element{}
{
  setMinInputs(0);
  setMaxInputs(0);
  setMinOutputs(2);
  setMaxOutputs(2);

  addOutput(ValueType::eInt, "Delta [ms]", IOSocket::eCanHoldInt);
  addOutput(ValueType::eFloat, "Delta [s]", IOSocket::eCanHoldFloat);
}

void DeltaTime::update(duration_t const &a_delta)
{
  m_delta = a_delta;
  m_outputs[0].value = static_cast<int32_t>(std::chrono::duration_cast<std::chrono::milliseconds>(m_delta).count());
  m_outputs[1].value = static_cast<float>(m_delta.count()) / 1000.f;
}

} // namespace spaghetti::elements::timers
