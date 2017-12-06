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

#include "shared_library.h"

#include <dlfcn.h>
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

  if (a_file.extension() != ".so") {
    log::error("{} is not shared library!", m_filename);
    a_errorCode = std::error_code(EINVAL, std::system_category());
    return;
  }

  m_handle = dlopen(a_file.c_str(), RTLD_NOW);
  if (m_handle == nullptr) {
    log::error("{}", dlerror());
    a_errorCode = std::error_code(EINVAL, std::system_category());
    return;
  }

  a_errorCode = std::error_code(0, std::system_category());
}

SharedLibrary::~SharedLibrary()
{
  log::info("[shared_library]: Closing {}", m_filename);
  if (m_handle) dlclose(m_handle);
}

bool SharedLibrary::has(std::string_view a_signature) const
{
  log::info("[shared_library]: Checking if symbol {} exists in {}", a_signature.data(), m_filename);

  if (m_handle == nullptr) return false;

  return dlsym(m_handle, a_signature.data()) != nullptr;
}

void *SharedLibrary::getSymbol(std::string_view a_signature)
{
  log::info("[shared_library]: Getting symbol {} from {}", a_signature.data(), m_filename);

  return dlsym(m_handle, a_signature.data());
}

} // namespace spaghetti
