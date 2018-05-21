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

#include <spaghetti/elements/logic/counter_up_down.h>

namespace spaghetti::elements::logic {

CounterUpDown::CounterUpDown()
  : Element{}
{
  setMinInputs(5);
  setMaxInputs(5);
  setMinOutputs(3);
  setMaxOutputs(3);

  addInput(ValueType::eBool, "Counts up", IOSocket::eCanHoldBool);
  addInput(ValueType::eBool, "Counts down", IOSocket::eCanHoldBool);
  addInput(ValueType::eBool, "Reset", IOSocket::eCanHoldBool);
  addInput(ValueType::eBool, "Load", IOSocket::eCanHoldBool);
  addInput(ValueType::eInt, "Preset value", IOSocket::eCanHoldInt);

  addOutput(ValueType::eBool, "State up", IOSocket::eCanHoldBool);
  addOutput(ValueType::eBool, "State down", IOSocket::eCanHoldBool);
  addOutput(ValueType::eInt, "Current value", IOSocket::eCanHoldInt);
}

void CounterUpDown::calculate()
{
  bool const CU{ std::get<bool>(m_inputs[0].value) };
  bool const CD{ std::get<bool>(m_inputs[1].value) };
  bool const RESET{ std::get<bool>(m_inputs[2].value) };
  bool const LOAD{ std::get<bool>(m_inputs[3].value) };
  int32_t const PRESET_VALUE{ std::get<int32_t>(m_inputs[4].value) };

  if (RESET != m_lastReset && RESET) {
    m_preset = PRESET_VALUE;
    m_current = 0;
    m_stateCU = false;
  }

  if (LOAD != m_lastLoad && LOAD) {
    m_preset = PRESET_VALUE;
    m_current = PRESET_VALUE;
    m_stateCD = false;
  }

  if (CD != m_lastCD && CD && m_current > 0) --m_current;
  if (CU != m_lastCU && CU && m_current < m_preset) ++m_current;

  m_stateCD = m_current == 0;
  m_stateCU = m_current == m_preset;

  m_outputs[0].value = m_stateCU;
  m_outputs[1].value = m_stateCD;
  m_outputs[2].value = m_current;

  m_lastCU = CU;
  m_lastCD = CD;
  m_lastReset = RESET;
  m_lastLoad = LOAD;
}

} // namespace spaghetti::elements::logic
