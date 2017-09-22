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

#ifndef TYPES_H
#define TYPES_H

// clang-format off

namespace Types {

enum {
  ePackage            = 0,
  eClock              = 1,
  eNand               = 2,
  eAnd                = 3,
  eNor                = 4,
  eOr                 = 5,
  eNot                = 6,

  eConstBool          = 7,
  eConstInt           = 8,
  eConstFloat         = 9,

  eRandomBool         = 10,

  eSwitchBool         = 11,
  eSwitchInt          = 12,
  eSwitchFloat        = 13,

#if 0
  eMultiplexerBool    = 13,
  eMultiplexerInt     = 14,
  eMultiplexerFloat   = 15,

  eMaxInt             = 16,
  eMaxFloat           = 17,
  eMinInt             = 18,
  eMinFloat           = 19,

  eClampInt           = 19,
  eClampFloat         = 20,

  eMapRangeInt        = 21,
  eMapRangeFloat      = 22,

  eConvertIntFloat    = 23,
  eConvertFloatInt    = 24,

  eGreaterEqualInt    = 25,
  eGreaterEqualFloat  = 26,
  eGreaterInt         = 27,
  eGreaterFloat       = 28,
  eEqualInt           = 29,
  eEqualFloat         = 30,
  eLowerInt           = 31,
  eLowerFloat         = 32,
  eLowerEqualInt      = 33,
  eLowerEqualFloat    = 34,

  eScaleInt           = 35,
  eScaleFloat         = 36,
#endif

  eUser               = 1000
};

}

// clang-format on

#endif // TYPES_H
