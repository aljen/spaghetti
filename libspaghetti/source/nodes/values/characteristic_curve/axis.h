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
#ifndef NODES_VALUES_CHARACTERISTIC_CURVE_AXIS_H
#define NODES_VALUES_CHARACTERISTIC_CURVE_AXIS_H

#include <QList>
#include <QString>
#include <QtGlobal>

namespace QtCharts {
class QValueAxis;
}

namespace spaghetti::nodes::values::characteristic_curve {

class Series;

class Axis {
 public:
  enum class Type { eInt, eFloat };

  Axis();
  ~Axis();

  QtCharts::QValueAxis *axis() const { return m_axis; }

  void setName(QString a_name);
  QString name() const { return m_name; }

  void setType(Type const a_type);
  Type type() const { return m_type; }

  void setMajorTicks(int const a_majorTicks);
  int majorTicks() const { return m_majorTicks; }

  void setMinorTicks(int const a_minorTicks);
  int minorTicks() const { return m_minorTicks; }

  void setMinimum(qreal const a_min);
  qreal minimum() const { return m_min; }

  void setMaximum(qreal const a_max);
  qreal maximum() const { return m_max; }

  void attachSeries(Series *const a_series);
  void detachSeries(Series *const a_series);

 private:
  QList<Series *> m_series{};
  QtCharts::QValueAxis *m_axis{};
  QString m_name{};
  Type m_type{};
  int m_majorTicks{};
  int m_minorTicks{};
  qreal m_min{};
  qreal m_max{};
};

} // namespace spaghetti::nodes::values::characteristic_curve

#endif // NODES_VALUES_CHARACTERISTIC_CURVE_AXIS_H
