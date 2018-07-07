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
#ifndef SPAGHETTI_STRINGS_H
#define SPAGHETTI_STRINGS_H

#include <cstdint>

#define HASH_SIZE_32 32
#define HASH_SIZE_64 64
#define HASH_SIZE HASH_SIZE_32

namespace spaghetti { namespace string {

constexpr size_t length(char const *const a_string)
{
  return *a_string ? 1 + length(a_string + 1) : 0;
}

// fnv1a hash

#if HASH_SIZE == HASH_SIZE_32
using hash_t = uint32_t;
constexpr hash_t FNV_OFFSET{ 0x811c9dc5u };
constexpr hash_t FNV_PRIME{ 0x01000193u };
#elif HASH_SIZE == HASH_SIZE_64
using hash_t = uint64_t;
constexpr hash_t FNV_OFFSET{ 0xcbf29ce484222325 };
constexpr hash_t FNV_PRIME{ 0x100000001b3 };
#endif

constexpr hash_t hash(char const *const a_key, hash_t const &a_offset = FNV_OFFSET, hash_t const &a_prime = FNV_PRIME)
{
  hash_t value{ a_offset };
  size_t const size{ length(a_key) };
  for (size_t i = 0; i < size; ++i) {
    value ^= static_cast<hash_t>(a_key[i]);
    value *= a_prime;
  }

  return value;
}

}} // namespace spaghetti::string

#endif // SPAGHETTI_STRINGS_H
