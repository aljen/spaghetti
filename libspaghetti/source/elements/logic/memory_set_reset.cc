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

#include <spaghetti/elements/logic/memory_set_reset.h>

namespace spaghetti::elements::logic {

MemorySetReset::MemorySetReset()
  : Element{}
{
  setMinInputs(2);
  setMaxInputs(2);
  setMinOutputs(1);
  setMaxOutputs(1);

  addInput(ValueType::eBool, "Set", IOSocket::eCanHoldBool);
  addInput(ValueType::eBool, "Reset", IOSocket::eCanHoldBool);

  addOutput(ValueType::eBool, "State", IOSocket::eCanHoldBool);
}

void MemorySetReset::calculate()
{
  bool const SET{ std::get<bool>(m_inputs[0].value) };
  bool const RESET{ std::get<bool>(m_inputs[1].value) };

  if (SET)
    m_outputs[0].value = true;
  else if (RESET)
    m_outputs[0].value = false;
}

} // namespace spaghetti::elements::logic
