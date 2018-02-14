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

#include "nodes/values/characteristic_curve/editor_widget.h"

#include <QDebug>
#include <QGuiApplication>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>

#include "nodes/values/characteristic_curve/line.h"
#include "nodes/values/characteristic_curve/point.h"
#include "spaghetti/utils.h"

using namespace QtCharts;

namespace spaghetti::nodes::values::characteristic_curve {

EditorWidget::EditorWidget(QWidget *const a_parent)
  : QChartView{ a_parent }
  , m_xAxis{ new QValueAxis }
  , m_yAxis{ new QValueAxis }
  , m_series{ new QtCharts::QLineSeries }
  , m_line{ new Line{ QColor{ Qt::yellow }, this->chart() } }
  , m_currentValue{ new Point{ this } }
{
  setRenderHint(QPainter::Antialiasing);
  setViewportUpdateMode(QChartView::FullViewportUpdate);

  m_xAxis->setMin(0.0);
  m_xAxis->setMax(1.0);
  m_xAxis->setTitleText("X");

  m_yAxis->setMin(0.0);
  m_yAxis->setMax(1.0);
  m_yAxis->setTitleText("Y");

  chart()->addAxis(m_xAxis, Qt::AlignBottom);
  chart()->addAxis(m_yAxis, Qt::AlignLeft);
  chart()->setTheme(QChart::ChartThemeDark);
  chart()->addSeries(m_series);
  m_series->attachAxis(m_xAxis);
  m_series->attachAxis(m_yAxis);

  chart()->legend()->hide();

  m_currentValue->setType(Point::Type::eCurrent);
}

void EditorWidget::setXName(const QString &a_name)
{
  m_xAxis->setTitleText(a_name);
}

QString EditorWidget::xName() const
{
  return m_xAxis->titleText();
}

void EditorWidget::setXMajorTicks(const int a_ticks)
{
  m_xAxis->setTickCount(a_ticks);
}

int EditorWidget::xMajorTicks() const
{
  return m_xAxis->tickCount();
}

void EditorWidget::setXMinorTicks(const int a_ticks)
{
  m_xAxis->setMinorTickCount(a_ticks);
}

int EditorWidget::xMinorTicks() const
{
  return m_xAxis->minorTickCount();
}

void EditorWidget::setXMinimum(const qreal a_min)
{
  m_xAxis->setMin(a_min);
}

qreal EditorWidget::xMinimum() const
{
  return m_xAxis->min();
}

void EditorWidget::setXMaximum(const qreal a_max)
{
  m_xAxis->setMax(a_max);
}

qreal EditorWidget::xMaximum() const
{
  return m_xAxis->max();
}

void EditorWidget::setYName(const QString &a_name)
{
  m_yAxis->setTitleText(a_name);
}

QString EditorWidget::yName() const
{
  return m_yAxis->titleText();
}

void EditorWidget::setYMajorTicks(const int a_ticks)
{
  m_yAxis->setTickCount(a_ticks);
}

int EditorWidget::yMajorTicks() const
{
  return m_yAxis->tickCount();
}

void EditorWidget::setYMinorTicks(const int a_ticks)
{
  m_yAxis->setMinorTickCount(a_ticks);
}

int EditorWidget::yMinorTicks() const
{
  return m_yAxis->minorTickCount();
}

void EditorWidget::setYMinimum(const qreal a_min)
{
  m_yAxis->setMin(a_min);
}

qreal EditorWidget::yMinimum() const
{
  return m_yAxis->min();
}

void EditorWidget::setYMaximum(const qreal a_max)
{
  m_yAxis->setMax(a_max);
}

qreal EditorWidget::yMaximum() const
{
  return m_xAxis->max();
}

void EditorWidget::keyPressEvent(QKeyEvent *a_event)
{
  QChartView::keyPressEvent(a_event);

  auto const MODIFIERS = a_event->modifiers();
  if (MODIFIERS & Qt::ControlModifier) createPointToAdd();
}

void EditorWidget::keyReleaseEvent(QKeyEvent *a_event)
{
  QChartView::keyReleaseEvent(a_event);

  auto const MODIFIERS = a_event->modifiers();
  if ((MODIFIERS & Qt::ControlModifier) == 0 && m_pointToAdd) {
    delete m_pointToAdd;
    m_pointToAdd = nullptr;
  }
}

void EditorWidget::mousePressEvent(QMouseEvent *a_event)
{
  QChartView::mousePressEvent(a_event);

  if (m_pointToAdd) {
    int index{};
    int const SIZE = m_series->count();
    for (int i = 0; i < SIZE; ++i) {
      QPointF point = m_series->at(i);
      if (m_lastMousePosition.x() >= point.x())
        index = i;
      else
        break;
    }

    addPoint(index + 1, m_lastMousePosition);

    createPointToAdd();
  }

  auto const MODIFIERS = QGuiApplication::keyboardModifiers();
  if (MODIFIERS & Qt::AltModifier && m_pointToRemove) {
    removePoint(m_pointToRemove->index());
    m_pointToRemove = nullptr;
  }
}

void EditorWidget::mouseMoveEvent(QMouseEvent *a_event)
{
  QChartView::mouseMoveEvent(a_event);

  m_lastMousePosition = chart()->mapToValue(a_event->pos());
  if (m_pointToAdd) m_pointToAdd->setPos(chart()->mapToPosition(m_lastMousePosition));
}

void EditorWidget::resizeEvent(QResizeEvent *a_event)
{
  QChartView::resizeEvent(a_event);

  for (auto &point : m_points) {
    auto const index = point->index();
    auto const pointOnChart = m_series->at(index);
    auto const pointOnView = chart()->mapToPosition(pointOnChart);
    point->setPos(pointOnView);
  }
  updateCurrentValue();
}

void EditorWidget::setX(qreal const a_value)
{
  m_x = a_value;

  updateCurrentValue();
}

#if 0
void LineEditor::createPoints()
{
  for (int i = 0; i < m_series->count(); ++i) {
    auto point = m_series->at(i);
    auto pointPos = chart()->mapToPosition(point);

    auto pointItem = new Point{ this };
    pointItem->setType(Point::Type::eNormal);
    pointItem->setIndex(i);
    pointItem->setPos(pointPos);
    m_points.push_back(pointItem);
  }

  updateCurrentValue();
}
#endif

void EditorWidget::setHoveredItem(Point *const a_item)
{
  m_pointToRemove = a_item;
}

void EditorWidget::updateCurrentValue()
{
  QPointF previous{ m_series->at(0) };
  QPointF current{};

  int const SIZE = m_series->count();
  for (int i = 0; i < SIZE; ++i) {
    current = m_series->at(i);
    if (current.x() < m_x)
      previous = current;
    else
      break;
  }

  qreal y{};
  qreal percent{};

  if (m_x == previous.x())
    y = previous.y();
  else if (m_x == current.x())
    y = current.y();
  else {
    percent = (m_x - previous.x()) / (current.x() - previous.x());
    y = lerp(previous.y(), current.y(), percent);
  }

  QPointF const position{ m_x, y };
  m_currentValue->setPos(chart()->mapToPosition(position));

  m_y = y;

  auto point1 = position;
  point1.ry() = m_yAxis->max();
  auto point2 = position;
  point2.ry() = m_yAxis->min();
  auto const pointPos1 = chart()->mapToPosition(point1);
  auto const pointPos2 = chart()->mapToPosition(point2);

  m_line->setPos(pointPos1);
  m_line->setLength(pointPos2.y() - pointPos1.y());
}

void EditorWidget::updatePoints()
{
  for (auto &point : m_points) delete point;
  m_points.clear();

  int const SIZE{ m_series->count() };
  for (int i = 0; i < SIZE; ++i) {
    auto const POINT = m_series->at(i);
    auto const POINT_POS = chart()->mapToPosition(POINT);
    qDebug() << "Adding" << POINT << "AT" << POINT_POS;

    Point *const point = new Point{ this };
    point->setType(Point::Type::eNormal);
    point->setIndex(i);
    point->setPos(POINT_POS);
    m_points.push_back(point);
  }
}

void EditorWidget::addPoint(int const a_index, QPointF const a_point)
{
  m_series->insert(a_index, a_point);

  m_pointToAdd->setIndex(a_index);
  m_pointToAdd->setType(Point::Type::eNormal);

  int const SIZE = m_points.size();
  for (int i = a_index; i < SIZE; ++i) m_points[i]->setIndex(i + 1);
  m_points.push_back(m_pointToAdd);
  std::sort(std::begin(m_points), std::end(m_points),
            [](Point *const a_first, Point *const a_second) { return a_first->index() < a_second->index(); });

  updateCurrentValue();
}

void EditorWidget::removePoint(int const a_index)
{
  int const SIZE = m_points.size();
  for (int i = a_index; i < SIZE; ++i) m_points[i]->setIndex(i - 1);

  Point *const point = m_points[a_index];
  m_series->remove(a_index);
  m_points.removeAt(a_index);
  delete point;

  updateCurrentValue();
}

void EditorWidget::createPointToAdd()
{
  m_pointToAdd = new Point{ this };
  m_pointToAdd->setType(Point::Type::eToAdd);
  m_pointToAdd->setPos(chart()->mapToPosition(m_lastMousePosition));
}

} // namespace spaghetti::nodes::values::characteristic_curve
