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

#define _USE_MATH_DEFINES
#include "nodes/values/characteristic_curve/generators.h"

#include <cmath>
#include <random>

namespace spaghetti::nodes::values::characteristic_curve {

std::random_device g_randomDevice{};
std::mt19937 g_randomEngine{ g_randomDevice() };

double gen_quadratic_easy_in(double position, double start, double end)
{
  return (end - start) * position * position + start;
}

double gen_quadratic_easy_out(double position, double start, double end)
{
  return (-(end - start)) * position * (position - 2.0) + start;
}

double gen_quadratic_easy_in_out(double position, double start, double end)
{
  position *= 2.0;
  if (position < 1.0) return ((end - start) / 2.0) * position * position + start;

  --position;
  return (-(end - start) / 2.0) * (position * (position - 2.0) - 1.0) + start;
}

double gen_cubic_easy_in(double position, double start, double end)
{
  return (end - start) * position * position * position + start;
}

double gen_cubic_easy_out(double position, double start, double end)
{
  --position;
  return (end - start) * (position * position * position + 1.0) + start;
}

double gen_cubic_easy_in_out(double position, double start, double end)
{
  position *= 2.0;
  if (position < 1.0) return ((end - start) / 2.0) * position * position * position + start;

  position -= 2.0;
  return ((end - start) / 2.0) * (position * position * position + 2.0) + start;
}

double gen_quartic_easy_in(double position, double start, double end)
{
  return (end - start) * position * position * position * position + start;
}

double gen_quartic_easy_out(double position, double start, double end)
{
  --position;
  return -(end - start) * (position * position * position * position - 1.0) + start;
}

double gen_quartic_easy_in_out(double position, double start, double end)
{
  position *= 2.0;
  if (position < 1.0) return ((end - start) / 2.0) * (position * position * position * position) + start;

  position -= 2.0;
  return (-(end - start) / 2.0) * (position * position * position * position - 2.0) + start;
}

double gen_quintic_easy_in(double position, double start, double end)
{
  return (end - start) * position * position * position * position * position + start;
}

double gen_quintic_easy_out(double position, double start, double end)
{
  --position;
  return (end - start) * (position * position * position * position * position + 1.0) + start;
}

double gen_quintic_easy_in_out(double position, double start, double end)
{
  position *= 2.0;
  if (position < 1.0) return ((end - start) / 2.0) * (position * position * position * position * position) + start;

  position -= 2.0;
  return ((end - start) / 2.0) * (position * position * position * position * position + 2.0) + start;
}

double gen_sinusoidal_easy_in(double position, double start, double end)
{
  return -(end - start) * std::cos(position * M_PI / 2.0) + (end - start) + start;
}

double gen_sinusoidal_easy_out(double position, double start, double end)
{
  return (end - start) * std::sin(position * M_PI / 2.0) + start;
}

double gen_sinusoidal_easy_in_out(double position, double start, double end)
{
  return (-(end - start) / 2.0) * (std::cos(position * M_PI) - 1.0) + start;
}

double gen_exponential_easy_in(double position, double start, double end)
{
  return (end - start) * std::pow(2.0, 10.0 * (position - 1.0)) + start;
}

double gen_exponential_easy_out(double position, double start, double end)
{
  return (end - start) * (-std::pow(2.0, -10.0 * position) + 1.0) + start;
}

double gen_exponential_easy_in_out(double position, double start, double end)
{
  position *= 2.0;
  if (position < 1.0) return ((end - start) / 2.0) * std::pow(2.0, 10.0 * (position - 1.0)) + start;

  --position;
  return ((end - start) / 2.0) * (-std::pow(2.0, -10.0 * position) + 2.0) + start;
}

double gen_circular_easy_in(double position, double start, double end)
{
  return -(end - start) * (std::sqrt(1.0 - position * position) - 1.0) + start;
}

double gen_circular_easy_out(double position, double start, double end)
{
  --position;
  return (end - start) * (std::sqrt(1.0 - position * position)) + start;
}

double gen_circular_easy_in_out(double position, double start, double end)
{
  position *= 2.0;
  if (position < 1.0) return (-(end - start) / 2.0) * (std::sqrt(1.0 - position * position) - 1.0) + start;

  position -= 2.0;
  return ((end - start) / 2.0) * (std::sqrt(1.0 - position * position) + 1.0) + start;
}

double gen_random(double position, double start, double end)
{
  (void)position;
  std::uniform_real_distribution<double> dist(start, end);
  return dist(g_randomEngine);
}

} // namespace spaghetti::nodes::values::characteristic_curve
