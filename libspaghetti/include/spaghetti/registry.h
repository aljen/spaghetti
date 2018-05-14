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

#pragma once
#ifndef SPAGHETTI_REGISTRY_H
#define SPAGHETTI_REGISTRY_H

// clang-format off
#ifdef _MSC_VER
# pragma warning(disable:4251)
#endif
// clang-format on

#include <cassert>
#include <memory>
#include <string>
#include <unordered_map>
#include <type_traits>

#include <spaghetti/api.h>
#include <spaghetti/strings.h>

namespace spaghetti {

class Element;
class Node;

class SPAGHETTI_API Registry final {
  struct MetaInfo {
    string::hash_t hash{};
    std::string type{};
    std::string name{};
    std::string icon{};
    template<typename T>
    using CloneFunc = T *(*)();
    CloneFunc<Element> cloneElement{};
    CloneFunc<Node> cloneNode{};
  };

 public:
  struct PackageInfo {
    std::string filename{};
    std::string path{};
    std::string icon{};
  };
  using Packages = std::unordered_map<std::string, PackageInfo>;

  static Registry &get();

  ~Registry();

  void registerInternalElements();
  void loadPlugins();
  void loadPackages();

  template<typename ElementDerived, typename NodeDerived = Node>
  typename std::enable_if_t<(std::is_base_of_v<Element, ElementDerived> && std::is_base_of_v<Node, NodeDerived>)>
  registerElement(std::string a_name, std::string a_icon)
  {
    string::hash_t const hash{ ElementDerived::HASH };
    assert(!hasElement(hash));
    MetaInfo info{ hash,
                   ElementDerived::TYPE,
                   std::move(a_name),
                   std::move(a_icon),
                   &cloneElement<ElementDerived>,
                   &cloneNode<NodeDerived> };
    addElement(info);
  }

  Element *createElement(char const *const a_name) { return createElement(string::hash(a_name)); }
  Element *createElement(string::hash_t const a_hash);

  Node *createNode(char const *const a_name) { return createNode(string::hash(a_name)); }
  Node *createNode(string::hash_t const a_hash);

  std::string elementName(char const *const a_name) { return elementName(string::hash(a_name)); }
  std::string elementName(string::hash_t const a_hash);

  std::string elementIcon(char const *const a_name) { return elementIcon(string::hash(a_name)); }
  std::string elementIcon(string::hash_t const a_hash);

  bool hasElement(string::hash_t const a_hash) const;

  size_t size() const;
  MetaInfo const &metaInfoFor(string::hash_t const a_hash) const;
  MetaInfo const &metaInfoAt(size_t const a_index) const;

  Packages const &packages() const;

  std::string appPath() const;
  std::string systemPluginsPath() const;
  std::string userPluginsPath() const;
  std::string systemPackagesPath() const;
  std::string userPackagesPath() const;

 private:
  Registry();

  void addElement(MetaInfo &a_metaInfo);

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
  struct PIMPL;
  std::unique_ptr<PIMPL> m_pimpl;
};

} // namespace spaghetti

#endif // SPAGHETTI_REGISTRY_H
