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
#include "spaghetti/paths.h"

#include <algorithm>
#include <vector>

#include <spaghetti/elements/all.h>
#include <spaghetti/filesystem.h>
#include <spaghetti/logger.h>
#include <spaghetti/shared_library.h>
#include <spaghetti/version.h>

namespace spaghetti {

template<>
SPAGHETTI_API Registry &Registry::get()
{
  static Registry s_registry{};
  return s_registry;
}

void register_internal_elements()
{
  using namespace elements;

  auto &registry = Registry::get();

  registry.registerType<Package>();

  registry.registerType<gates::And>();
  registry.registerType<gates::Nand>();
  registry.registerType<gates::Nor>();
  registry.registerType<gates::Not>();
  registry.registerType<gates::Or>();

  registry.registerType<logic::AssignFloat>();
  registry.registerType<logic::AssignInt>();

  registry.registerType<logic::CounterDown>();
  registry.registerType<logic::CounterUp>();
  registry.registerType<logic::CounterUpDown>();

  registry.registerType<logic::IfGreaterEqual>();
  registry.registerType<logic::IfGreater>();
  registry.registerType<logic::IfEqual>();
  registry.registerType<logic::IfLower>();
  registry.registerType<logic::IfLowerEqual>();

  registry.registerType<logic::Latch>();

  registry.registerType<logic::MemoryDifference>();
  registry.registerType<logic::MemoryResetSet>();
  registry.registerType<logic::MemorySetReset>();

  registry.registerType<logic::MultiplexerInt>();
  registry.registerType<logic::DemultiplexerInt>();

  registry.registerType<logic::Blinker>();
  registry.registerType<logic::Switch>();

  registry.registerType<logic::TriggerFalling>();
  registry.registerType<logic::TriggerRising>();

  registry.registerType<logic::PID>();

  registry.registerType<logic::SnapshotFloat>();
  registry.registerType<logic::SnapshotInt>();

  registry.registerType<math::Abs>();
  registry.registerType<math::BCD>();
  registry.registerType<math::SQRT>();

  registry.registerType<math::Add>();
  registry.registerType<math::AddIf>();
  registry.registerType<math::Subtract>();
  registry.registerType<math::SubtractIf>();
  registry.registerType<math::Divide>();
  registry.registerType<math::DivideIf>();
  registry.registerType<math::Multiply>();
  registry.registerType<math::MultiplyIf>();

  registry.registerType<math::Cos>();
  registry.registerType<math::Sin>();

  registry.registerType<math::Lerp>();
  registry.registerType<math::Sign>();

  registry.registerType<pneumatic::Tank>();
  registry.registerType<pneumatic::Valve>();

  registry.registerType<timers::DeltaTime>();
  registry.registerType<timers::Clock>();
  registry.registerType<timers::TimerOn>();
  registry.registerType<timers::TimerOff>();
  registry.registerType<timers::TimerPulse>();

  registry.registerType<ui::BCDToSevenSegmentDisplay>();

  registry.registerType<ui::FloatInfo>();
  registry.registerType<ui::IntInfo>();

  registry.registerType<ui::PushButton>();
  registry.registerType<ui::ToggleButton>();

  registry.registerType<ui::SevenSegmentDisplay>();

  registry.registerType<values::ConstBool>();
  registry.registerType<values::ConstFloat>();
  registry.registerType<values::ConstInt>();
  registry.registerType<values::RandomBool>();
  registry.registerType<values::RandomFloat>();
  registry.registerType<values::RandomFloatIf>();
  registry.registerType<values::RandomInt>();
  registry.registerType<values::RandomIntIf>();

  registry.registerType<values::Degree2Radian>();
  registry.registerType<values::Radian2Degree>();
  registry.registerType<values::Int2Float>();
  registry.registerType<values::Float2Int>();

  registry.registerType<values::MinInt>();
  registry.registerType<values::MaxInt>();
  registry.registerType<values::MinFloat>();
  registry.registerType<values::MaxFloat>();

  registry.registerType<values::ClampFloat>();
  registry.registerType<values::ClampInt>();

  registry.registerType<values::CharacteristicCurve>();
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

void load_packages()
{
  Packages packages{};

  auto loadFrom = [&packages](fs::path const &a_path) {
    log::warn("Loading packages from {}", a_path.string());
    auto directories = scan_for_dirs(a_path);
    directories.push_back(a_path);
    std::sort(std::begin(directories), std::end(directories));
    for (auto const &DIRECTORY : directories) {
      for (auto const &ENTRY : fs::directory_iterator(DIRECTORY)) {
        if (fs::is_directory(ENTRY)) continue;
        auto const FILENAME = ENTRY.path().string();
        log::warn("Loading package '{}'", FILENAME);
        packages[FILENAME] = Package::getInfoFor(FILENAME);
      }
    }
  };

  loadFrom(system_packages_path());
  loadFrom(user_packages_path());

  log::warn("Loaded {} packages", packages.size());
  for (auto const &PACKAGE : packages) log::warn("{} as '{}'", PACKAGE.first, PACKAGE.second.path);
}

void initialize()
{
  log::init();

  log::info("Spaghetti version: {}, git: {}@{}, build date: {}, {}", version::STRING, version::BRANCH,
            version::COMMIT_SHORT_HASH, __DATE__, __TIME__);

  register_internal_elements();

  auto &registry = Registry::get();

#if _GLIBCXX_USE_CXX11_ABI == 0
  registry.loadPlugins("old_abi");
#else
  registry.loadPlugins();
#endif
}

} // namespace spaghetti
