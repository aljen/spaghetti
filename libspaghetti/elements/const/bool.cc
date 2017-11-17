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

#include "elements/const/bool.h"
#include "elements/package.h"

namespace elements::const_value {

Bool::Bool()
{
  setMinInputs(0);
  setMaxInputs(0);
  setMinOutputs(1);
  setMaxOutputs(1);
  addOutput(ValueType::eBool, "#1");
}

void Bool::serialize(Json &a_json)
{
  Element::serialize(a_json);

  auto &properties = a_json["properties"];
  properties["value"] = m_currentValue;
}

void Bool::deserialize(const Json &a_json)
{
  Element::deserialize(a_json);

  auto const &properties = a_json["properties"];
  m_currentValue = properties["value"].get<bool>();

  m_outputs[0].value = m_currentValue;
}

void Bool::toggle()
{
  m_currentValue = !m_currentValue;
  m_outputs[0].value = m_currentValue;

  m_package->elementChanged(id());
}

void Bool::set(bool a_state)
{
  if (a_state == m_currentValue) return;

  m_currentValue = a_state;
  m_outputs[0].value = a_state;

  m_package->elementChanged(id());
}

} // namespace elements::const_value
