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

#include "elements/logic/trigger_falling.h"

namespace spaghetti::elements::logic {

TriggerFalling::TriggerFalling()
  : Element{}
{
  setMinInputs(1);
  setMaxInputs(1);
  setMinOutputs(1);
  setMaxOutputs(1);

  addInput(ValueType::eBool, "Input", IOSocket::eCanHoldBool);

  addOutput(ValueType::eBool, "State", IOSocket::eCanHoldBool);
}

void TriggerFalling::calculate()
{
  bool const INPUT{ std::get<bool>(m_inputs[0].value) };

  switch (m_state) {
    case State::eWait:
      if (INPUT != m_lastValue && !INPUT) m_state = State::eSet;
      break;
    case State::eSet:
      m_outputs[0].value = true;
      m_state = State::eReset;
      break;
    case State::eReset:
      m_outputs[0].value = false;
      m_state = State::eWait;
      break;
  }

  m_lastValue = INPUT;
}

} // namespace spaghetti::elements::logic
