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

#include "elements/logic/nand.h"
#include "elements/package.h"

namespace elements::logic {

core::MetaData &Nand::metaData()
{
  static core::MetaData metaData{ "NAND (Bool)", "logic/nand", ":/elements/logic/nand.png" };

  return metaData;
}

Nand::Nand()
  : Element{}
{
  setMinInputs(2);
  setMinOutputs(1);
  setMaxOutputs(1);
  addInput(ValueType::eBool, "#1");
  addInput(ValueType::eBool, "#2");
  addOutput(ValueType::eBool, "#1");
}

bool Nand::calculate()
{
  bool const currentState{ std::get<bool>(m_outputs[0].value) };

  bool allSets{ true };
  for (auto &input : m_inputs) {
    if (!input.value) return false; // TODO(awyszynski): continue instead?
    bool const v{ std::get<bool>(*input.value) };
    if (!v) {
      allSets = false;
      break;
    }
  }

  allSets = !allSets;

  if (allSets != currentState) m_outputs[0].value = allSets;

  return allSets != currentState;
}

} // namespace elements::logic
