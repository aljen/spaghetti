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

#include "colors.h"
#include <array>

// https://www.materialpalette.com/
// 244,  53,  64, 255 - RED
// 232,   0,  99, 255 - PINK
// 154,  41, 169, 255 - PURPLE
// 101,  63, 176, 255 - DEEP PURPLE
//  62,  84, 174, 255 - INDIGO
//  29, 151, 237, 255 - BLUE
//   0, 170, 238, 255 - LIGHT BLUE
//   0, 188, 208, 255 - CYAN
//   0, 149, 134, 255 - TEAL
//  75, 173,  88, 255 - GREEN
// 137, 193,  86, 255 - LIGHT GREEN
// 203, 217,  81, 255 - LIME
// 254, 231,  86, 255 - YELLOW
// 254, 187,  59, 255 - AMBER
// 254, 144,  50, 255 - ORANGE
// 254,  74,  53, 255 - DEEP ORANGE
// 120,  83,  74, 255 - BROWN
// 156, 156, 156, 255 - GREY
//  95, 124, 136, 255 - BLUE GREY
//  58,  66,  71, 255 - DARK GREY
//  82,  55,  77, 255 - PURPLE TAUPE
// 111,  80,  96, 255 - WENGE

namespace {
std::array<QColor, static_cast<size_t>(Color::eCount)> const g_colors{ {
    { 95, 124, 136, 255 },  // Color::eNameBackground
    { 255, 245, 238, 255 }, // Color::eFontName
    { 169, 169, 169, 255 }, // Color::eFontType
    { 105, 105, 105, 64 },  // Color::eNodeHeader
    { 105, 105, 105, 128 }, // Color::eNodeContent
    { 54, 54, 54, 255 },    // Color::eSocketBorder
    { 255, 99, 71, 255 },   // Color::eSocketInput
    { 135, 206, 235, 255 }, // Color::eSocketOutput
    { 173, 255, 47, 255 },  // Color::eSocketDrop
    { 255, 215, 0, 255 },   // Color::eSocketHover
    { 244, 53, 64, 255 },   // Color::eBoolSignalOff
    { 75, 173, 88, 255 },   // Color::eBoolSignalOn
    { 62, 84, 174, 255 },   // Color::eIntegerSignalOff
    { 0, 170, 238, 255 },   // Color::eIntegerSignalOn
    { 111, 80, 96, 255 },   // Color::eFloatSignalOff
                            //    { 232, 0, 99, 255 },    // Color::eFloatSignalOn
    { 254, 144, 50, 255 },  // Color::eFloatSignalOn
    { 165, 165, 165, 64 },  // Color::eLink
    { 255, 255, 255, 255 }  // Color::eSelected
} };
}

QColor get_color(Color const a_color)
{
  return g_colors[static_cast<size_t>(a_color)];
}
