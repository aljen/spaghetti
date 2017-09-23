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

#include "core/registry.h"
#include "elements/logic/all.h"
#include "elements/package.h"

namespace core {

Registry &Registry::get()
{
  static Registry s_registry{};
  return s_registry;
}

void register_internal_elements()
{
  Registry &registry{ Registry::get() };

  registry.registerElement<elements::Package, nodes::Package>();
  registry.registerElement<elements::logic::Clock, nodes::logic::Test>();
  registry.registerElement<elements::logic::Nand, nodes::logic::Test>();
  registry.registerElement<elements::logic::And, nodes::logic::Test>();
  registry.registerElement<elements::logic::Nor, nodes::logic::Test>();
  registry.registerElement<elements::logic::Or, nodes::logic::Test>();
  registry.registerElement<elements::logic::Not, nodes::logic::Test>();
  registry.registerElement<elements::logic::ConstBool, nodes::logic::Test>();
  registry.registerElement<elements::logic::ConstFloat, nodes::logic::Test>();
  registry.registerElement<elements::logic::ConstInt, nodes::logic::Test>();
  registry.registerElement<elements::logic::RandomBool, nodes::logic::Test>();
  registry.registerElement<elements::logic::Switch, nodes::logic::Test>();
}

} // namespace core
