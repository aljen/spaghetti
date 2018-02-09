#include "nodes/values/characteristic_curve/point.h"

#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>

#include "nodes/values/scale_widget_editor.h"

using namespace QtCharts;

namespace spaghetti::nodes::values {

Point::Point(ScaleWidgetEditor *const a_editor, QChart *const a_chart)
  : QGraphicsItem{ a_chart }
  , m_editor{ a_editor }
  , m_chart{ a_chart }
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

Point::Point(ScaleWidgetEditor *const a_editor)
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

        newPointOnChart.rx() = std::clamp(newPointOnChart.x(), minX, maxX);
        newPointOnChart.ry() = std::clamp(newPointOnChart.y(), minY, maxY);

        auto scenePoint = m_chart->mapToPosition(newPointOnChart);

        return scenePoint;
      }
      case ItemPositionHasChanged: {
        QPointF const newPoint = m_chart->mapToValue(a_value.toPointF());
        m_series->replace(m_index, newPoint);
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
  bool const last = m_index == m_series->count() - 1;

  if (m_type == Type::eNormal && !FIRST && !last) {
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

void Point::setType(const Point::Type a_type)
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

} // namespace spaghetti::nodes::values
