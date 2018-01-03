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

#include "elements/math/divide_if.h"

namespace spaghetti::elements::math {

DivideIf::DivideIf()
  : Element{}
{
  setMinInputs(3);
  setMinOutputs(1);
  setMaxOutputs(1);

  addInput(ValueType::eBool, "Enabled", IOSocket::eCanHoldBool);
  addInput(ValueType::eFloat, "#1", IOSocket::eCanHoldFloat | IOSocket::eCanChangeName);
  addInput(ValueType::eFloat, "#2", IOSocket::eCanHoldFloat | IOSocket::eCanChangeName);

  addOutput(ValueType::eFloat, "Value", IOSocket::eCanHoldFloat);
}

void DivideIf::calculate()
{
  bool const ENABLED{ std::get<bool>(m_inputs[0].value) };

  if (ENABLED != m_enabled && !ENABLED) {
    m_outputs[0].value = 0.0f;
    return;
  }

  m_enabled = ENABLED;

  if (!m_enabled) return;

  float const A{ std::get<float>(m_inputs[1].value) };
  float const B{ std::get<float>(m_inputs[2].value) };
  float const VALUE{ B == 0.0f ? 0.0f : A / B };

  m_outputs[0].value = VALUE;
}

} // namespace spaghetti::elements::math
