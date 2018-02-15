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

class EditorWindow;
class Point;
class Line;
class Series;
class Axis;

class EditorWidget : public QtCharts::QChartView {
 public:
  explicit EditorWidget(QWidget *const a_parent = nullptr);

  void setXName(QString const &a_name);
  QString xName() const;

  void setXMajorTicks(int const a_ticks);
  int xMajorTicks() const;

  void setXMinorTicks(int const a_ticks);
  int xMinorTicks() const;

  void setXMinimum(qreal const a_min);
  qreal xMinimum() const;

  void setXMaximum(qreal const a_max);
  qreal xMaximum() const;

  void setYName(QString const &a_name);
  QString yName() const;

  void setYMajorTicks(int const a_ticks);
  int yMajorTicks() const;

  void setYMinorTicks(int const a_ticks);
  int yMinorTicks() const;

  void setYMinimum(qreal const a_min);
  qreal yMinimum() const;

  void setYMaximum(qreal const a_max);
  qreal yMaximum() const;

  void setX(qreal const a_value);

  //  void createPoints();

  void setHoveredItem(Point *const a_item);
  void updateCurrentValue();

  QtCharts::QLineSeries const *series() const { return m_series; }
  QtCharts::QLineSeries *series() { return m_series; }

  void updatePoints();

  void setEditorWindow(EditorWindow *const a_window);
  EditorWindow *editorWindow() const { return m_window; }

  void changePoint(int const a_index, QPointF const a_point);

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
  EditorWindow *m_window{};
  QtCharts::QValueAxis *m_xAxis{};
  QtCharts::QValueAxis *m_yAxis{};
  QtCharts::QLineSeries *m_series{};
  QList<Point *> m_points{};
  Line *const m_line{};
  Point *m_currentValue{};
  Point *m_pointToAdd{};
  Point *m_pointToRemove{};
  QPointF m_lastMousePosition{};
  qreal m_x{};
  qreal m_y{};
};

} // namespace spaghetti::nodes::values::characteristic_curve

#endif // NODES_VALUES_CHARACTERISTIC_CURVE_EDITOR_WIDGET_H
