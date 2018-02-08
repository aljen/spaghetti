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

#include "elements/ui/bcd_to_seven_segment_display.h"

namespace spaghetti::elements::ui {

BCDToSevenSegmentDisplay::BCDToSevenSegmentDisplay()
{
  setMinInputs(4);
  setMaxInputs(4);
  setMinOutputs(8);
  setMaxOutputs(8);

  addInput(ValueType::eBool, "A", IOSocket::eCanHoldBool);
  addInput(ValueType::eBool, "B", IOSocket::eCanHoldBool);
  addInput(ValueType::eBool, "C", IOSocket::eCanHoldBool);
  addInput(ValueType::eBool, "D", IOSocket::eCanHoldBool);

  addOutput(ValueType::eBool, "A", IOSocket::eCanHoldBool);
  addOutput(ValueType::eBool, "B", IOSocket::eCanHoldBool);
  addOutput(ValueType::eBool, "C", IOSocket::eCanHoldBool);
  addOutput(ValueType::eBool, "D", IOSocket::eCanHoldBool);
  addOutput(ValueType::eBool, "E", IOSocket::eCanHoldBool);
  addOutput(ValueType::eBool, "F", IOSocket::eCanHoldBool);
  addOutput(ValueType::eBool, "G", IOSocket::eCanHoldBool);
}

void BCDToSevenSegmentDisplay::calculate()
{
  int32_t const A{ static_cast<int32_t>(std::get<bool>(m_inputs[0].value)) };
  int32_t const B{ static_cast<int32_t>(std::get<bool>(m_inputs[1].value)) };
  int32_t const C{ static_cast<int32_t>(std::get<bool>(m_inputs[2].value)) };
  int32_t const D{ static_cast<int32_t>(std::get<bool>(m_inputs[3].value)) };

  int32_t const VALUE{ (D << 3) | (C << 2) | (B << 1) | A };

  switch (VALUE) {
    case 0: setOutputs(true, true, true, true, true, true, false); break;
    case 1: setOutputs(false, true, true, false, false, false, false); break;
    case 2: setOutputs(true, true, false, true, true, false, true); break;
    case 3: setOutputs(true, true, true, true, false, false, true); break;
    case 4: setOutputs(false, true, true, false, false, true, true); break;
    case 5: setOutputs(true, false, true, true, false, true, true); break;
    case 6: setOutputs(true, false, true, true, true, true, true); break;
    case 7: setOutputs(true, true, true, false, false, false, false); break;
    case 8: setOutputs(true, true, true, true, true, true, true); break;
    case 9: setOutputs(true, true, true, true, false, true, true); break;
    default: setOutputs(false, false, false, false, false, false, false); break;
  }
}

void BCDToSevenSegmentDisplay::setOutputs(bool const a_A, bool const a_B, bool const a_C, bool const a_D,
                                          bool const a_E, bool const a_F, bool const a_G)
{
  m_outputs[0].value = a_A;
  m_outputs[1].value = a_B;
  m_outputs[2].value = a_C;
  m_outputs[3].value = a_D;
  m_outputs[4].value = a_E;
  m_outputs[5].value = a_F;
  m_outputs[6].value = a_G;
}

} // namespace spaghetti::elements::ui
