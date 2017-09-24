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

#include <boost/filesystem.hpp>

#include "core/registry.h"
#include "elements/logic/all.h"
#include "elements/element.h"
#include "elements/package.h"
#include "nodes/node.h"

namespace fs = boost::filesystem;
namespace dll = boost::dll;

inline void init_resources()
{
  Q_INIT_RESOURCE(icons);
}

namespace core {

Registry &Registry::get()
{
  static Registry s_registry{};
  return s_registry;
}

void Registry::registerInternalElements()
{
  init_resources();

  registerElement<elements::Package, nodes::Package>();
  registerElement<elements::logic::Clock, nodes::logic::Test>();
  registerElement<elements::logic::Nand, nodes::logic::Test>();
  registerElement<elements::logic::And, nodes::logic::Test>();
  registerElement<elements::logic::Nor, nodes::logic::Test>();
  registerElement<elements::logic::Or, nodes::logic::Test>();
  registerElement<elements::logic::Not, nodes::logic::Test>();
  registerElement<elements::logic::ConstBool, nodes::logic::Test>();
  registerElement<elements::logic::ConstFloat, nodes::logic::Test>();
  registerElement<elements::logic::ConstInt, nodes::logic::Test>();
  registerElement<elements::logic::RandomBool, nodes::logic::Test>();
  registerElement<elements::logic::Switch, nodes::logic::Test>();
}

void Registry::loadPlugins()
{
  fs::path const pluginsDir{ "../plugins" };

  if (!fs::is_directory(pluginsDir)) return;

  for (auto const &entry : fs::directory_iterator(pluginsDir)) {
    if (!fs::is_regular_file(entry)) continue;

    boost::system::error_code error{};
    auto plugin = std::make_shared<dll::shared_library>(entry, error, dll::load_mode::append_decorations);

    if (error != 0 || !plugin->has("register_plugin")) continue;

    auto registerPlugin = plugin->get<void(core::Registry &)>("register_plugin");
    registerPlugin(*this);

    m_plugins.emplace_back(std::move(plugin));
  }
}

} // namespace core
