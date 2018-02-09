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

#include "nodes/values/characteristic_curve_int_int.h"

#include <QDebug>
#include <QPushButton>
#include <QSpinBox>
#include <QTableWidget>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>

#include "elements/values/characteristic_curve_int_int.h"
#include "nodes/values/characteristic_curve/point.h"
#include "spaghetti/package.h"

using namespace QtCharts;

namespace spaghetti::nodes::values {

CharacteristicCurveIntInt::CharacteristicCurveIntInt()
  : m_widget{ new QChart }
  , m_series{ new QLineSeries }
  , m_xAxis{ new QValueAxis }
  , m_yAxis{ new QValueAxis }
  , m_lastPoint{ -1.0, -1.0 }
  , m_current{ new CharacteristicCurvePoint{ m_widget } }
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

  m_current->setType(CharacteristicCurvePoint::Type::eCurrent);
}

void CharacteristicCurveIntInt::refreshCentralWidget()
{
  if (!m_element) return;

  updateCurrentValue(false);
  calculateBoundingRect();
}

void CharacteristicCurveIntInt::showProperties()
{
  showCommonProperties();
  showIOProperties(IOSocketsType::eInputs);
  showIOProperties(IOSocketsType::eOutputs);

  propertiesInsertTitle("X range");

  int currentIndex = m_properties->rowCount();
  m_properties->insertRow(currentIndex);

  auto const scaler = static_cast<elements::values::CharacteristicCurveIntInt *const>(m_element);

  QTableWidgetItem *item{};
  item = new QTableWidgetItem{ "Min" };
  item->setFlags(item->flags() & ~Qt::ItemIsEditable);
  m_properties->setItem(currentIndex, 0, item);

  auto const xMinValue = new QSpinBox{};
  xMinValue->setRange(-10000, 10000);
  xMinValue->setValue(scaler->xMin());
  m_properties->setCellWidget(currentIndex, 1, xMinValue);

  QObject::connect(xMinValue, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [this](int a_value) {
    auto const scalerElement = static_cast<elements::values::CharacteristicCurveIntInt *const>(m_element);
    auto const package = scalerElement->package();
    package->pauseDispatchThread();
    scalerElement->setXMin(a_value);
    synchronizeFromElement();
    package->resumeDispatchThread();
  });

  currentIndex = m_properties->rowCount();
  m_properties->insertRow(currentIndex);

  item = new QTableWidgetItem{ "Max" };
  item->setFlags(item->flags() & ~Qt::ItemIsEditable);
  m_properties->setItem(currentIndex, 0, item);

  auto const xMaxValue = new QSpinBox{};
  xMaxValue->setRange(-10000, 10000);
  xMaxValue->setValue(scaler->xMax());
  m_properties->setCellWidget(currentIndex, 1, xMaxValue);

  QObject::connect(xMaxValue, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [this](int a_value) {
    auto const scalerElement{ static_cast<elements::values::CharacteristicCurveIntInt *const>(m_element) };
    auto const package = scalerElement->package();
    package->pauseDispatchThread();
    scalerElement->setXMax(a_value);
    synchronizeFromElement();
    package->resumeDispatchThread();
  });

  propertiesInsertTitle("Y range");

  currentIndex = m_properties->rowCount();
  m_properties->insertRow(currentIndex);

  item = new QTableWidgetItem{ "Min" };
  item->setFlags(item->flags() & ~Qt::ItemIsEditable);
  m_properties->setItem(currentIndex, 0, item);

  auto const yMinValue = new QSpinBox{};
  yMinValue->setRange(-10000, 10000);
  yMinValue->setValue(scaler->yMin());
  m_properties->setCellWidget(currentIndex, 1, yMinValue);

  QObject::connect(yMinValue, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [this](int a_value) {
    auto const scalerElement{ static_cast<elements::values::CharacteristicCurveIntInt *const>(m_element) };
    auto const package = scalerElement->package();
    package->pauseDispatchThread();
    scalerElement->setYMin(a_value);
    synchronizeFromElement();
    package->resumeDispatchThread();
  });

  currentIndex = m_properties->rowCount();
  m_properties->insertRow(currentIndex);

  item = new QTableWidgetItem{ "Max" };
  item->setFlags(item->flags() & ~Qt::ItemIsEditable);
  m_properties->setItem(currentIndex, 0, item);

  auto const yMaxValue = new QSpinBox{};
  yMaxValue->setRange(-10000, 10000);
  yMaxValue->setValue(scaler->yMax());
  m_properties->setCellWidget(currentIndex, 1, yMaxValue);

  QObject::connect(yMaxValue, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [this](int a_value) {
    auto const scalerElement{ static_cast<elements::values::CharacteristicCurveIntInt *const>(m_element) };
    auto const package = scalerElement->package();
    package->pauseDispatchThread();
    scalerElement->setYMax(a_value);
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
    auto const scalerElement{ static_cast<elements::values::CharacteristicCurveIntInt *const>(m_element) };
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

  QObject::connect(editSeries, &QPushButton::clicked, [this] { qDebug() << "EDIT SERIES"; });
}

void CharacteristicCurveIntInt::elementSet()
{
  constexpr qreal const CHART_SIZE{ 400. };
  m_widget->resize({ CHART_SIZE, CHART_SIZE });

  synchronizeFromElement();
}

void CharacteristicCurveIntInt::synchronizeFromElement()
{
  assert(m_element);

  auto const element = static_cast<elements::values::CharacteristicCurveIntInt *>(m_element);

  m_xAxis->setMin(static_cast<qreal>(element->xMin()));
  m_xAxis->setMax(static_cast<qreal>(element->xMax()));
  m_yAxis->setMin(static_cast<qreal>(element->yMin()));
  m_yAxis->setMax(static_cast<qreal>(element->yMax()));

  m_series->clear();

  auto &series = element->series();
  for (auto &point : series) m_series->append(static_cast<qreal>(point.x), static_cast<qreal>(point.y));

  updateCurrentValue(true);
}

void CharacteristicCurveIntInt::updateCurrentValue(bool const a_force)
{
  auto const element = static_cast<elements::values::CharacteristicCurveIntInt *>(m_element);
  auto const POINT = element->value();
  QPointF const position{ static_cast<qreal>(POINT.x), static_cast<qreal>(POINT.y) };

  if (a_force || position != m_lastPoint) {
    m_current->setPos(m_widget->mapToPosition(position));
    calculateBoundingRect();
    m_lastPoint = position;
  }
}

} // namespace spaghetti::nodes::values

