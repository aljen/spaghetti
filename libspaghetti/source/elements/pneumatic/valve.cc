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

#include "elements/pneumatic/valve.h"

#include <cmath>

namespace spaghetti::elements::pneumatic {

constexpr float const RO = 1.2f;

Valve::Valve()
  : Element{}
{
  setMinInputs(5);
  setMaxInputs(5);
  setMinOutputs(2);
  setMaxOutputs(2);

  addInput(ValueType::eFloat, "Open [0.0..1.0]", IOSocket::eCanHoldFloat);
  addInput(ValueType::eFloat, "P1", IOSocket::eCanHoldFloat);
  addInput(ValueType::eFloat, "V1", IOSocket::eCanHoldFloat);
  addInput(ValueType::eFloat, "P2", IOSocket::eCanHoldFloat);
  addInput(ValueType::eFloat, "V2", IOSocket::eCanHoldFloat);

  addOutput(ValueType::eFloat, "ΔP1", IOSocket::eCanHoldFloat);
  addOutput(ValueType::eFloat, "ΔP2", IOSocket::eCanHoldFloat);
}

void Valve::update(duration_t const &a_delta)
{
  m_deltaS = static_cast<float>(a_delta.count()) / 1000.f;
}

void Valve::calculate()
{
  float const VALVE{ std::get<float>(m_inputs[0].value) };
  float const P1{ std::get<float>(m_inputs[1].value) };
  float const V1{ std::get<float>(m_inputs[2].value) + 0.001f };
  float const P2{ std::get<float>(m_inputs[3].value) };
  float const V2{ std::get<float>(m_inputs[4].value) + 0.001f };

  float const ABS_DELTA_P{ std::abs(P1 - P2) };
  float const SQRT_ABS_DELTA_P{ std::sqrt((2.f / RO) * ABS_DELTA_P) };
  if (P1 - P2 >= 0.f) {
    m_deltaV = SQRT_ABS_DELTA_P;
    m_outputs[0].value = -(m_deltaP / V1);
    m_outputs[1].value = m_deltaP / V2;
  } else {
    m_deltaV = -1.f * SQRT_ABS_DELTA_P;
    m_outputs[0].value = m_deltaP / V1;
    m_outputs[1].value = -(m_deltaP / V2);
  }

  m_deltaP = VALVE * (RO * m_deltaV * m_deltaV) / 2.f * m_deltaS;
}

} // namespace spaghetti::elements::pneumatic
