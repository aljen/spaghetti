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

#include "core/logger.h"
#include "core/registry.h"
#include "core/version.h"
#include "elements/all.h"
#include "nodes/all.h"

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

Registry::Registry()
{
  log::init();

  log::info("Spaghetti version: {}, git: {}@{}, build date: {}, {}", version::STRING, version::BRANCH,
            version::COMMIT_SHORT_HASH, __DATE__, __TIME__);
}

void Registry::registerInternalElements()
{
  init_resources();

  using namespace elements;

  registerElement<Package>("Package", ":/elements/logic/package.png");

  registerElement<arithmetic::Add>("Add (Float)", ":/unknown.png");
  registerElement<arithmetic::AddIf>("Add If (Float)", ":/unknown.png");
  registerElement<arithmetic::Multiply>("Multiply (Float)", ":/unknown.png");
  registerElement<arithmetic::MultiplyIf>("Multiply If (Float)", ":/unknown.png");

  registerElement<const_value::Bool, nodes::const_value::Bool>("Const value (Bool)", ":/elements/logic/const_bool.png");
  registerElement<const_value::Float, nodes::const_value::Float>("Const value (Float)",
                                                                 ":/elements/logic/const_float.png");
  registerElement<const_value::Int, nodes::const_value::Int>("Const value (Int)", ":/elements/logic/const_int.png");

  registerElement<gates::Nand>("NAND (Bool)", ":/elements/logic/nand.png");
  registerElement<gates::And>("AND (Bool)", ":/elements/logic/and.png");
  registerElement<gates::Nor>("NOR (Bool)", ":/elements/logic/nor.png");
  registerElement<gates::Or>("OR (Bool)", ":/elements/logic/or.png");
  registerElement<gates::Not>("NOT (Bool)", ":/elements/logic/not.png");

  registerElement<logic::IfGreaterEqual>("If A >= B (Float)", ":/unknown.png");
  registerElement<logic::IfGreater>("If A > B (Float)", ":/unknown.png");
  registerElement<logic::IfEqual>("If A == B (Float)", ":/unknown.png");
  registerElement<logic::IfLower>("If A < B (Float)", ":/unknown.png");
  registerElement<logic::IfLowerEqual>("If A <= B (Float)", ":/unknown.png");

  registerElement<logic::Blinker, nodes::logic::Blinker>("Blinker (Bool)", ":/unknown.png");
  registerElement<logic::Clock, nodes::logic::Clock>("Clock (ms)", ":/elements/logic/clock.png");
  registerElement<logic::RandomBool>("Random (Bool)", ":/elements/logic/random_value.png");
  registerElement<logic::Switch>("Switch (Int)", ":/elements/logic/switch.png");

  registerElement<ui::FloatInfo, nodes::ui::FloatInfo>("Info (Float)", ":/unknown.png");
  registerElement<ui::PushButton, nodes::ui::PushButton>("Push Button (Bool)", ":/elements/ui/push_button.png");
  registerElement<ui::ToggleButton, nodes::ui::ToggleButton>("Toggle Button (Bool)", ":/elements/ui/toggle_button.png");
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
