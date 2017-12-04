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

#include "elements/logic/demultiplexer_int.h"
#include "spaghetti/package.h"
#include "spaghetti/utils.h"

namespace spaghetti::elements::logic {

DemultiplexerInt::DemultiplexerInt()
  : Element{}
{
  setMinInputs(2);
  setMaxInputs(2);
  setMinOutputs(2);
  addInput(ValueType::eInt, "Select");
  addInput(ValueType::eInt, "Value");
  addOutput(ValueType::eInt, "#1");
  addOutput(ValueType::eInt, "#2");
}

bool DemultiplexerInt::calculate()
{
  if (!allInputsConnected()) return false;

  int32_t const SELECT =
      std::clamp<int32_t>(std::get<int32_t>(*m_inputs[0].value), 0, static_cast<int32_t>(m_outputs.size()) - 1);
  int32_t const VALUE = std::get<int32_t>(*m_inputs[1].value);

  m_outputs[SELECT].value = VALUE;

  return true;
}

} // namespace spaghetti::elements::logic
