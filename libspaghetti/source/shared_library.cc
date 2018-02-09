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

#include "shared_library.h"

// clang-format off
#if defined(_WIN64) || defined(_WIN32)
# include <windows.h>
#elif defined(__unix__)
# include <dlfcn.h>
#endif
// clang-format on
#include "spaghetti/logger.h"

namespace spaghetti {

SharedLibrary::SharedLibrary(fs::path a_file, std::error_code &a_errorCode)
  : m_filename{ a_file.string() }
{
  log::info("[shared_library]: Opening {}", m_filename);

  if (!fs::exists(a_file)) {
    log::error("{} don't exist!", m_filename);
    a_errorCode = std::error_code(ENOENT, std::system_category());
    return;
  }

#if defined(_WIN64) || defined(_WIN32)
  if (a_file.extension() != ".dll") {
#else
  if (a_file.extension() != ".so") {
#endif
    log::debug("{} is not shared library!", m_filename);
    a_errorCode = std::error_code(EINVAL, std::system_category());
    return;
  }

#if defined(_WIN64) || defined(_WIN32)
  m_handle = LoadLibrary(a_file.c_str());
  if (m_handle == nullptr) {
    auto const ERROR_CODE = GetLastError();
    LPSTR buffer{};
    size_t const SIZE{ FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, ERROR_CODE,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), reinterpret_cast<LPSTR>(&buffer), 0, nullptr) };
    std::string const MESSAGE(buffer, SIZE);
    LocalFree(buffer);
    log::error("LoadLibrary error: {}", MESSAGE);
    a_errorCode = std::error_code(ERROR_CODE, std::system_category());
  }
#elif defined(__unix__)
  m_handle = dlopen(a_file.c_str(), RTLD_NOW);
  if (m_handle == nullptr) {
    log::error("{}", dlerror());
    a_errorCode = std::error_code(EINVAL, std::system_category());
    return;
  }
#endif

  a_errorCode = std::error_code(0, std::system_category());
}

SharedLibrary::~SharedLibrary()
{
  log::info("[shared_library]: Closing {}", m_filename);
#if defined(_WIN64) || defined(_WIN32)
  if (m_handle) FreeLibrary(static_cast<HMODULE>(m_handle));
#elif defined(__unix__)
  if (m_handle) dlclose(m_handle);
#endif
}

bool SharedLibrary::has(std::string_view a_signature) const
{
  log::info("[shared_library]: Checking if symbol {} exists in {}", a_signature.data(), m_filename);

  if (m_handle == nullptr) return false;

#if defined(_WIN64) || defined(_WIN32)
  return GetProcAddress(static_cast<HMODULE>(m_handle), a_signature.data()) != nullptr;
#elif defined(__unix__)
  return dlsym(m_handle, a_signature.data()) != nullptr;
#endif
}

void *SharedLibrary::getSymbol(std::string_view a_signature)
{
  log::info("[shared_library]: Getting symbol {} from {}", a_signature.data(), m_filename);

#if defined(_WIN64) || defined(_WIN32)
  return reinterpret_cast<void*>(GetProcAddress(static_cast<HMODULE>(m_handle), a_signature.data()));
#elif defined(__unix__)
  return dlsym(m_handle, a_signature.data());
#endif
}

} // namespace spaghetti
