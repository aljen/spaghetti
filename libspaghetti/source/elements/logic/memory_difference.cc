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

#include "elements/logic/memory_difference.h"

namespace spaghetti::elements::logic {

MemoryDifference ::MemoryDifference()
  : Element{}
{
  setMinInputs(1);
  setMaxInputs(1);
  setMinOutputs(2);
  setMaxOutputs(2);

  addInput(ValueType::eInt, "Input", IOSocket::eCanHoldInt);

  addOutput(ValueType::eInt, "Current", IOSocket::eCanHoldInt);
  addOutput(ValueType::eInt, "Previous", IOSocket::eCanHoldInt);
}

void MemoryDifference ::calculate()
{
  int32_t const INPUT{ std::get<int32_t>(m_inputs[0].value) };

  if (INPUT != m_currentValue) {
    m_lastValue = m_currentValue;
    m_currentValue = INPUT;
  }

  m_outputs[0].value = m_currentValue;
  m_outputs[1].value = m_lastValue;
}

} // namespace spaghetti::elements::logic
