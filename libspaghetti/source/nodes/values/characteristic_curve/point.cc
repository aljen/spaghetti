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

#include "nodes/values/characteristic_curve/point.h"

#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <cmath>

#include "nodes/values/characteristic_curve/editor_widget.h"
#include "nodes/values/characteristic_curve/editor_window.h"

using namespace QtCharts;

namespace spaghetti::nodes::values::characteristic_curve {

Point::Point(EditorWidget *const a_editor, QChart *const a_chart)
  : QGraphicsItem{ a_chart }
  , m_editor{ a_editor }
  , m_chart{ a_chart }
  , m_series{ (a_editor ? a_editor->series() : nullptr) }
{
  setFlag(ItemIsMovable, true);
  setFlag(ItemSendsScenePositionChanges, true);
  setAcceptHoverEvents(true);
  setZValue(100.0);
}

Point::Point(QChart *const a_chart)
  : Point{ nullptr, a_chart }
{
}

Point::Point(EditorWidget *const a_editor)
  : Point{ a_editor, a_editor->chart() }
{
}

void Point::paint(QPainter *a_painter, QStyleOptionGraphicsItem const *a_option, QWidget *a_widget)
{
  (void)a_option;
  (void)a_widget;

  QBrush brush{ m_color };
  a_painter->setPen(Qt::NoPen);
  a_painter->setBrush(brush);
  a_painter->drawEllipse({ 0.0, 0.0 }, POINT_RADIUS, POINT_RADIUS);
}

QVariant Point::itemChange(GraphicsItemChange a_change, QVariant const &a_value)
{
  if (m_index >= 0) {
    switch (a_change) {
      case ItemPositionChange: {
        bool const FIRST_POINT = m_index == 0;
        bool const LAST_POINT = m_index == m_series->count() - 1;

        auto const axisX = static_cast<QValueAxis *const>(m_chart->axisX(m_series));
        auto const axisY = static_cast<QValueAxis *const>(m_chart->axisY(m_series));

        qreal minX{};
        qreal maxX{};
        auto const minY = axisY->min();
        auto const maxY = axisY->max();

        if (FIRST_POINT) {
          minX = axisX->min();
          maxX = axisX->min();
        } else if (LAST_POINT) {
          minX = axisX->max();
          maxX = axisX->max();
        } else {
          auto const prev = m_series->at(m_index - 1);
          auto const next = m_series->at(m_index + 1);
          minX = prev.x();
          maxX = next.x();
        }

        auto newPointOnChart = m_chart->mapToValue(a_value.toPointF());

        auto const editorWindow = m_editor->editorWindow();
        bool const X_IS_INT = editorWindow->xValueType() == EditorWindow::ValueType::eInt;
        bool const Y_IS_INT = editorWindow->yValueType() == EditorWindow::ValueType::eInt;

        newPointOnChart.rx() = std::clamp(newPointOnChart.x(), minX, maxX);
        if (X_IS_INT) newPointOnChart.rx() = static_cast<int32_t>(newPointOnChart.x());
        newPointOnChart.ry() = std::clamp(newPointOnChart.y(), minY, maxY);
        if (Y_IS_INT) newPointOnChart.ry() = static_cast<int32_t>(newPointOnChart.y());

        auto scenePoint = m_chart->mapToPosition(newPointOnChart);

        return scenePoint;
      }
      case ItemPositionHasChanged: {
        QPointF const newPoint = m_chart->mapToValue(a_value.toPointF());
        m_series->replace(m_index, newPoint);
        auto const editorWindow = m_editor->editorWindow();
        editorWindow->changePoint(m_index, newPoint);
        m_editor->updateCurrentValue();
        break;
      }

      default: break;
    }
  }

  return QGraphicsItem::itemChange(a_change, a_value);
}

void Point::hoverEnterEvent(QGraphicsSceneHoverEvent *a_event)
{
  (void)a_event;

  if (m_editor == nullptr) return;

  bool const FIRST = m_index == 0;
  bool const LAST = m_index == m_series->count() - 1;

  if (m_type == Type::eNormal && !FIRST && !LAST) {
    m_editor->setHoveredItem(this);
    setType(Type::eToRemove);
  }
}

void Point::hoverLeaveEvent(QGraphicsSceneHoverEvent *a_event)
{
  (void)a_event;

  if (m_type == Type::eToRemove) {
    m_editor->setHoveredItem(nullptr);

    setType(Type::eNormal);
  }
}

void Point::setType(Type const a_type)
{
  m_type = a_type;

  switch (m_type) {
    case Type::eNormal: m_color = QColor{ Qt::white }; break;
    case Type::eCurrent:
      m_color = QColor{ Qt::green };
      setFlag(ItemIsMovable, false);
      break;
    case Type::eToAdd: m_color = QColor{ Qt::yellow }; break;
    case Type::eToRemove: m_color = QColor{ Qt::red }; break;
  }

  update();
}

void Point::setIndex(int const a_index)
{
  m_index = a_index;
}

} // namespace spaghetti::nodes::values::characteristic_curve
