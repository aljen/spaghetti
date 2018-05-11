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

#include "spaghetti/registry.h"

// clang-format off
#if defined(_WIN64) || defined(_WIN32)
# define WIN32_LEAN_AND_MEAN
# include <windows.h>
# include <shlobj.h>
# include <sstream>
#endif
// clang-format on

#include <algorithm>
#include <vector>

#include "filesystem.h"
#include "shared_library.h"

#include "elements/all.h"
#include "nodes/all.h"
#include "spaghetti/logger.h"
#include "spaghetti/version.h"

inline void init_resources()
{
  Q_INIT_RESOURCE(icons);
}

static std::string get_application_path()
{
#ifndef MAX_PATH
#define MAX_PATH 4098
#endif
  std::string name{};
  name.resize(MAX_PATH);
#if defined(_WIN64) || defined(_WIN32)
  GetModuleFileNameA(0, const_cast<LPSTR>(name.c_str()), MAX_PATH);
#elif defined(__linux__)
  readlink("/proc/self/exe", &name[0], MAX_PATH);
#endif
  name.resize(strlen(name.data()));

  return name;
}

namespace spaghetti {

struct Registry::PIMPL {
  using Plugins = std::vector<std::shared_ptr<SharedLibrary>>;
  using MetaInfos = std::vector<MetaInfo>;
  MetaInfos metaInfos{};
  Plugins plugins{};
  Packages packages{};
  fs::path app_path{};
  fs::path system_plugins_path{};
  fs::path user_plugins_path{};
  fs::path system_packages_path{};
  fs::path user_packages_path{};
};

Registry &Registry::get()
{
  static Registry s_registry{};
  return s_registry;
}

Registry::~Registry() = default;

Registry::Registry()
  : m_pimpl{ std::make_unique<PIMPL>() }
{
  log::init();

  log::info("Spaghetti version: {}, git: {}@{}, build date: {}, {}", version::STRING, version::BRANCH,
            version::COMMIT_SHORT_HASH, __DATE__, __TIME__);
  fs::path const APP_PATH{ fs::path{ get_application_path() }.parent_path() };
#if defined(_WIN64) || defined(_WIN32)
  wchar_t *appDataPath{};
  HRESULT result = SHGetKnownFolderPath(FOLDERID_LocalAppData, KF_FLAG_DEFAULT, nullptr, &appDataPath);
  std::wstringstream ss{};
  ss << appDataPath;
  CoTaskMemFree(static_cast<void *>(appDataPath));
  fs::path const APP_DATA_PATH{ ss.str() };

  fs::path const SYSTEM_PLUGINS_PATH{ fs::canonical(fs::path{ APP_PATH.string() + "/../Plugins" }) };
  fs::path const SYSTEM_PACKAGES_PATH{ fs::canonical(fs::path{ APP_PATH.string() + "/../Packages" }) };

  fs::path const USER_PLUGINS_PATH{ fs::absolute(APP_DATA_PATH / "Spaghetti/Plugins") };
  fs::path const USER_PACKAGES_PATH{ fs::absolute(APP_DATA_PATH / "Spaghetti/Packages") };
#else
  fs::path const HOME_PATH{ getenv("HOME") };

  fs::path const LIB_PATH{ fs::canonical(fs::path{ APP_PATH.string() + "/../lib" }) };
  fs::path const PACKAGES_PATH{ fs::canonical(fs::path{ APP_PATH.string() + "/../packages" }) };
  fs::path const SYSTEM_PLUGINS_PATH{ LIB_PATH / "spaghetti" };
  fs::path const SYSTEM_PACKAGES_PATH{ PACKAGES_PATH };

  fs::path const USER_PLUGINS_PATH{ fs::absolute(HOME_PATH / ".config/spaghetti/plugins") };
  fs::path const USER_PACKAGES_PATH{ fs::absolute(HOME_PATH / ".config/spaghetti/packages") };
#endif

  fs::create_directories(USER_PLUGINS_PATH);
  fs::create_directories(USER_PACKAGES_PATH);

  m_pimpl->app_path = APP_PATH;
  m_pimpl->system_plugins_path = SYSTEM_PLUGINS_PATH;
  m_pimpl->user_plugins_path = USER_PLUGINS_PATH;
  m_pimpl->system_packages_path = SYSTEM_PACKAGES_PATH;
  m_pimpl->user_packages_path = USER_PACKAGES_PATH;
}

void Registry::registerInternalElements()
{
  init_resources();

  using namespace elements;

  registerElement<Package, nodes::Package>("Package", ":/logic/package.png");

  registerElement<gates::And>("AND (Bool)", ":/gates/and.png");
  registerElement<gates::Nand>("NAND (Bool)", ":/gates/nand.png");
  registerElement<gates::Nor>("NOR (Bool)", ":/gates/nor.png");
  registerElement<gates::Not>("NOT (Bool)", ":/gates/not.png");
  registerElement<gates::Or>("OR (Bool)", ":/gates/or.png");

  registerElement<logic::AssignFloat>("Assign (Float)", ":/unknown.png");
  registerElement<logic::AssignInt>("Assign (Int)", ":/unknown.png");

  registerElement<logic::CounterDown>("Counter Down (Int)", ":/unknown.png");
  registerElement<logic::CounterUp>("Counter Up (Int)", ":/unknown.png");
  registerElement<logic::CounterUpDown>("Counter Up/Down (Int)", ":/unknown.png");

  registerElement<logic::IfGreaterEqual>("If A >= B (Float)", ":/unknown.png");
  registerElement<logic::IfGreater>("If A > B (Float)", ":/unknown.png");
  registerElement<logic::IfEqual>("If A == B (Float)", ":/unknown.png");
  registerElement<logic::IfLower>("If A < B (Float)", ":/unknown.png");
  registerElement<logic::IfLowerEqual>("If A <= B (Float)", ":/unknown.png");

  registerElement<logic::Latch>("Latch (Bool)", ":/unknown.png");

  registerElement<logic::MemoryDifference>("Memory Difference (Int)", ":/unknown.png");
  registerElement<logic::MemoryResetSet>("Memory RS (Bool)", ":/unknown.png");
  registerElement<logic::MemorySetReset>("Memory SR (Bool)", ":/unknown.png");

  registerElement<logic::MultiplexerInt>("Multiplexer (Int)", ":/unknown.png");
  registerElement<logic::DemultiplexerInt>("Demultiplexer (Int)", ":/unknown.png");

  registerElement<logic::Blinker, nodes::logic::Blinker>("Blinker (Bool)", ":/unknown.png");
  registerElement<logic::Switch>("Switch (Int)", ":/logic/switch.png");

  registerElement<logic::TriggerFalling>("Trigger Falling (Bool)", ":/unknown.png");
  registerElement<logic::TriggerRising>("Trigger Rising (Bool)", ":/unknown.png");

  registerElement<logic::PID>("PID", ":/unknown.png");

  registerElement<logic::SnapshotFloat>("Snapshot (Float)", ":/unknown.png");
  registerElement<logic::SnapshotInt>("Snapshot (Int)", ":/unknown.png");

  registerElement<math::Abs>("Abs (Float)", ":/unknown.png");
  registerElement<math::BCD>("BCD", ":/unknown.png");
  registerElement<math::SQRT>("Square Root (Float)", ":/unknown.png");

  registerElement<math::Add>("Add (Float)", ":/unknown.png");
  registerElement<math::AddIf>("Add If (Float)", ":/unknown.png");
  registerElement<math::Subtract>("Subtract (Float)", ":/unknown.png");
  registerElement<math::SubtractIf>("Subtract If (Float)", ":/unknown.png");
  registerElement<math::Divide>("Divide (Float)", ":/unknown.png");
  registerElement<math::DivideIf>("Divide If (Float)", ":/unknown.png");
  registerElement<math::Multiply>("Multiply (Float)", ":/unknown.png");
  registerElement<math::MultiplyIf>("Multiply If (Float)", ":/unknown.png");

  registerElement<math::Cos>("Cos (Rad)", ":/unknown.png");
  registerElement<math::Sin>("Sin (Rad)", ":/unknown.png");

  registerElement<math::Lerp>("Lerp (Float)", ":/unknown.png");
  registerElement<math::Sign>("Sign (Float)", ":/unknown.png");

  registerElement<pneumatic::Tank, nodes::pneumatic::Tank>("Tank", ":/unknown.png");
  registerElement<pneumatic::Valve>("Valve", ":/unknown.png");

  registerElement<timers::DeltaTime>("Delta Time (ms)", ":/logic/clock.png");
  registerElement<timers::Clock, nodes::timers::Clock>("Clock (ms)", ":/logic/clock.png");
  registerElement<timers::TimerOn>("T_ON", ":/logic/clock.png");
  registerElement<timers::TimerOff>("T_OFF", ":/logic/clock.png");
  registerElement<timers::TimerPulse>("T_PULSE", ":/logic/clock.png");

  registerElement<ui::BCDToSevenSegmentDisplay>("BCD -> 7SD", ":/unknown.png");

  registerElement<ui::FloatInfo, nodes::ui::FloatInfo>("Info (Float)", ":/values/const_float.png");
  registerElement<ui::IntInfo, nodes::ui::IntInfo>("Info (Int)", ":/values/const_int.png");

  registerElement<ui::PushButton, nodes::ui::PushButton>("Push Button (Bool)", ":/ui/push_button.png");
  registerElement<ui::ToggleButton, nodes::ui::ToggleButton>("Toggle Button (Bool)", ":/ui/toggle_button.png");

  registerElement<ui::SevenSegmentDisplay, nodes::ui::SevenSegmentDisplay>("7 Segment Display", ":/unknown.png");

  registerElement<values::ConstBool, nodes::values::ConstBool>("Const value (Bool)", ":/values/const_bool.png");
  registerElement<values::ConstFloat, nodes::values::ConstFloat>("Const value (Float)", ":/values/const_float.png");
  registerElement<values::ConstInt, nodes::values::ConstInt>("Const value (Int)", ":/values/const_int.png");
  registerElement<values::RandomBool>("Random value (Bool)", ":/values/random_value.png");
  registerElement<values::RandomFloat, nodes::values::RandomFloat>("Random value (Float)", ":/values/random_value.png");
  registerElement<values::RandomFloatIf, nodes::values::RandomFloatIf>("Random value If (Float)", ":/values/random_value.png");
  registerElement<values::RandomInt, nodes::values::RandomInt>("Random value (Int)", ":/values/random_value.png");
  registerElement<values::RandomIntIf, nodes::values::RandomIntIf>("Random value If (Int)", ":/values/random_value.png");

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

// clang-format off
  registerElement<values::CharacteristicCurve
#ifdef SPAGHETTI_USE_CHARTS
                  , nodes::values::CharacteristicCurve
#endif
                  >("Characteristic Curve", ":/unknown.png");
// clang-format on
}

void Registry::loadPlugins()
{
  auto loadFrom = [this](fs::path const &a_path) {
    spaghetti::log::info("Loading plugins from {}", a_path.string());
    if (!fs::is_directory(a_path)) return;
    for (auto const &ENTRY : fs::directory_iterator(a_path)) {
      spaghetti::log::info("Loading {}..", ENTRY.path().string());
      if (!(fs::is_regular_file(ENTRY) || fs::is_symlink(ENTRY))) continue;

      std::error_code error{};
      auto plugin = std::make_shared<SharedLibrary>(ENTRY, error);

      if (error.value() != 0 || !plugin->has("register_plugin")) continue;

      auto registerPlugin = plugin->get<void(Registry &)>("register_plugin");
      registerPlugin(*this);

      m_pimpl->plugins.emplace_back(std::move(plugin));
    }
  };

  auto const ADDITIONAL_PLUGINS_PATH = getenv("SPAGHETTI_ADDITIONAL_PLUGINS_PATH");
  if (ADDITIONAL_PLUGINS_PATH) loadFrom(fs::path{ ADDITIONAL_PLUGINS_PATH });

  loadFrom(m_pimpl->system_plugins_path);
  loadFrom(m_pimpl->user_plugins_path);
}

std::vector<fs::path> scan_for_dirs(fs::path const &a_path)
{
  std::vector<fs::path> ret{};

  for (auto const &ENTRY : fs::directory_iterator(a_path)) {
    if (fs::is_directory(ENTRY)) {
      auto temp = scan_for_dirs(ENTRY);
      ret.insert(std::end(ret), std::begin(temp), std::end(temp));
      ret.push_back(ENTRY);
    }
  }

  return ret;
}

void Registry::loadPackages()
{
  Packages packages{};

  auto loadFrom = [&packages](fs::path const &a_path) {
    auto directories = scan_for_dirs(a_path);
    directories.push_back(a_path);
    std::sort(std::begin(directories), std::end(directories));
    for (auto const &DIRECTORY : directories) {
      for (auto const &ENTRY : fs::directory_iterator(DIRECTORY)) {
        if (fs::is_directory(ENTRY)) continue;
        auto const FILENAME = ENTRY.path().string();
        log::warn("Loading package '{}'", FILENAME);
        packages[FILENAME] = Package::getPathFor(FILENAME);
      }
    }
  };

  loadFrom(m_pimpl->system_packages_path);
  loadFrom(m_pimpl->user_packages_path);

  log::warn("Loaded {} packages", packages.size());
  for (auto const &PACKAGE : packages) log::warn("{} as '{}'", PACKAGE.first, PACKAGE.second);

  m_pimpl->packages = packages;
}

Element *Registry::createElement(string::hash_t const a_hash)
{
  auto const &META_INFO = metaInfoFor(a_hash);
  assert(META_INFO.cloneElement);
  return META_INFO.cloneElement();
}

Node *Registry::createNode(string::hash_t const a_hash)
{
  auto const &META_INFO = metaInfoFor(a_hash);
  assert(META_INFO.cloneNode);
  return META_INFO.cloneNode();
}

std::string Registry::elementName(string::hash_t const a_hash)
{
  auto const &META_INFO = metaInfoFor(a_hash);
  return META_INFO.name;
}

std::string Registry::elementIcon(string::hash_t const a_hash)
{
  auto const &META_INFO = metaInfoFor(a_hash);
  return META_INFO.icon;
}

void Registry::addElement(MetaInfo &a_metaInfo)
{
  auto &metaInfos = m_pimpl->metaInfos;
  metaInfos.push_back(std::move(a_metaInfo));
}

bool Registry::hasElement(string::hash_t const a_hash) const
{
  auto const &META_INFOS = m_pimpl->metaInfos;
  auto const IT = std::find_if(std::begin(META_INFOS), std::end(META_INFOS),
                               [a_hash](auto const &a_metaInfo) { return a_metaInfo.hash == a_hash; });
  return IT != std::end(META_INFOS);
}

size_t Registry::size() const
{
  auto const &META_INFOS = m_pimpl->metaInfos;
  return META_INFOS.size();
}

Registry::MetaInfo const &Registry::metaInfoFor(string::hash_t const a_hash) const
{
  auto &metaInfos = m_pimpl->metaInfos;
  assert(hasElement(a_hash));
  auto const IT = std::find_if(std::begin(metaInfos), std::end(metaInfos),
                               [a_hash](auto const &a_metaInfo) { return a_metaInfo.hash == a_hash; });
  return *IT;
}

Registry::MetaInfo const &Registry::metaInfoAt(size_t const a_index) const
{
  auto const &META_INFOS = m_pimpl->metaInfos;
  return META_INFOS[a_index];
}

Registry::Packages const &Registry::packages() const
{
  return m_pimpl->packages;
}

std::string Registry::appPath() const
{
  return m_pimpl->app_path.string();
}

std::string Registry::systemPluginsPath() const
{
  return m_pimpl->system_plugins_path.string();
}

std::string Registry::userPluginsPath() const
{
  return m_pimpl->user_plugins_path.string();
}

std::string Registry::systemPackagesPath() const
{
  return m_pimpl->system_packages_path.string();
}

std::string Registry::userPackagesPath() const
{
  return m_pimpl->user_packages_path.string();
}

} // namespace spaghetti
