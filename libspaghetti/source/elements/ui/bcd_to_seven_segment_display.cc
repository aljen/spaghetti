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
    case 0:
      m_outputs[0].value = true;  // A
      m_outputs[1].value = true;  // B
      m_outputs[2].value = true;  // C
      m_outputs[3].value = true;  // D
      m_outputs[4].value = true;  // E
      m_outputs[5].value = true;  // F
      m_outputs[6].value = false; // G
      break;
    case 1:
      m_outputs[0].value = false; // A
      m_outputs[1].value = true;  // B
      m_outputs[2].value = true;  // C
      m_outputs[3].value = false; // D
      m_outputs[4].value = false; // E
      m_outputs[5].value = false; // F
      m_outputs[6].value = false; // G
      break;
    case 2:
      m_outputs[0].value = true;  // A
      m_outputs[1].value = true;  // B
      m_outputs[2].value = false; // C
      m_outputs[3].value = true;  // D
      m_outputs[4].value = true;  // E
      m_outputs[5].value = false; // F
      m_outputs[6].value = true;  // G
      break;
    case 3:
      m_outputs[0].value = true;  // A
      m_outputs[1].value = true;  // B
      m_outputs[2].value = true;  // C
      m_outputs[3].value = true;  // D
      m_outputs[4].value = false; // E
      m_outputs[5].value = false; // F
      m_outputs[6].value = true;  // G
      break;
    case 4:
      m_outputs[0].value = false; // A
      m_outputs[1].value = true;  // B
      m_outputs[2].value = true;  // C
      m_outputs[3].value = false; // D
      m_outputs[4].value = false; // E
      m_outputs[5].value = true;  // F
      m_outputs[6].value = true;  // G
      break;
    case 5:
      m_outputs[0].value = true;  // A
      m_outputs[1].value = false; // B
      m_outputs[2].value = true;  // C
      m_outputs[3].value = true;  // D
      m_outputs[4].value = false; // E
      m_outputs[5].value = true;  // F
      m_outputs[6].value = true;  // G
      break;
    case 6:
      m_outputs[0].value = true;  // A
      m_outputs[1].value = false; // B
      m_outputs[2].value = true;  // C
      m_outputs[3].value = true;  // D
      m_outputs[4].value = true;  // E
      m_outputs[5].value = true;  // F
      m_outputs[6].value = true;  // G
      break;
    case 7:
      m_outputs[0].value = true;  // A
      m_outputs[1].value = true;  // B
      m_outputs[2].value = true;  // C
      m_outputs[3].value = false; // D
      m_outputs[4].value = false; // E
      m_outputs[5].value = false; // F
      m_outputs[6].value = false; // G
      break;
    case 8:
      m_outputs[0].value = true; // A
      m_outputs[1].value = true; // B
      m_outputs[2].value = true; // C
      m_outputs[3].value = true; // D
      m_outputs[4].value = true; // E
      m_outputs[5].value = true; // F
      m_outputs[6].value = true; // G
      break;
    case 9:
      m_outputs[0].value = true;  // A
      m_outputs[1].value = true;  // B
      m_outputs[2].value = true;  // C
      m_outputs[3].value = true;  // D
      m_outputs[4].value = false; // E
      m_outputs[5].value = true;  // F
      m_outputs[6].value = true;  // G
      break;
    default:
      m_outputs[0].value = false; // A
      m_outputs[1].value = false; // B
      m_outputs[2].value = false; // C
      m_outputs[3].value = false; // D
      m_outputs[4].value = false; // E
      m_outputs[5].value = false; // F
      m_outputs[6].value = false; // G
      break;
  }
}

} // namespace spaghetti::elements::ui
