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

#include <boost/dll.hpp>
#include <cassert>
#include <type_traits>

#ifdef _MSC_VER
# include <unordered_map>
#else
# include <sparsepp/spp.h>
#endif

#include "core/metadata.h"
#include "core/strings.h"

// clang-format off
#if defined(_WIN64) || defined(_WIN32)
# if defined(SPAGHETTI_SHARED)
#  if defined(SPAGHETTI_EXPORTS)
#   define SPAGHETTI_API __declspec(dllexport)
#  else
#   define SPAGHETTI_API __declspec(dllimport)
#  endif
# else
#  define SPAGHETTI_API
# endif
#else
# define SPAGHETTI_API __attribute__((visibility("default")))
#endif
// clang-format on

namespace elements {
class Element;
}
namespace nodes {
class Node;
}

#define REGISTRY_SPP_MAP 1
#define REGISTRY_STD_UNORDERED_MAP 2
#ifdef _MSC_VER
# define REGISTRY_MAP REGISTRY_STD_UNORDERED_MAP
#else
# define REGISTRY_MAP REGISTRY_SPP_MAP
#endif

namespace core {

class Registry final {
  struct Info {
    MetaData *data;
    template<typename T>
    using CloneFunc = T *(*)();
    CloneFunc<elements::Element> cloneElement{};
    CloneFunc<nodes::Node> cloneNode{};
  };

 public:
#if REGISTRY_MAP == REGISTRY_SPP_MAP
  using Elements = spp::sparse_hash_map<string::hash_t, Info>;
#elif REGISTRY_MAP == REGISTRY_STD_UNORDERED_MAP
  using Elements = std::unordered_map<string::hash_t, Info>;
#endif

  static Registry &get();

  void registerInternalElements();
  void loadPlugins();

  template<typename Element, typename Node>
  typename std::enable_if_t<(std::is_base_of_v<elements::Element, Element> && std::is_base_of_v<nodes::Node, Node>)>
  registerElement()
  {
    MetaData &metaData{ Element::metaData() };
    string::hash_t const hash{ string::hash(metaData.path) };

    if (m_elements.find(hash) != std::end(m_elements)) return;

    Info info{ &metaData, &cloneElement<Element>, &cloneNode<Node> };
    m_elements[hash] = info;
  }

  elements::Element *createElement(char const *const a_name) { return createElement(string::hash(a_name)); }
  elements::Element *createElement(string::hash_t const a_hash)
  {
    if (m_elements.find(a_hash) == std::end(m_elements)) return nullptr;
    assert(m_elements[a_hash].cloneElement);
    return m_elements[a_hash].cloneElement();
  }

  nodes::Node *createNode(char const *const a_name) { return createNode(string::hash(a_name)); }
  nodes::Node *createNode(string::hash_t const a_hash)
  {
    if (m_elements.find(a_hash) == std::end(m_elements)) return nullptr;
    assert(m_elements[a_hash].cloneNode);
    return m_elements[a_hash].cloneNode();
  }

  Elements const &elements() const { return m_elements; }

 private:
  Registry() = default;

  template<typename T>
  static elements::Element *cloneElement()
  {
    return new T;
  }

  template<typename T>
  static nodes::Node *cloneNode()
  {
    return new T;
  }

 private:
  using Plugins = std::vector<std::shared_ptr<boost::dll::shared_library>>;
  Elements m_elements{};
  Plugins m_plugins{};
};

} // namespace core

#endif // CORE_REGISTRY_H
