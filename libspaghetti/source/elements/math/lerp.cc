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

#include <spaghetti/elements/math/lerp.h>
#include <spaghetti/utils.h>

namespace spaghetti::elements::math {

Lerp::Lerp()
  : Element{}
{
  setMinInputs(3);
  setMaxInputs(3);
  setMinOutputs(1);
  setMaxOutputs(1);

  addInput(ValueType::eFloat, "Min", IOSocket::eCanHoldFloat);
  addInput(ValueType::eFloat, "Max", IOSocket::eCanHoldFloat);
  addInput(ValueType::eFloat, "T", IOSocket::eCanHoldFloat);

  addOutput(ValueType::eFloat, "Value", IOSocket::eCanHoldFloat);
}

void Lerp::calculate()
{
  float const MIN = std::get<float>(m_inputs[0].value);
  float const MAX = std::get<float>(m_inputs[1].value);
  float const T = std::get<float>(m_inputs[2].value);

  m_outputs[0].value = lerp(MIN, MAX, T);
}

} // namespace spaghetti::elements::math
