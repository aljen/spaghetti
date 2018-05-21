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

#include <spaghetti/elements/pneumatic/tank.h>

namespace spaghetti::elements::pneumatic {

constexpr float const INITIAL_PRESSURE = 1013.f;
constexpr float const INITIAL_VOLUME = 10.f;

Tank::Tank()
  : Element{}
  , m_initialPressure{ INITIAL_PRESSURE }
  , m_pressure{ INITIAL_PRESSURE }
  , m_volume{ INITIAL_VOLUME }
{
  setMinInputs(1);
  setMinOutputs(2);
  setMaxOutputs(2);

  addInput(ValueType::eFloat, "ΔP", IOSocket::eCanHoldFloat | IOSocket::eCanChangeName);

  addOutput(ValueType::eFloat, "P", IOSocket::eCanHoldFloat);
  addOutput(ValueType::eFloat, "V", IOSocket::eCanHoldFloat);

  setDefaultNewInputFlags(IOSocket::eCanHoldFloat | IOSocket::eCanChangeName);
}

void Tank::serialize(Json &a_json)
{
  Element::serialize(a_json);

  auto &properties = a_json["properties"];
  properties["initial_pressure"] = m_initialPressure;
  properties["volume"] = m_volume;
}

void Tank::deserialize(Json const &a_json)
{
  Element::deserialize(a_json);

  auto const &PROPERTIES = a_json["properties"];
  m_initialPressure = PROPERTIES["initial_pressure"].get<float>();
  m_volume = PROPERTIES["volume"].get<float>();
  m_pressure = m_initialPressure;
}

void Tank::calculate()
{
  float deltaP{};
  for (auto const &input : m_inputs) deltaP += std::get<float>(input.value);
  m_pressure += deltaP;
  m_outputs[0].value = m_pressure;
  m_outputs[1].value = m_volume;
}

void Tank::setInitialPressure(float const a_pressure)
{
  m_initialPressure = a_pressure;
  m_pressure = a_pressure;
}

void Tank::setVolume(float const a_volume)
{
  m_volume = a_volume;
}

} // namespace spaghetti::elements::pneumatic
