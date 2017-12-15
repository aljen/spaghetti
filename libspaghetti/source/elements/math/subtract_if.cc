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

#include "elements/math/subtract_if.h"

namespace spaghetti::elements::math {

SubtractIf::SubtractIf()
  : Element{}
{
  setMinInputs(3);
  setMinOutputs(1);
  setMaxOutputs(1);

  addInput(ValueType::eBool, "Enabled");
  addInput(ValueType::eFloat, "#1");
  addInput(ValueType::eFloat, "#2");

  addOutput(ValueType::eFloat, "Value");
}

void SubtractIf::calculate()
{
  bool const ENABLED{ std::get<bool>(m_inputs[0].value) };

  if (ENABLED != m_enabled && !ENABLED) {
    m_outputs[0].value = 0.0f;
    return;
  }

  m_enabled = ENABLED;

  if (!m_enabled) return;

  float ret{ std::get<float>(m_inputs[1].value) };
  size_t const SIZE{ m_inputs.size() };
  for (size_t i = 2; i < SIZE; ++i) ret -= std::get<float>(m_inputs[i].value);

  m_outputs[0].value = ret;
}

} // namespace spaghetti::elements::math
