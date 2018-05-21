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

#include <spaghetti/elements/values/random_int_if.h>

namespace {
std::random_device g_random{};
std::mt19937 g_generator{ g_random() };
} // namespace

namespace spaghetti::elements::values {

RandomIntIf::RandomIntIf()
  : Element{}
{
  setMinInputs(3);
  setMaxInputs(3);

  setMinOutputs(1);
  setMaxOutputs(1);

  addInput(ValueType::eBool, "Enabled", IOSocket::eCanHoldBool);
  addInput(ValueType::eInt, "Enabled interval [ms]", IOSocket::eCanHoldInt);
  addInput(ValueType::eInt, "Disabled interval [ms]", IOSocket::eCanHoldInt);

  addOutput(ValueType::eInt, "Value", IOSocket::eCanHoldInt);
}

void RandomIntIf::serialize(Json &a_json)
{
  Element::serialize(a_json);

  auto &properties = a_json["properties"];
  properties["enabled_min"] = m_enabledMin;
  properties["enabled_max"] = m_enabledMax;
  properties["disabled_min"] = m_disabledMin;
  properties["disabled_max"] = m_disabledMax;
}

void RandomIntIf::deserialize(Json const &a_json)
{
  Element::deserialize(a_json);

  auto const &PROPERTIES = a_json["properties"];
  m_enabledMin = PROPERTIES["enabled_min"].get<int32_t>();
  m_enabledMax = PROPERTIES["enabled_max"].get<int32_t>();
  m_disabledMin = PROPERTIES["disabled_min"].get<int32_t>();
  m_disabledMax = PROPERTIES["disabled_max"].get<int32_t>();

  updateDistributions();
}

void RandomIntIf::update(duration_t const &a_delta)
{
  m_elapsed += a_delta;
  auto const INTERVAL = m_enabled ? m_enabledInterval : m_disabledInterval;
  if (m_elapsed >= INTERVAL) {
    m_value = m_enabled ? m_enabledDistrib(g_generator) : m_disabledDistrib(g_generator);
    m_elapsed = duration_t{};
  }
}

void RandomIntIf::calculate()
{
  bool const ENABLED{ std::get<bool>(m_inputs[0].value) };
  int32_t const ENABLED_INTERVAL{ std::get<int32_t>(m_inputs[1].value) };
  int32_t const DISABLED_INTERVAL{ std::get<int32_t>(m_inputs[2].value) };

  if (ENABLED != m_enabled) m_elapsed = duration_t{};

  m_enabled = ENABLED;
  m_enabledInterval = duration_t{ ENABLED_INTERVAL };
  m_disabledInterval = duration_t{ DISABLED_INTERVAL };

  m_outputs[0].value = m_value;
}

} // namespace spaghetti::elements::values
