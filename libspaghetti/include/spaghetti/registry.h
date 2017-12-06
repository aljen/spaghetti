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

#pragma once
#ifndef SPAGHETTI_REGISTRY_H
#define SPAGHETTI_REGISTRY_H

#include <cassert>
#include <type_traits>

// clang-format off
#ifdef _MSC_VER
# pragma warning(disable:4251)
# include <unordered_map>
#else
# include <sparsepp/spp.h>
#endif
// clang-format on

#include <spaghetti/api.h>
#include <spaghetti/strings.h>

namespace boost::dll {
class shared_library;
}

// clang-format off
#define REGISTRY_SPP_MAP 1
#define REGISTRY_STD_UNORDERED_MAP 2
#ifdef _MSC_VER
# define REGISTRY_MAP REGISTRY_STD_UNORDERED_MAP
#else
# define REGISTRY_MAP REGISTRY_SPP_MAP
#endif
// clang-format on

namespace spaghetti {

class Element;
class Node;

class SPAGHETTI_API Registry final {
  struct MetaInfo {
    std::string type{};
    std::string name{};
    std::string icon{};
    template<typename T>
    using CloneFunc = T *(*)();
    CloneFunc<Element> cloneElement{};
    CloneFunc<Node> cloneNode{};
  };

 public:
#if REGISTRY_MAP == REGISTRY_SPP_MAP
  using Elements = spp::sparse_hash_map<string::hash_t, MetaInfo>;
#elif REGISTRY_MAP == REGISTRY_STD_UNORDERED_MAP
  using Elements = std::unordered_map<string::hash_t, MetaInfo>;
#endif

  static Registry &get();

  void registerInternalElements();
  void loadPlugins();

  template<typename ElementDerived, typename NodeDerived = Node>
  typename std::enable_if_t<(std::is_base_of_v<Element, ElementDerived> && std::is_base_of_v<Node, NodeDerived>)>
  registerElement(std::string a_name, std::string a_icon)
  {
    string::hash_t const hash{ ElementDerived::HASH };
    assert(m_elements.find(hash) == std::end(m_elements));
    MetaInfo const info{ ElementDerived::TYPE, std::move(a_name), std::move(a_icon), &cloneElement<ElementDerived>,
                         &cloneNode<NodeDerived> };
    m_elements[hash] = info;
  }

  Element *createElement(char const *const a_name) { return createElement(string::hash(a_name)); }
  Element *createElement(string::hash_t const a_hash)
  {
    if (m_elements.find(a_hash) == std::end(m_elements)) return nullptr;
    assert(m_elements[a_hash].cloneElement);
    return m_elements[a_hash].cloneElement();
  }

  Node *createNode(char const *const a_name) { return createNode(string::hash(a_name)); }
  Node *createNode(string::hash_t const a_hash)
  {
    if (m_elements.find(a_hash) == std::end(m_elements)) return nullptr;
    assert(m_elements[a_hash].cloneNode);
    return m_elements[a_hash].cloneNode();
  }

  std::string elementName(char const *const a_name) { return elementName(string::hash(a_name)); }
  std::string elementName(string::hash_t const a_hash)
  {
    assert(m_elements.find(a_hash) != std::end(m_elements));
    return m_elements[a_hash].name;
  }

  std::string elementIcon(char const *const a_name) { return elementIcon(string::hash(a_name)); }
  std::string elementIcon(string::hash_t const a_hash)
  {
    assert(m_elements.find(a_hash) != std::end(m_elements));
    return m_elements[a_hash].icon;
  }

  Elements const &elements() const { return m_elements; }

 private:
  Registry();

  template<typename T>
  static Element *cloneElement()
  {
    return new T;
  }

  template<typename T>
  static Node *cloneNode()
  {
    return new T;
  }

 private:
  using Plugins = std::vector<std::shared_ptr<boost::dll::shared_library>>;
  Elements m_elements{};
  Plugins m_plugins{};
};

} // namespace spaghetti

#endif // SPAGHETTI_REGISTRY_H
