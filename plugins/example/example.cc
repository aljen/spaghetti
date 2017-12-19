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

#include <cstdlib>
#include <iostream>

#include "spaghetti/element.h"
#include "spaghetti/logger.h"
#include "spaghetti/node.h"
#include "spaghetti/registry.h"

class Example final : public spaghetti::Element {
 public:
  static constexpr char const *const TYPE{ "plugins/example" };
  static constexpr spaghetti::string::hash_t const HASH{ spaghetti::string::hash(TYPE) };

  Example()
    : spaghetti::Element{}
  {
  }

  char const *type() const noexcept override { return TYPE; }
  spaghetti::string::hash_t hash() const noexcept override { return HASH; }
};

extern "C" SPAGHETTI_API void register_plugin(spaghetti::Registry &a_registry)
{
  spaghetti::log::init_from_plugin();

  a_registry.registerElement<Example>("Example (Bool)", ":/unknown.png");
}
