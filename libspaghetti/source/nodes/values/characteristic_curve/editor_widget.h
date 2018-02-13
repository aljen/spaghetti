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
#ifndef NODES_VALUES_CHARACTERISTIC_CURVE_EDITOR_WIDGET_H
#define NODES_VALUES_CHARACTERISTIC_CURVE_EDITOR_WIDGET_H

#include <QtCharts/QChartView>

namespace QtCharts {
class QLineSeries;
class QValueAxis;
} // namespace QtCharts

namespace spaghetti::nodes::values::characteristic_curve {

class Point;
class Line;
class Series;
class Axis;

class EditorWidget : public QtCharts::QChartView {
 public:
  explicit EditorWidget(QWidget *const a_parent = nullptr);

  void setInputName(QString const &a_name);
  QString inputName() const;

  void setInputMajorTicks(int const a_ticks);
  int inputMajorTicks() const;

  void setInputMinorTicks(int const a_ticks);
  int inputMinorTicks() const;

  void setInputMinimum(qreal const a_min);
  qreal inputMinimum() const;

  void setInputMaximum(qreal const a_max);
  qreal inputMaximum() const;

  void addAxis(Axis *const a_axis);
  void removeAxis(int const a_index);
  QList<Axis *> axes() const { return m_axes; }

  void addSeries(Series *const a_series);
  void removeSeries(int const a_index);
  QList<Series *> series() const { return m_series; }

  void setInput(qreal const a_value);

  //  void createPoints();

  void setHoveredItem(Point *const a_item);
  void updateCurrentValue();

 protected:
  void keyPressEvent(QKeyEvent *a_event) override;
  void keyReleaseEvent(QKeyEvent *a_event) override;
  void mousePressEvent(QMouseEvent *a_event) override;
  void mouseMoveEvent(QMouseEvent *a_event) override;
  void resizeEvent(QResizeEvent *a_event) override;

 private:
  void addPoint(int const a_index, QPointF const a_point);
  void removePoint(int const a_index);
  void createPointToAdd();

 private:
  QtCharts::QValueAxis *m_xAxis{};
  QList<Axis *> m_axes{};
  QList<Series *> m_series{};
  Line *const m_line{};
  Point *m_pointToAdd{};
  Point *m_pointToRemove{};
  QPointF m_lastMousePosition{};
  qreal m_input{};
};

} // namespace spaghetti::nodes::values::characteristic_curve

#endif // NODES_VALUES_CHARACTERISTIC_CURVE_EDITOR_WIDGET_H
