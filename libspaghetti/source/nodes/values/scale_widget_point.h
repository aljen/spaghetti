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
#ifndef NODES_VALUES_SCALE_WIDGET_POINT_H
#define NODES_VALUES_SCALE_WIDGET_POINT_H

#include <QGraphicsItem>

namespace QtCharts {
class QChart;
class QLineSeries;
} // namespace QtCharts

namespace spaghetti::nodes::values {

class ScaleWidgetEditor;

constexpr qreal const POINT_RADIUS = 6.0;

class Point : public QGraphicsItem {
 public:
  enum class Type { eNormal, eCurrent, eToAdd, eToRemove };

  explicit Point(QtCharts::QChart *const a_chart);
  explicit Point(ScaleWidgetEditor *const a_editor);

  QRectF boundingRect() const override { return m_boundingRect; }
  void paint(QPainter *a_painter, QStyleOptionGraphicsItem const *a_option, QWidget *a_widget) override;
  QVariant itemChange(GraphicsItemChange a_change, QVariant const &a_value) override;
  void hoverEnterEvent(QGraphicsSceneHoverEvent *a_event) override;
  void hoverLeaveEvent(QGraphicsSceneHoverEvent *a_event) override;

  void setType(Type const a_type);

  int index() const { return m_index; }
  void setIndex(int const a_index);

 private:
  explicit Point(ScaleWidgetEditor *const a_editor, QtCharts::QChart *const a_chart);

 private:
  QColor m_color{};
  ScaleWidgetEditor *const m_editor{};
  QtCharts::QChart *const m_chart{};
  QtCharts::QLineSeries *const m_series{};
  QRectF m_boundingRect{ -POINT_RADIUS, -POINT_RADIUS, POINT_RADIUS * 2.0, POINT_RADIUS * 2.0 };
  Type m_type{};
  int m_index{ -1 };
};

} // namespace spaghetti::nodes::values

#endif // NODES_VALUES_SCALE_WIDGET_POINT_H
