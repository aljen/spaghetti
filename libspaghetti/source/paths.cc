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

#include "spaghetti/paths.h"

#include <cassert>
#include <cstring>
#include <string>

// clang-format off
#if defined(_WIN64) || defined(_WIN32)
# define WIN32_LEAN_AND_MEAN
# include <windows.h>
# include <shlobj.h>
# include <sstream>
#elif defined(__linux__)
# include <unistd.h>
#endif
// clang-format on

#include "spaghetti/filesystem.h"

namespace spaghetti {

std::string_view app_path()
{
  static std::string const APP_PATH = [] {
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
  }();

  return APP_PATH;
}

std::string_view app_data_path()
{
  static std::string const APP_DATA_PATH = [] {
#if defined(_WIN64) || defined(_WIN32)
  wchar_t *appDataPath{};
  HRESULT result = SHGetKnownFolderPath(FOLDERID_LocalAppData, KF_FLAG_DEFAULT, nullptr, &appDataPath);
  std::wstringstream ss{};
  ss << appDataPath;
  CoTaskMemFree(static_cast<void *>(appDataPath));
  fs::path const TEMP_APP_DATA_PATH{ ss.str() };
  return fs::absolute(TEMP_APP_DATA_PATH / "Spaghetti").string();
#else
  auto const HOME_ENV = getenv("HOME");
  assert(HOME_ENV);
  fs::path const DATA_PATH = fs::absolute(fs::path{ HOME_ENV } / ".config/spaghetti");
  fs::create_directories(DATA_PATH);
  return DATA_PATH.string();
#endif
  }();

  return APP_DATA_PATH;
}

} // namespace spaghetti
