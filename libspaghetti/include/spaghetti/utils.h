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

#pragma once
#ifndef SPAGHETTI_UTILS_H
#define SPAGHETTI_UTILS_H

#include <cmath>
#include <limits>

namespace spaghetti {

constexpr float const PI = 3.1415926535897932f;
constexpr float const RAD2DEG = 180.0f / PI;
constexpr float const DEG2RAD = PI / 180.0f;

inline bool nearly_equal(float const &a_a, float const &a_b)
{
  return std::nextafter(a_a, std::numeric_limits<float>::lowest()) <= a_b &&
         std::nextafter(a_a, std::numeric_limits<float>::max()) >= a_b;
}

} // namespace spaghetti

#endif // SPAGHETTI_UTILS_H
