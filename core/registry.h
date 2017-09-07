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

#ifndef CORE_REGISTRY_H
#define CORE_REGISTRY_H

#include <sparsepp/spp.h>
#include "core/strings.h"
#include "elements/element.h"

namespace core {

class Registry final {
 public:
  static Registry& get();

  template<typename T>
  void registerElement(char const *const a_name)
  {
    registerElement<T>(string::hash(a_name));
  }

  template<typename T>
  void registerElement(string::hash_t const a_hash)
  {
    if (m_registry.find(a_hash) != std::end(m_registry)) return;
    m_registry[a_hash] = &clone<T>;
  }

  elements::Element* create(char const *const a_name)
  {
    return create(string::hash(a_name));
  }

  elements::Element* create(string::hash_t const a_hash)
  {
    if (m_registry.find(a_hash) == std::end(m_registry)) return nullptr;
    return m_registry[a_hash]();
  }

 private:
  Registry() = default;

  template<typename T>
  static elements::Element* clone()
  {
    return new T;
  }

 private:
  using CreateElementFunc = elements::Element* (*)();
  spp::sparse_hash_map<string::hash_t, CreateElementFunc> m_registry{};
};

void register_internal_elements();

} // namespace core

#endif // CORE_REGISTRY_H
