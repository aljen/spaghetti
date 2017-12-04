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

#include "elements/math/multiply_if.h"
#include "spaghetti/package.h"

namespace spaghetti::elements::math {

MultiplyIf::MultiplyIf()
  : Element{}
{
  setMinInputs(3);
  setMinOutputs(1);
  setMaxOutputs(1);
  addInput(ValueType::eBool, "Enabled");
  addInput(ValueType::eFloat, "A");
  addInput(ValueType::eFloat, "B");
  addOutput(ValueType::eFloat, "A * B");
}

bool MultiplyIf::calculate()
{
  if (!allInputsConnected()) return false;

  bool const enabled = std::get<bool>(*m_inputs[0].value);

  if (enabled != m_enabled && !enabled) {
    m_outputs[0].value = 0.0f;
    return true;
  }

  m_enabled = enabled;

  if (!m_enabled) return false;

  float const A = std::get<float>(*m_inputs[1].value);
  float const B = std::get<float>(*m_inputs[2].value);

  m_outputs[0].value = A * B;

  return true;
}

} // namespace spaghetti::elements::math
