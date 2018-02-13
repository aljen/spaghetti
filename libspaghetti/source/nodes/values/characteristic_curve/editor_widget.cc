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

#include <QGuiApplication>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>

#include "nodes/values/characteristic_curve/axis.h"
#include "nodes/values/characteristic_curve/line.h"
#include "nodes/values/characteristic_curve/point.h"
#include "nodes/values/characteristic_curve/series.h"

using namespace QtCharts;

namespace spaghetti::nodes::values::characteristic_curve {

EditorWidget::EditorWidget(QWidget *const a_parent)
  : QChartView{ a_parent }
  , m_xAxis{ new QValueAxis } //  , m_series{ new QLineSeries }
                              //  , m_currentValue{ new Point{ this } }
  , m_line{ new Line{ QColor{ Qt::yellow }, this->chart() } }
{
  setRenderHint(QPainter::Antialiasing);
  setViewportUpdateMode(QChartView::FullViewportUpdate);

  m_xAxis->setMin(0.0);
  m_xAxis->setMax(1.0);
  m_xAxis->setTitleText("X");

  chart()->addAxis(m_xAxis, Qt::AlignBottom);
  chart()->setTheme(QChart::ChartThemeDark);

  //  m_series->append(0.0, 14);
  //  m_series->append(0.25, 18);
  //  m_series->append(0.45, 24);
  //  m_series->append(0.6, 29);
  //  m_series->append(0.75, 34);
  //  m_series->append(1.0, 50);
  //  m_series->setName("Wykres");

  //  chart()->addSeries(m_series);

  //  chart()->createDefaultAxes();

  //  auto xAxis = chart()->axisX(m_series);
  //  xAxis->setTitleText("Time");
  //  auto yAxis = static_cast<QValueAxis *>(chart()->axisY(m_series));
  ////  yAxis->setMin(0.0);
  ////  yAxis->setMax(148500.0);
  //  yAxis->setTickCount(5);
  //  yAxis->setMinorTickCount(5);
  //  yAxis->setTitleText("F");

  //  m_currentValue->setType(Point::Type::eCurrent);
}

void EditorWidget::setInputName(const QString &a_name)
{
  m_xAxis->setTitleText(a_name);
}

QString EditorWidget::inputName() const
{
  return m_xAxis->titleText();
}

void EditorWidget::setInputMajorTicks(const int a_ticks)
{
  m_xAxis->setTickCount(a_ticks);
}

int EditorWidget::inputMajorTicks() const
{
  return m_xAxis->tickCount();
}

void EditorWidget::setInputMinorTicks(const int a_ticks)
{
  m_xAxis->setMinorTickCount(a_ticks);
}

int EditorWidget::inputMinorTicks() const
{
  return m_xAxis->minorTickCount();
}

void EditorWidget::setInputMinimum(const qreal a_min)
{
  m_xAxis->setMin(a_min);
}

qreal EditorWidget::inputMinimum() const
{
  return m_xAxis->min();
}

void EditorWidget::setInputMaximum(const qreal a_max)
{
  m_xAxis->setMax(a_max);
}

qreal EditorWidget::inputMaximum() const
{
  return m_xAxis->max();
}

void EditorWidget::addAxis(Axis *const a_axis)
{
  m_axes.push_back(a_axis);

  chart()->addAxis(a_axis->axis(), Qt::AlignLeft);
}

void EditorWidget::removeAxis(const int a_index)
{
  auto axis = m_axes.at(a_index);
  m_axes.removeAt(a_index);
  delete axis;
}

void EditorWidget::addSeries(Series *const a_series)
{
  m_series.push_back(a_series);
  chart()->addSeries(a_series->series());
  a_series->series()->append(0.0, 0.0);
  a_series->series()->append(1.0, 1.0);
  a_series->series()->attachAxis(m_xAxis);
  a_series->series()->attachAxis(m_axes[0]->axis());

  //  chart()->createDefaultAxes();
}

void EditorWidget::removeSeries(const int a_index)
{
  auto series = m_series.at(a_index);
  m_series.removeAt(a_index);
  delete series;
}

void EditorWidget::keyPressEvent(QKeyEvent *a_event)
{
  QChartView::keyPressEvent(a_event);

  auto const modifiers = a_event->modifiers();
  if (modifiers & Qt::ControlModifier) {
    createPointToAdd();
  }
}

void EditorWidget::keyReleaseEvent(QKeyEvent *a_event)
{
  QChartView::keyReleaseEvent(a_event);

  auto const modifiers = a_event->modifiers();
  if ((modifiers & Qt::ControlModifier) == 0 && m_pointToAdd) {
    delete m_pointToAdd;
    m_pointToAdd = nullptr;
  }
}

void EditorWidget::mousePressEvent(QMouseEvent *a_event)
{
  QChartView::mousePressEvent(a_event);

#if 0
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

  auto const modifiers = QGuiApplication::keyboardModifiers();
  if (modifiers & Qt::AltModifier && m_pointToRemove) {
    removePoint(m_pointToRemove->index());
  }
#endif
}

void EditorWidget::mouseMoveEvent(QMouseEvent *a_event)
{
  QChartView::mouseMoveEvent(a_event);

#if 0
  m_lastMousePosition = chart()->mapToValue(a_event->pos());
  if (m_pointToAdd) m_pointToAdd->setPos(chart()->mapToPosition(m_lastMousePosition));
#endif
}

void EditorWidget::resizeEvent(QResizeEvent *a_event)
{
  QChartView::resizeEvent(a_event);

#if 0
  for (auto &point : m_points) {
    auto const index = point->index();
    auto const pointOnChart = m_series->at(index);
    auto const pointOnView = chart()->mapToPosition(pointOnChart);
    point->setPos(pointOnView);
  }
  updateCurrentValue();
#endif
}

void EditorWidget::setInput(qreal const a_value)
{
  m_input = a_value;

#if 0
  updateCurrentValue();
#endif
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
//  qDebug() << Q_FUNC_INFO;

#if 0
  QPointF previous{ m_series->at(0) };
  QPointF current{};

  int const SIZE = m_series->count();
  for (int i = 0; i < SIZE; ++i) {
    current = m_series->at(i);
    //    qDebug() << "testing point" << current << "previous:" << previous;
    if (current.x() < m_input) {
      //      qDebug() << "skipping point" << current << "previous:" << previous;
      previous = current;
    } else {
      //      qDebug() << "found point" << current << "previous:" << previous;
      break;
    }
  }

  qreal y{};
  qreal percent{};

  if (m_input == previous.x())
    y = previous.y();
  else if (m_input == current.x())
    y = current.y();
  else {
    percent = (m_input - previous.x()) / (current.x() - previous.x());
    y = lerp(previous.y(), current.y(), percent);
  }
  //  qDebug() << "percent:" << percent << "y:" << y;

  QPointF const position{ m_input, y };
  m_currentValue->setPos(chart()->mapToPosition(position));

  m_output = y;

  auto axisY = static_cast<QValueAxis*>(chart()->axisY(m_series));
  auto point1 = position;
  point1.ry() = axisY->max();
  auto point2 = position;
  point2.ry() = axisY->min();
  auto const pointPos1 = chart()->mapToPosition(point1);
  auto const pointPos2 = chart()->mapToPosition(point2);

  m_line->setPos(pointPos1);
  m_line->setLength(pointPos2.y() - pointPos1.y());
#endif
}

void EditorWidget::addPoint(const int a_index, const QPointF a_point)
{
  (void)a_index;
  (void)a_point;
#if 0
  m_series->insert(a_index, a_point);

  m_pointToAdd->setIndex(a_index);
  m_pointToAdd->setType(Point::Type::eNormal);

  int const SIZE = m_points.size();
  for (int i = a_index; i < SIZE; ++i) m_points[i]->setIndex(i + 1);
  m_points.push_back(m_pointToAdd);
  std::sort(std::begin(m_points), std::end(m_points),
            [](Point *const a_first, Point *const a_second) { return a_first->index() < a_second->index(); });

  updateCurrentValue();
#endif
}

void EditorWidget::removePoint(const int a_index)
{
  (void)a_index;
#if 0
  int const SIZE = m_points.size();
  for (int i = a_index; i < SIZE; ++i) m_points[i]->setIndex(i - 1);

  Point *const point = m_points[a_index];
  m_series->remove(a_index);
  m_points.removeAt(a_index);
  delete point;

  updateCurrentValue();
#endif
}

void EditorWidget::createPointToAdd()
{
  m_pointToAdd = new Point{ this };
  m_pointToAdd->setType(Point::Type::eToAdd);
  m_pointToAdd->setPos(chart()->mapToPosition(m_lastMousePosition));
}

} // namespace spaghetti::nodes::values::characteristic_curve
