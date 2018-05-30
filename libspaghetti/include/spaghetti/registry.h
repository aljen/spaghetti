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
#include <vector>

#include <spaghetti/api.h>
#include <spaghetti/logger.h>
#include <spaghetti/strings.h>
#include <spaghetti/shared_library.h>
#include <spaghetti/paths.h>
#include <spaghetti/version.h>

namespace spaghetti {

class Element;

SPAGHETTI_API void register_internal_elements();
SPAGHETTI_API void load_packages();


struct PackageInfo {
  std::string filename{};
  std::string path{};
  std::string icon{};
};
using Packages = std::unordered_map<std::string, PackageInfo>;

struct TRegistryUserData {};
template<typename TType, typename TUserData = TRegistryUserData>
class TRegistry final {
 public:
  template<typename T>
  using CloneFunc = T *(*)();
  struct Info {
    std::string type{};
    CloneFunc<TType> clone{};
  };
  using Types = std::unordered_map<string::hash_t, Info>;

 private:
  using Plugins = std::vector<std::shared_ptr<SharedLibrary>>;

 public:
  static TRegistry &get();

  ~TRegistry() = default;

  void loadPlugins()
  {
    auto load_from = [this](fs::path const &a_path) {
      spaghetti::log::info("Loading plugins from {}", a_path.string());
      if (!fs::is_directory(a_path)) return;
      for (auto const &ENTRY : fs::directory_iterator(a_path)) {
        spaghetti::log::info("Loading {}..", ENTRY.path().string());
        if (!(fs::is_regular_file(ENTRY) || fs::is_symlink(ENTRY))) continue;

        std::error_code error{};
        auto plugin = std::make_shared<SharedLibrary>(ENTRY, error);

        if (error.value() != 0 || !plugin->has("register_plugin")) continue;

        auto registerPlugin = plugin->get<void(TRegistry<TType, TUserData> &)>("register_plugin");
        registerPlugin(*this);

        m_plugins.emplace_back(std::move(plugin));
      }
    };

    auto const ADDITIONAL_PLUGINS_PATH = getenv("SPAGHETTI_ADDITIONAL_PLUGINS_PATH");
    if (ADDITIONAL_PLUGINS_PATH) load_from(fs::path{ ADDITIONAL_PLUGINS_PATH });

    load_from(system_plugins_path());
    load_from(user_plugins_path());
  }

  template<typename TTypeDerived>
  typename std::enable_if_t<std::is_base_of_v<TType, TTypeDerived>>
  registerType()
  {
    auto const HASH = TTypeDerived::HASH;
    auto const TYPE = TTypeDerived::TYPE;
    assert(!has(HASH));
    m_types[HASH] = { TYPE, &clone<TTypeDerived> };
  }

  TType *create(char const *const a_name) { return create(string::hash(a_name)); }

  TType *create(string::hash_t const a_hash)
  {
    assert(has(a_hash));
    auto const createFunction = m_types.at(a_hash).clone;
    assert(createFunction);
    return createFunction();
  }

  bool has(string::hash_t const a_hash) const
  {
    return m_types.count(a_hash) == 1;
  }

  size_t size() const
  {
    return m_types.size();
  }

  TUserData &data() { return m_data; }
  TUserData const &data() const { return m_data; }

 private:
  TRegistry() = default;

  template<typename T>
  static TType *clone()
  {
    return new T;
  }

 private:
  Types m_types{};
  Plugins m_plugins{};
  TUserData m_data{};
};

struct UserData {};
using Registry = TRegistry<Element, UserData>;

SPAGHETTI_API void initialize();

} // namespace spaghetti

#endif // SPAGHETTI_REGISTRY_H
