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

#include <random>
#include "elements/logic/random_bool.h"
#include "elements/package.h"

std::random_device g_random{};
std::mt19937 g_generator{ g_random() };
std::bernoulli_distribution g_distrib(0.47);

namespace elements {
namespace logic {

core::MetaData &RandomBool::metaData()
{
  static core::MetaData metaData{ "Random (Bool)", "logic/random_bool", ":/elements/logic/const_value.png" };

  return metaData;
}

RandomBool::RandomBool()
  : Element{}
{
  setMinInputs(1);
  setMaxInputs(1);
  setMinOutputs(1);
  setMaxOutputs(1);
  addInput(Type::eBool, "#1");
  addOutput(Type::eBool, "#1");
}

bool RandomBool::calculate()
{
  bool const currentState{ std::get<bool>(m_outputs[0].value) };
  bool const newState{ g_distrib(g_generator) };
  if (newState != currentState)
    m_outputs[0].value = newState;
  return newState != currentState;
}

} // namespace logic
} // namespace elements
