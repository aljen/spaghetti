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

#ifndef CORE_ID_MANAGER_H
#define CORE_ID_MANAGER_H

#include <cassert>
#include <cstdint>
#include <vector>

namespace core {

template<typename Data, typename Id>
class IdManager {
 public:
  template<typename Type, typename... Args>
  Id acquire(Args &&... args) noexcept
  {
    uint32_t index{};

    if (m_free.empty()) {
      index = static_cast<uint32_t>(m_data.size());
      m_data.emplace_back(new Type{ std::forward<Args>(args)... });
    } else {
      index = m_free.back();
      assert(m_data[index] == nullptr);
      m_data[index] = new Type{ std::forward<Args>(args)... };
      m_free.pop_back();
    }

    return Id{ index };
  }

  void release(Id &a_id) noexcept
  {
    uint32_t const index{ a_id };
    assert(index < m_data.size());

    delete m_data[a_id];
    m_data[a_id] = nullptr;
    m_free.emplace_back(index);
    a_id.invalidate();
  }

  void reserve(size_t a_size) noexcept
  {
    m_data.reserve(a_size);
    m_free.reserve(a_size);
  }

  Data get(Id const &a_id) noexcept
  {
    uint32_t const index{ static_cast<uint32_t>(a_id) };
    assert(index < m_data.size());
    return m_data[index];
  }

  Data get(Id const &a_id) const noexcept
  {
    uint32_t const index{ static_cast<uint32_t>(a_id) };
    assert(index < m_data.size());
    return m_data[index];
  }

  uint32_t used() const noexcept { return m_data.size() - m_free.size(); }

 private:
  std::vector<Data> m_data{};
  std::vector<uint32_t> m_free{};
};

} // namespace core

#endif // CORE_ID_MANAGER_H
