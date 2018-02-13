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
#ifndef NODES_VALUES_CHARACTERISTIC_CURVE_SERIES_H
#define NODES_VALUES_CHARACTERISTIC_CURVE_SERIES_H

#include <QList>

namespace QtCharts {
class QLineSeries;
}

namespace spaghetti::nodes::values::characteristic_curve {

class Axis;
class Point;

class Series {
 public:
  enum class GeneratorType {
    eQuadraticEasyIn,
    eQuadraticEasyOut,
    eQuadraticEasyInOut,
    eCubicEasyIn,
    eCubicEasyOut,
    eCubicEasyInOut,
    eQuarticEasyIn,
    eQuarticEasyOut,
    eQuarticEasyInOut,
    eQuinticEasyIn,
    eQuinticEasyOut,
    eQuinticEasyInOut,
    eSinusoidalEasyIn,
    eSinusoidalEasyOut,
    eSinusoidalEasyInOut,
    eExponentialEasyIn,
    eExponentialEasyOut,
    eExponentialEasyInOut,
    eCircularEasyIn,
    eCircularEasyOut,
    eCircularEasyInOut,
    eRandom,
    eCount
  };

  Series();
  ~Series();

  void setName(QString const a_name);

  void setInput(qreal const a_input);
  void setRange(qreal const a_min, qreal const a_max);

  qreal output() const { return m_output; }

  void generate(GeneratorType const a_type, int const a_steps, qreal const a_start, qreal const a_stop);

  void attachAxis(Axis *const a_axis);
  void detachAxis();

  QtCharts::QLineSeries *series() const { return m_series; }

 private:
  Axis *m_axis{};
  QtCharts::QLineSeries *const m_series{};
  QList<Point *> m_points{};
  Point *const m_currentValue{};
  qreal m_input{};
  qreal m_output{};

  qreal m_min{};
  qreal m_max{};
};

} // namespace spaghetti::nodes::values::characteristic_curve

#endif // NODES_VALUES_CHARACTERISTIC_CURVE_SERIES_H
