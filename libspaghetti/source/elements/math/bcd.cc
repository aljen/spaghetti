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

#include <spaghetti/elements/math/bcd.h>

namespace spaghetti::elements::math {

BCD::BCD()
  : Element{}
{
  setMinInputs(1);
  setMaxInputs(1);
  setMinOutputs(4);
  setMaxOutputs(4);

  addInput(ValueType::eInt, "Number", IOSocket::eCanHoldInt);

  addOutput(ValueType::eBool, "A", IOSocket::eCanHoldBool);
  addOutput(ValueType::eBool, "B", IOSocket::eCanHoldBool);
  addOutput(ValueType::eBool, "C", IOSocket::eCanHoldBool);
  addOutput(ValueType::eBool, "D", IOSocket::eCanHoldBool);
}

void BCD::calculate()
{
  int32_t const VALUE{ std::get<int32_t>(m_inputs[0].value) };

  m_outputs[0].value = static_cast<bool>(VALUE & (1 << 0));
  m_outputs[1].value = static_cast<bool>(VALUE & (1 << 1));
  m_outputs[2].value = static_cast<bool>(VALUE & (1 << 2));
  m_outputs[3].value = static_cast<bool>(VALUE & (1 << 3));
}

} // namespace spaghetti::elements::math
