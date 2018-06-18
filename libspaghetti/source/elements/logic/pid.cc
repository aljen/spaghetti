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

#include <spaghetti/elements/logic/pid.h>
#include <spaghetti/utils.h>

namespace spaghetti::elements::logic {

PID::PID()
  : Element{}
{
  setMinInputs(7);
  setMaxInputs(7);
  setMinOutputs(1);
  setMaxOutputs(1);

  addInput(ValueType::eFloat, "PV", IOSocket::eCanHoldFloat);
  addInput(ValueType::eFloat, "SP", IOSocket::eCanHoldFloat);
  addInput(ValueType::eFloat, "KP", IOSocket::eCanHoldFloat);
  addInput(ValueType::eFloat, "KI", IOSocket::eCanHoldFloat);
  addInput(ValueType::eFloat, "KD", IOSocket::eCanHoldFloat);
  addInput(ValueType::eFloat, "CV High", IOSocket::eCanHoldFloat);
  addInput(ValueType::eFloat, "CV Low", IOSocket::eCanHoldFloat);

  addOutput(ValueType::eFloat, "CV", IOSocket::eCanHoldFloat);
}

void PID::update(duration_t const &a_delta)
{
  m_delta = static_cast<float>(a_delta.count()) / 1000.f;
}

void PID::calculate()
{
  float const PV{ std::get<float>(m_inputs[0].value) };
  float const SP{ std::get<float>(m_inputs[1].value) };
  float const KP{ std::get<float>(m_inputs[2].value) };
  float const KI{ std::get<float>(m_inputs[3].value) };
  float const KD{ std::get<float>(m_inputs[4].value) };
  float const CV_HIGH{ std::get<float>(m_inputs[5].value) };
  float const CV_LOW{ std::get<float>(m_inputs[6].value) };

  float const ERROR{ SP - PV };

  if (!nearly_equal(KI, 0.0f)) m_integral += (ERROR * m_delta) / KI;
  m_integral = std::clamp(m_integral, CV_LOW, CV_HIGH);

  float const P{ KP * ERROR };
  float const I{ m_integral };
  float const D{ KD * ((ERROR - m_lastError) / m_delta) };

  float const CV{ std::clamp(P + I + D, CV_LOW, CV_HIGH) };

  m_lastError = ERROR;

  m_outputs[0].value = CV;
}

} // namespace spaghetti::elements::logic
