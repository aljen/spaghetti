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

#include <boost/dll.hpp>
#include <boost/filesystem.hpp>

#include "core/registry.h"
#include "elements/element.h"
#include "elements/logic/all.h"
#include "elements/ui/all.h"
#include "elements/package.h"
#include "nodes/ui/float_info.h"
#include "nodes/ui/push_button.h"
#include "nodes/ui/toggle_button.h"
#include "nodes/blinker.h"
#include "nodes/clock.h"
#include "nodes/const_value.h"
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

  using namespace elements;

  registerElement<Package>("Package", ":/elements/logic/package.png");

  registerElement<ui::FloatInfo, nodes::ui::FloatInfo>("Float Info", ":/unknown.png");
  registerElement<ui::PushButton, nodes::ui::PushButton>("Push Button", ":/elements/ui/push_button.png");
  registerElement<ui::ToggleButton, nodes::ui::ToggleButton>("Toggle Button", ":/elements/ui/toggle_button.png");

  registerElement<logic::Blinker, nodes::Blinker>("Blinker (bool)", ":/unknown.png");
  registerElement<logic::Clock, nodes::Clock>("Clock (ms)", ":/elements/logic/clock.png");
  registerElement<logic::Nand>("NAND (Bool)", ":/elements/logic/nand.png");
  registerElement<logic::And>("AND (Bool)", ":/elements/logic/and.png");
  registerElement<logic::Nor>("NOR (Bool)", ":/elements/logic/nor.png");
  registerElement<logic::Or>("OR (Bool)", ":/elements/logic/or.png");
  registerElement<logic::Not>("NOT (Bool)", ":/elements/logic/not.png");
  registerElement<logic::ConstBool, nodes::ConstBool>("Const value (Bool)", ":/elements/logic/const_bool.png");
  registerElement<logic::ConstFloat, nodes::ConstFloat>("Const value (Float)", ":/elements/logic/const_float.png");
  registerElement<logic::ConstInt, nodes::ConstInt>("Const value (Int)", ":/elements/logic/const_int.png");
  registerElement<logic::RandomBool>("Random (Bool)", ":/elements/logic/random_value.png");
  registerElement<logic::Switch>("Switch (Int)", ":/elements/logic/switch.png");
}

void Registry::loadPlugins()
{
  fs::path const pluginsDir{ "../plugins" };

  if (!fs::is_directory(pluginsDir)) return;

  for (auto const &entry : fs::directory_iterator(pluginsDir)) {
    if (!fs::is_regular_file(entry)) continue;

    boost::system::error_code error{};
    auto plugin = std::make_shared<dll::shared_library>(entry, error, dll::load_mode::append_decorations);

    if (error.value() != 0 || !plugin->has("register_plugin")) continue;

    auto registerPlugin = plugin->get<void(core::Registry &)>("register_plugin");
    registerPlugin(*this);

    m_plugins.emplace_back(std::move(plugin));
  }
}

} // namespace core
