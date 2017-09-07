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
#include "elements/package.h"
#include "elements/logic/all.h"

namespace core {

Registry& Registry::get()
{
  static Registry s_registry{};
  return s_registry;
}

void register_internal_elements()
{
  Registry &registry{ Registry::get() };

  using namespace elements;

  registry.registerElement<Package>("package");
  registry.registerElement<logic::Nand>("logic/nand");
  registry.registerElement<logic::And>("logic/and");
  registry.registerElement<logic::Nor>("logic/nor");
  registry.registerElement<logic::Or>("logic/or");
  registry.registerElement<logic::Not>("logic/not");
  registry.registerElement<logic::ConstBool>("logic/const_bool");
  registry.registerElement<logic::ConstInt>("logic/const_int");
  registry.registerElement<logic::Switch>("logic/switch");
}

} // namespace core
