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

namespace Types {

enum {
  ePackage            = 0,
  eNand               = 1,
  eAnd                = 2,
  eNor                = 3,
  eOr                 = 4,
  eNot                = 5,

  eConstBool          = 6,
  eConstInt           = 7,
  eConstFloat         = 8,

  eSwitchBool         = 9,
  eSwitchInt          = 10,
  eSwitchFloat        = 11,

  eMultiplexerBool    = 12,
  eMultiplexerInt     = 13,
  eMultiplexerFloat   = 14,

  eMaxInt             = 15,
  eMaxFloat           = 16,
  eMinInt             = 17,
  eMinFloat           = 18,

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

  eUser               = 1000
};

}

#endif // TYPES_H
