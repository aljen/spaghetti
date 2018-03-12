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

#include "nodes/values/random_float.h"
#include "elements/values/random_float.h"

#include <QDoubleSpinBox>
#include <QTableWidget>

namespace spaghetti::nodes::values {

RandomFloat::RandomFloat() {}

void RandomFloat::showProperties()
{
  showCommonProperties();
  showIOProperties(IOSocketsType::eOutputs);

  propertiesInsertTitle("Random Float");

  int row = m_properties->rowCount();
  m_properties->insertRow(row);

  auto const randomFloat = static_cast<elements::values::RandomFloat *const>(m_element);

  QTableWidgetItem *item{};

  item = new QTableWidgetItem{ "Minimum" };
  item->setFlags(item->flags() & ~Qt::ItemIsEditable);
  m_properties->setItem(row, 0, item);

  float const MIN_VALUE = randomFloat->min();

  auto const minValue = new QDoubleSpinBox;
  minValue->setRange(-9999999.0, 9999999.0);
  minValue->setDecimals(6);
  minValue->setValue(static_cast<qreal>(MIN_VALUE));
  m_properties->setCellWidget(row, 1, minValue);

  QObject::connect(minValue, static_cast<void (QDoubleSpinBox::*)(qreal)>(&QDoubleSpinBox::valueChanged),
                   [randomFloat](qreal a_value) { randomFloat->setMin(static_cast<float>(a_value)); });

  row = m_properties->rowCount();
  m_properties->insertRow(row);

  item = new QTableWidgetItem{ "Maximum" };
  item->setFlags(item->flags() & ~Qt::ItemIsEditable);
  m_properties->setItem(row, 0, item);

  float const MAX_VALUE = randomFloat->max();

  auto const maxValue = new QDoubleSpinBox;
  maxValue->setRange(-9999999.0, 9999999.0);
  maxValue->setDecimals(6);
  maxValue->setValue(static_cast<qreal>(MAX_VALUE));
  m_properties->setCellWidget(row, 1, maxValue);

  QObject::connect(maxValue, static_cast<void (QDoubleSpinBox::*)(qreal)>(&QDoubleSpinBox::valueChanged),
                   [randomFloat](qreal a_value) { randomFloat->setMax(static_cast<float>(a_value)); });
}

} // namespace spaghetti::nodes::values
