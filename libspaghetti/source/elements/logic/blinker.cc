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

#include "elements/logic/blinker.h"
#include "spaghetti/package.h"

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

Blinker::~Blinker() {}

void Blinker::update(duration_t const &a_delta)
{
  if (m_enabled) {
    m_time += a_delta;

    auto const rate = m_state ? m_highRate : m_lowRate;
    if (m_time >= rate) {
      m_time = duration_t{};
      m_state = !m_state;
      m_outputs[0].value = m_state;
      m_package->elementChanged(id());
    }
  }
}

bool Blinker::calculate()
{
  if (!allInputsConnected()) return false;

  bool const enabled = std::get<bool>(*m_inputs[0].value);
  duration_t const highRate = duration_t{ std::get<int>(*m_inputs[1].value) };
  duration_t const lowRate = duration_t{ std::get<int>(*m_inputs[2].value) };

  bool changed{};
  changed |= enabled != m_enabled;
  changed |= highRate != m_highRate;
  changed |= lowRate != m_lowRate;

  m_enabled = enabled;
  m_highRate = highRate;
  m_lowRate = lowRate;

  if (changed) {
    m_time = duration_t{};
    m_state = false;
    m_outputs[0].value = m_state;
  }

  return changed;
}

} // namespace spaghetti::elements::logic
