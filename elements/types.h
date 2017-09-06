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
  ePackage      = 0,
  eNand         = 1,
  eAnd          = 2,
  eOr           = 3,
  eNot          = 4,

  eConstBool    = 5,
  eConstInt     = 6,
  eConstFloat   = 7,

  eSwitchBool   = 8,
  eSwitchInt    = 9,
  eSwitchFloat  = 10,

  eMultiplexerBool  = 11,
  eMultiplexerInt   = 12,
  eMultiplexerFloat = 13,

  eMaxInt           = 14,
  eMaxFloat         = 15,
  eMinInt           = 16,
  eMinFloat         = 17,

  eClampInt         = 18,
  eClampFloat       = 19,

  eMapRangeInt      = 20,
  eMapRangeFloat    = 21,

  eConvertIntFloat  = 22,
  eConvertFloatInt  = 23,

  eGreaterEqualInt    = 24,
  eGreaterEqualFloat  = 25,
  eGreaterInt         = 26,
  eGreaterFloat       = 27,
  eEqualInt           = 28,
  eEqualFloat         = 29,
  eLowerInt           = 30,
  eLowerFloat         = 31,
  eLowerEqualInt      = 32,
  eLowerEqualFloat    = 33,

  eScaleInt           = 34,
  eScaleFloat         = 35,

  eUser         = 1000
};

}

#endif // TYPES_H
