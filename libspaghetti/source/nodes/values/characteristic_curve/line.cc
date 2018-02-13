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

#include "nodes/values/characteristic_curve/line.h"
#include "nodes/values/characteristic_curve/editor_widget.h"

namespace spaghetti::nodes::values::characteristic_curve {

Line::Line(QColor const a_color, QGraphicsItem *const a_parent)
  : QGraphicsItem{ a_parent }
  , m_color{ a_color }
  , m_length{ 150.0 }
{
  setZValue(200.0);
  m_boundingRect = QRectF{ 0.0, 0.0, 10.0, m_length };
}

void Line::paint(QPainter *a_painter, const QStyleOptionGraphicsItem *a_option, QWidget *a_widget)
{
  (void)a_option;
  (void)a_widget;

  QPen pen{ m_color };
  a_painter->setPen(pen);
  a_painter->drawLine(QPointF{ 0.0, 0.0 }, QPointF{ 0.0, m_length });
}

void Line::setLength(qreal a_length)
{
  prepareGeometryChange();
  m_length = a_length;
  m_boundingRect = QRectF{ 0.0, 0.0, 10.0, m_length };
}

} // namespace spaghetti::nodes::values::characteristic_curve
