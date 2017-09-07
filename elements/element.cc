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

#include "elements/element.h"
#include "elements/package.h"

namespace elements {

#if 0
void Element::addInput(size_t a_id)
{
  m_manager->notifyOnChange(a_id, m_id);
  m_inputs.emplace_back(a_id);
}
#endif

bool Element::addInput(Element::Type a_type, std::string a_name)
{
  if (m_inputs.size() + 1 > m_maxInputs) return false;

  Input input{};
  input.name = a_name;
  input.type = a_type;
  m_inputs.emplace_back(input);

  return true;
}

bool Element::addOutput(Element::Type a_type, std::string a_name)
{
  if (m_outputs.size() + 1 > m_maxOutputs) return false;

  Output output{};
  output.name = a_name;
  output.type = a_type;
  m_outputs.emplace_back(output);

  return true;
}

bool Element::connect(size_t a_sourceId, uint8_t a_outputId, uint8_t a_inputId)
{
  return m_package->connect(a_sourceId, a_outputId, m_id, a_inputId);
}

void Element::setMinInputs(uint8_t a_min)
{
  if (a_min > m_maxInputs) return;
  m_minInputs = a_min;
}

void Element::setMaxInputs(uint8_t a_max)
{
  if (a_max < m_minInputs) return;
  m_maxInputs = a_max;
}

void Element::setMinOutputs(uint8_t a_min)
{
  if (a_min > m_maxOutputs) return;
  m_minOutputs = a_min;
}

void Element::setMaxOutputs(uint8_t a_max)
{
  if (a_max < m_minOutputs) return;
  m_maxOutputs = a_max;
}

} // namespace elements
