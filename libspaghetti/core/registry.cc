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

  registerElement<Package>("Package", ":/logic/package.png");

  registerElement<arithmetic::Add>("Add (Float)", ":/unknown.png");
  registerElement<arithmetic::AddIf>("Add If (Float)", ":/unknown.png");
  registerElement<arithmetic::Multiply>("Multiply (Float)", ":/unknown.png");
  registerElement<arithmetic::MultiplyIf>("Multiply If (Float)", ":/unknown.png");

  registerElement<gates::And>("AND (Bool)", ":/gates/and.png");
  registerElement<gates::Nand>("NAND (Bool)", ":/gates/nand.png");
  registerElement<gates::Nor>("NOR (Bool)", ":/gates/nor.png");
  registerElement<gates::Not>("NOT (Bool)", ":/gates/not.png");
  registerElement<gates::Or>("OR (Bool)", ":/gates/or.png");

  registerElement<logic::IfGreaterEqual>("If A >= B (Float)", ":/unknown.png");
  registerElement<logic::IfGreater>("If A > B (Float)", ":/unknown.png");
  registerElement<logic::IfEqual>("If A == B (Float)", ":/unknown.png");
  registerElement<logic::IfLower>("If A < B (Float)", ":/unknown.png");
  registerElement<logic::IfLowerEqual>("If A <= B (Float)", ":/unknown.png");

  registerElement<logic::Blinker, nodes::logic::Blinker>("Blinker (Bool)", ":/unknown.png");
  registerElement<logic::Clock, nodes::logic::Clock>("Clock (ms)", ":/logic/clock.png");
  registerElement<logic::Switch>("Switch (Int)", ":/logic/switch.png");

  registerElement<ui::FloatInfo, nodes::ui::FloatInfo>("Info (Float)", ":/values/const_float.png");
  registerElement<ui::IntInfo, nodes::ui::IntInfo>("Info (Int)", ":/values/const_int.png");
  registerElement<ui::PushButton, nodes::ui::PushButton>("Push Button (Bool)", ":/ui/push_button.png");
  registerElement<ui::ToggleButton, nodes::ui::ToggleButton>("Toggle Button (Bool)", ":/ui/toggle_button.png");

  registerElement<values::ConstBool, nodes::values::ConstBool>("Const value (Bool)", ":/values/const_bool.png");
  registerElement<values::ConstFloat, nodes::values::ConstFloat>("Const value (Float)", ":/values/const_float.png");
  registerElement<values::ConstInt, nodes::values::ConstInt>("Const value (Int)", ":/values/const_int.png");
  registerElement<values::RandomBool>("Random value (Bool)", ":/values/random_value.png");

  registerElement<values::Degree2Radian>("Convert angle (Deg2Rad)", ":/unknown.png");
  registerElement<values::Radian2Degree>("Convert angle (Rad2Deg)", ":/unknown.png");
  registerElement<values::Int2Float>("Convert value (Int2Float)", ":/unknown.png");
  registerElement<values::Float2Int>("Convert value (Float2Int)", ":/unknown.png");

  registerElement<values::MinInt>("Minimum value (Int)", ":/unknown.png");
  registerElement<values::MaxInt>("Maximum value (Int)", ":/unknown.png");
  registerElement<values::MinFloat>("Minimum value (Float)", ":/unknown.png");
  registerElement<values::MaxFloat>("Maximum value (Float)", ":/unknown.png");

  registerElement<values::ClampFloat>("Clamp value (Float)", ":/unknown.png");
  registerElement<values::ClampInt>("Clamp value (Int)", ":/unknown.png");
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
