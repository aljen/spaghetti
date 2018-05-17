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

#include "elements/values/random_int.h"

namespace {
std::random_device g_random{};
std::mt19937 g_generator{ g_random() };
} // namespace

namespace spaghetti::elements::values {

RandomInt::RandomInt()
  : Element{}
{
  setMinInputs(1);
  setMaxInputs(1);
  setMinOutputs(1);
  setMaxOutputs(1);

  addInput(ValueType::eBool, "Trigger", IOSocket::eCanHoldBool);

  addOutput(ValueType::eInt, "Value", IOSocket::eCanHoldInt);
}

void RandomInt::serialize(Json &a_json)
{
  Element::serialize(a_json);

  auto &properties = a_json["properties"];
  properties["min"] = m_min;
  properties["max"] = m_max;
}

void RandomInt::deserialize(Json const &a_json)
{
  Element::deserialize(a_json);

  auto const &PROPERTIES = a_json["properties"];
  m_min = PROPERTIES["min"].get<int32_t>();
  m_max = PROPERTIES["max"].get<int32_t>();

  updateDistribution();
}

void RandomInt::calculate()
{
  bool const STATE{ std::get<bool>(m_inputs[0].value) };

  if (STATE != m_state && STATE) {
    int32_t const VALUE{ m_distrib(g_generator) };
    m_outputs[0].value = VALUE;
  }
  m_state = STATE;
}

} // namespace spaghetti::elements::values
