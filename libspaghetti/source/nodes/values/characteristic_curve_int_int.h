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
#ifndef NODES_VALUES_SCALE_INT_H
#define NODES_VALUES_SCALE_INT_H

#include "spaghetti/node.h"

namespace QtCharts {
class QChart;
class QLineSeries;
class QValueAxis;
} // namespace QtCharts

namespace spaghetti::nodes::values {

class Point;

class CharacteristicCurveIntInt : public Node {
 public:
  CharacteristicCurveIntInt();

 private:
  void refreshCentralWidget() override;
  void showProperties() override;
  void elementSet() override;

 private:
  void synchronizeFromElement();
  void updateCurrentValue(bool const a_force);

 private:
  QtCharts::QChart *m_widget{};
  QtCharts::QLineSeries *m_series{};
  QtCharts::QValueAxis *m_xAxis{};
  QtCharts::QValueAxis *m_yAxis{};
  QPointF m_lastPoint{};
  Point *m_current{};
};

} // namespace spaghetti::nodes::values

#endif // NODES_VALUES_SCALE_INT_H
