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

#include "elements/logic/demultiplexer_int.h"
#include "spaghetti/utils.h"

namespace spaghetti::elements::logic {

DemultiplexerInt::DemultiplexerInt()
  : Element{}
{
  setMinInputs(2);
  setMaxInputs(2);
  setMinOutputs(2);

  addInput(ValueType::eInt, "Select", IOSocket::eCanHoldInt);
  addInput(ValueType::eInt, "Value", IOSocket::eCanHoldInt);

  addOutput(ValueType::eInt, "#1", IOSocket::eCanHoldInt | IOSocket::eCanChangeName);
  addOutput(ValueType::eInt, "#2", IOSocket::eCanHoldInt | IOSocket::eCanChangeName);

  setDefaultNewOutputFlags(IOSocket::eCanHoldInt | IOSocket::eCanChangeName);
}

void DemultiplexerInt::calculate()
{
  int32_t const SELECT{ std::get<int32_t>(m_inputs[0].value) };
  int32_t const VALUE{ std::get<int32_t>(m_inputs[1].value) };
  int32_t const SIZE{ static_cast<int32_t>(m_outputs.size()) - 1 };
  int32_t const INDEX{ std::clamp<int32_t>(SELECT, 0, SIZE) };

  for (auto &&output : m_outputs) output.value = 0;

  m_outputs[static_cast<size_t>(INDEX)].value = VALUE;
}

} // namespace spaghetti::elements::logic
