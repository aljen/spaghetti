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

#include "nodes/values/scale_int.h"
#include "elements/values/scale_int.h"

#include <QDebug>
#include <QPushButton>
#include <QSpinBox>
#include <QTableWidget>

namespace spaghetti::nodes::values {

class ScaleIntWidget : public QGraphicsItem {
 public:
  ScaleIntWidget()
    : QGraphicsItem{}
  {
  }

  QRectF boundingRect() const override { return m_boundingRect; }

  void paint(QPainter *a_painter, QStyleOptionGraphicsItem const *a_option, QWidget *a_widget) override
  {
    (void)a_painter;
    (void)a_option;
    (void)a_widget;
  }

 private:
  QRectF m_boundingRect{ 0, 0, 194, 222 };
};

ScaleInt::ScaleInt()
{
  auto const widget = new ScaleIntWidget;
  setCentralWidget(widget);

  m_widget = widget;
}

void ScaleInt::refreshCentralWidget()
{
  if (!m_element) return;

  //  auto const &inputs = m_element->inputs();

  //  bool const VALUE{ std::get<bool>(inputs[0].value) };

  calculateBoundingRect();
}

void ScaleInt::showProperties()
{
  showCommonProperties();
  showIOProperties(IOSocketsType::eInputs);
  showIOProperties(IOSocketsType::eOutputs);

  propertiesInsertTitle("X range");

  int currentIndex = m_properties->rowCount();
  m_properties->insertRow(currentIndex);

  elements::values::ScaleInt *const scaler{ static_cast<elements::values::ScaleInt *const>(m_element) };

  QTableWidgetItem *item{};
  item = new QTableWidgetItem{ "Min" };
  item->setFlags(item->flags() & ~Qt::ItemIsEditable);
  m_properties->setItem(currentIndex, 0, item);

  QSpinBox *xMinValue = new QSpinBox{};
  xMinValue->setRange(-10000, 10000);
  xMinValue->setValue(scaler->xMin());
  m_properties->setCellWidget(currentIndex, 1, xMinValue);

  QObject::connect(xMinValue, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [this](int a_value) {
    elements::values::ScaleInt *const scalerElement{ static_cast<elements::values::ScaleInt *const>(m_element) };
    scalerElement->setXMin(a_value);
  });

  currentIndex = m_properties->rowCount();
  m_properties->insertRow(currentIndex);

  item = new QTableWidgetItem{ "Max" };
  item->setFlags(item->flags() & ~Qt::ItemIsEditable);
  m_properties->setItem(currentIndex, 0, item);

  QSpinBox *xMaxValue = new QSpinBox{};
  xMaxValue->setRange(-10000, 10000);
  xMaxValue->setValue(scaler->xMax());
  m_properties->setCellWidget(currentIndex, 1, xMaxValue);

  QObject::connect(xMaxValue, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [this](int a_value) {
    elements::values::ScaleInt *const scalerElement{ static_cast<elements::values::ScaleInt *const>(m_element) };
    scalerElement->setXMax(a_value);
  });

  propertiesInsertTitle("Y range");

  currentIndex = m_properties->rowCount();
  m_properties->insertRow(currentIndex);

  item = new QTableWidgetItem{ "Min" };
  item->setFlags(item->flags() & ~Qt::ItemIsEditable);
  m_properties->setItem(currentIndex, 0, item);

  QSpinBox *yMinValue = new QSpinBox{};
  yMinValue->setRange(-10000, 10000);
  yMinValue->setValue(scaler->yMin());
  m_properties->setCellWidget(currentIndex, 1, yMinValue);

  QObject::connect(yMinValue, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [this](int a_value) {
    elements::values::ScaleInt *const scalerElement{ static_cast<elements::values::ScaleInt *const>(m_element) };
    scalerElement->setYMin(a_value);
  });

  currentIndex = m_properties->rowCount();
  m_properties->insertRow(currentIndex);

  item = new QTableWidgetItem{ "Max" };
  item->setFlags(item->flags() & ~Qt::ItemIsEditable);
  m_properties->setItem(currentIndex, 0, item);

  QSpinBox *yMaxValue = new QSpinBox{};
  yMaxValue->setRange(-10000, 10000);
  yMaxValue->setValue(scaler->yMax());
  m_properties->setCellWidget(currentIndex, 1, yMaxValue);

  QObject::connect(yMaxValue, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [this](int a_value) {
    elements::values::ScaleInt *const scalerElement{ static_cast<elements::values::ScaleInt *const>(m_element) };
    scalerElement->setYMax(a_value);
  });

  propertiesInsertTitle("Series");

  currentIndex = m_properties->rowCount();
  m_properties->insertRow(currentIndex);

  item = new QTableWidgetItem{ "Count" };
  item->setFlags(item->flags() & ~Qt::ItemIsEditable);
  m_properties->setItem(currentIndex, 0, item);

  QSpinBox *countValue = new QSpinBox{};
  countValue->setRange(2, 100);
  countValue->setValue(static_cast<int32_t>(scaler->seriesCount()));
  m_properties->setCellWidget(currentIndex, 1, countValue);

  QObject::connect(countValue, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [this](int a_value) {
    elements::values::ScaleInt *const scalerElement{ static_cast<elements::values::ScaleInt *const>(m_element) };
    scalerElement->setSeriesCount(static_cast<size_t>(a_value));
  });

  currentIndex = m_properties->rowCount();
  m_properties->insertRow(currentIndex);

  QPushButton *editSeries = new QPushButton{ "Edit Series" };
  m_properties->setSpan(currentIndex, 0, 1, 2);
  m_properties->setCellWidget(currentIndex, 0, editSeries);

  QObject::connect(editSeries, &QPushButton::clicked, [this] { qDebug() << "EDIT SERIES"; });
}

} // namespace spaghetti::nodes::values
