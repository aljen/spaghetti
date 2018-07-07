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
#ifndef SPAGHETTI_SHARED_LIBRARY_H
#define SPAGHETTI_SHARED_LIBRARY_H

// clang-format off
#ifdef _MSC_VER
# pragma warning(disable:4251)
#endif
// clang-format on

#include <system_error>
#include <spaghetti/api.h>
#include <spaghetti/filesystem.h>

namespace spaghetti {

class SPAGHETTI_API SharedLibrary final {
 public:
  SharedLibrary(fs::path const &a_file, std::error_code &a_errorCode);
  ~SharedLibrary();

  bool has(std::string_view a_signature) const;

  template<typename T>
  T &get(std::string_view a_signature)
  {
    return *reinterpret_cast<T *>(getSymbol(a_signature));
  }

 private:
  void *getSymbol(std::string_view a_signature);

 private:
  std::string m_filename{};
  void *m_handle{};
};

} // namespace spaghetti

#endif // SPAGHETTI_SHARED_LIBRARY_H
