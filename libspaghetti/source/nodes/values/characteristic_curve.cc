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

#include "nodes/values/characteristic_curve.h"

#include <QDebug>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QSpinBox>
#include <QTableWidget>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>

#include "elements/values/characteristic_curve.h"
#include "nodes/values/characteristic_curve/editor_window.h"
#include "nodes/values/characteristic_curve/point.h"
#include "spaghetti/package.h"

using namespace QtCharts;

using ElementType = spaghetti::elements::values::CharacteristicCurve;

namespace spaghetti::nodes::values {

CharacteristicCurve::CharacteristicCurve()
  : m_widget{ new QChart }
  , m_series{ new QLineSeries }
  , m_xAxis{ new QValueAxis }
  , m_yAxis{ new QValueAxis }
  , m_lastPoint{ -1.0, -1.0 }
  , m_current{ new characteristic_curve::Point{ m_widget } }
  , m_editor{ new characteristic_curve::EditorWindow{ this } }
{
  setCentralWidget(m_widget);

  m_widget->legend()->hide();
  m_widget->setTheme(QChart::ChartThemeDark);
  m_widget->addAxis(m_xAxis, Qt::AlignBottom);
  m_widget->addAxis(m_yAxis, Qt::AlignLeft);
  m_widget->addSeries(m_series);

  m_series->attachAxis(m_xAxis);
  m_series->attachAxis(m_yAxis);

  QFont labelsFont{};
  labelsFont.setPixelSize(10);
  m_xAxis->setLabelsFont(labelsFont);
  m_yAxis->setLabelsFont(labelsFont);

  m_current->setType(characteristic_curve::Point::Type::eCurrent);
}

CharacteristicCurve::~CharacteristicCurve()
{
  delete m_editor;
}

void CharacteristicCurve::refreshCentralWidget()
{
  if (!m_element) return;

  updateCurrentValue(false);
  calculateBoundingRect();
}

void CharacteristicCurve::showProperties()
{
  showCommonProperties();
  showIOProperties(IOSocketsType::eInputs);
  showIOProperties(IOSocketsType::eOutputs);

  propertiesInsertTitle("X range");

  int currentIndex = m_properties->rowCount();
  m_properties->insertRow(currentIndex);

  auto const scaler = static_cast<ElementType *const>(m_element);

  QTableWidgetItem *item{};
  item = new QTableWidgetItem{ "Min" };
  item->setFlags(item->flags() & ~Qt::ItemIsEditable);
  m_properties->setItem(currentIndex, 0, item);

  auto const xMinValue = new QDoubleSpinBox{};
  xMinValue->setRange(-10000.0, 10000.0);
  xMinValue->setValue(static_cast<qreal>(scaler->xMinimum()));
  m_properties->setCellWidget(currentIndex, 1, xMinValue);

  QObject::connect(xMinValue, static_cast<void (QDoubleSpinBox::*)(qreal)>(&QDoubleSpinBox::valueChanged),
                   [this](qreal a_value) {
                     auto const scalerElement = static_cast<ElementType *const>(m_element);
                     auto const package = scalerElement->package();
                     package->pauseDispatchThread();
                     scalerElement->setXMinimum(static_cast<float>(a_value));
                     synchronizeFromElement();
                     package->resumeDispatchThread();
                   });

  currentIndex = m_properties->rowCount();
  m_properties->insertRow(currentIndex);

  item = new QTableWidgetItem{ "Max" };
  item->setFlags(item->flags() & ~Qt::ItemIsEditable);
  m_properties->setItem(currentIndex, 0, item);

  auto const xMaxValue = new QDoubleSpinBox{};
  xMaxValue->setRange(-10000.0, 10000.0);
  xMaxValue->setValue(static_cast<qreal>(scaler->xMaximum()));
  m_properties->setCellWidget(currentIndex, 1, xMaxValue);

  QObject::connect(xMaxValue, static_cast<void (QDoubleSpinBox::*)(qreal)>(&QDoubleSpinBox::valueChanged),
                   [this](qreal a_value) {
                     auto const scalerElement{ static_cast<ElementType *const>(m_element) };
                     auto const package = scalerElement->package();
                     package->pauseDispatchThread();
                     scalerElement->setXMaximum(static_cast<float>(a_value));
                     synchronizeFromElement();
                     package->resumeDispatchThread();
                   });

  propertiesInsertTitle("Y range");

  currentIndex = m_properties->rowCount();
  m_properties->insertRow(currentIndex);

  item = new QTableWidgetItem{ "Min" };
  item->setFlags(item->flags() & ~Qt::ItemIsEditable);
  m_properties->setItem(currentIndex, 0, item);

  auto const yMinValue = new QDoubleSpinBox{};
  yMinValue->setRange(-10000.0, 10000.0);
  yMinValue->setValue(static_cast<qreal>(scaler->yMinimum()));
  m_properties->setCellWidget(currentIndex, 1, yMinValue);

  QObject::connect(yMinValue, static_cast<void (QDoubleSpinBox::*)(qreal)>(&QDoubleSpinBox::valueChanged),
                   [this](qreal a_value) {
                     auto const scalerElement{ static_cast<ElementType *const>(m_element) };
                     auto const package = scalerElement->package();
                     package->pauseDispatchThread();
                     scalerElement->setYMinimum(static_cast<float>(a_value));
                     synchronizeFromElement();
                     package->resumeDispatchThread();
                   });

  currentIndex = m_properties->rowCount();
  m_properties->insertRow(currentIndex);

  item = new QTableWidgetItem{ "Max" };
  item->setFlags(item->flags() & ~Qt::ItemIsEditable);
  m_properties->setItem(currentIndex, 0, item);

  auto const yMaxValue = new QDoubleSpinBox{};
  yMaxValue->setRange(-10000.0, 10000.0);
  yMaxValue->setValue(static_cast<qreal>(scaler->yMaximum()));
  m_properties->setCellWidget(currentIndex, 1, yMaxValue);

  QObject::connect(yMaxValue, static_cast<void (QDoubleSpinBox::*)(qreal)>(&QDoubleSpinBox::valueChanged),
                   [this](qreal a_value) {
                     auto const scalerElement{ static_cast<ElementType *const>(m_element) };
                     auto const package = scalerElement->package();
                     package->pauseDispatchThread();
                     scalerElement->setYMaximum(static_cast<float>(a_value));
                     synchronizeFromElement();
                     package->resumeDispatchThread();
                   });

  propertiesInsertTitle("Series");

  currentIndex = m_properties->rowCount();
  m_properties->insertRow(currentIndex);

  item = new QTableWidgetItem{ "Count" };
  item->setFlags(item->flags() & ~Qt::ItemIsEditable);
  m_properties->setItem(currentIndex, 0, item);

  auto const countValue = new QSpinBox{};
  countValue->setRange(2, 100);
  countValue->setValue(static_cast<int32_t>(scaler->seriesCount()));
  m_properties->setCellWidget(currentIndex, 1, countValue);

  QObject::connect(countValue, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [this](int a_value) {
    auto const scalerElement{ static_cast<ElementType *const>(m_element) };
    auto const package = scalerElement->package();
    package->pauseDispatchThread();
    scalerElement->setSeriesCount(static_cast<size_t>(a_value));
    synchronizeFromElement();
    package->resumeDispatchThread();
  });

  currentIndex = m_properties->rowCount();
  m_properties->insertRow(currentIndex);

  auto const editSeries = new QPushButton{ "Edit Series" };
  m_properties->setSpan(currentIndex, 0, 1, 2);
  m_properties->setCellWidget(currentIndex, 0, editSeries);

  QObject::connect(editSeries, &QPushButton::clicked, [this] { open(); });
}

void CharacteristicCurve::elementSet()
{
  constexpr qreal const CHART_SIZE{ 400. };
  m_widget->resize({ CHART_SIZE, CHART_SIZE });

  synchronizeFromElement();
}

bool CharacteristicCurve::open()
{
  m_editor->synchronizeFromNode();
  m_editor->exec();
//  m_editor->show();
  return true;
}

void CharacteristicCurve::synchronizeFromElement()
{
  assert(m_element);

  auto const element = static_cast<ElementType *>(m_element);

  m_xAxis->setMin(static_cast<qreal>(element->xMinimum()));
  m_xAxis->setMax(static_cast<qreal>(element->xMaximum()));
  m_xAxis->setTitleText(inputs()[0]->name());
  m_yAxis->setMin(static_cast<qreal>(element->yMinimum()));
  m_yAxis->setMax(static_cast<qreal>(element->yMaximum()));
  m_yAxis->setTitleText(outputs()[0]->name());

  m_series->clear();

  auto &series = element->series();
  for (auto &point : series) m_series->append(static_cast<qreal>(point.x), static_cast<qreal>(point.y));

  updateCurrentValue(true);
}

void CharacteristicCurve::updateCurrentValue(bool const a_force)
{
  auto const element = static_cast<ElementType *>(m_element);
  auto const POINT = element->value();
  QPointF const position{ static_cast<qreal>(POINT.x), static_cast<qreal>(POINT.y) };

  if (a_force || position != m_lastPoint) {
    m_current->setPos(m_widget->mapToPosition(position));
    calculateBoundingRect();
    m_lastPoint = position;
    if (m_editor->isVisible())
      m_editor->setValue(static_cast<qreal>(POINT.x));
  }
}

} // namespace spaghetti::nodes::values
