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

#include "elements/logic/blinker.h"

namespace spaghetti::elements::logic {

Blinker::Blinker()
{
  setMinInputs(3);
  setMaxInputs(3);
  setMinOutputs(1);
  setMaxOutputs(1);

  addInput(ValueType::eBool, "Enabled");
  addInput(ValueType::eInt, "High rate");
  addInput(ValueType::eInt, "Low rate");

  addOutput(ValueType::eBool, "State");
}

void Blinker::update(duration_t const &a_delta)
{
  if (m_enabled) {
    m_time += a_delta;

    auto const RATE = m_state ? m_highRate : m_lowRate;
    if (m_time >= RATE) {
      m_time = duration_t{};
      m_state = !m_state;
      m_outputs[0].value = m_state;
    }
  }
}

void Blinker::calculate()
{
  bool const ENABLED = std::get<bool>(m_inputs[0].value);
  duration_t const HIGH_RATE = duration_t{ std::get<int>(m_inputs[1].value) };
  duration_t const LOW_RATE = duration_t{ std::get<int>(m_inputs[2].value) };

  bool changed{};
  changed |= ENABLED != m_enabled;
  changed |= HIGH_RATE != m_highRate;
  changed |= LOW_RATE != m_lowRate;

  m_enabled = ENABLED;
  m_highRate = HIGH_RATE;
  m_lowRate = LOW_RATE;

  if (changed) {
    m_time = duration_t{};
    m_state = false;
    m_outputs[0].value = m_state;
  }
}

} // namespace spaghetti::elements::logic
