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

#include "nodes/values/random_float_if.h"
#include <spaghetti/elements/values/random_float_if.h>

#include <QDoubleSpinBox>
#include <QTableWidget>

namespace spaghetti::nodes::values {

RandomFloatIf::RandomFloatIf() {}

void RandomFloatIf::showProperties()
{
  showCommonProperties();
  showIOProperties(IOSocketsType::eOutputs);

  propertiesInsertTitle("Random Float");

  propertiesInsertTitle("Enabled");

  int row = m_properties->rowCount();
  m_properties->insertRow(row);

  auto const randomFloat = static_cast<elements::values::RandomFloatIf *>(m_element);

  QTableWidgetItem *item{};

  item = new QTableWidgetItem{ "Minimum" };
  item->setFlags(item->flags() & ~Qt::ItemIsEditable);
  m_properties->setItem(row, 0, item);

  float const ENABLED_MIN_VALUE = randomFloat->enabledMin();

  auto const enabledMinValue = new QDoubleSpinBox;
  enabledMinValue->setRange(-9999999.0, 9999999.0);
  enabledMinValue->setDecimals(6);
  enabledMinValue->setValue(static_cast<qreal>(ENABLED_MIN_VALUE));
  m_properties->setCellWidget(row, 1, enabledMinValue);

  QObject::connect(enabledMinValue, static_cast<void (QDoubleSpinBox::*)(qreal)>(&QDoubleSpinBox::valueChanged),
                   [randomFloat](qreal a_value) { randomFloat->setEnabledMin(static_cast<float>(a_value)); });

  row = m_properties->rowCount();
  m_properties->insertRow(row);

  item = new QTableWidgetItem{ "Maximum" };
  item->setFlags(item->flags() & ~Qt::ItemIsEditable);
  m_properties->setItem(row, 0, item);

  float const ENABLED_MAX_VALUE = randomFloat->enabledMax();

  auto const enabledMaxValue = new QDoubleSpinBox;
  enabledMaxValue->setRange(-9999999.0, 9999999.0);
  enabledMaxValue->setDecimals(6);
  enabledMaxValue->setValue(static_cast<qreal>(ENABLED_MAX_VALUE));
  m_properties->setCellWidget(row, 1, enabledMaxValue);

  QObject::connect(enabledMaxValue, static_cast<void (QDoubleSpinBox::*)(qreal)>(&QDoubleSpinBox::valueChanged),
                   [randomFloat](qreal a_value) { randomFloat->setEnabledMax(static_cast<float>(a_value)); });

  propertiesInsertTitle("Disabled");

  row = m_properties->rowCount();
  m_properties->insertRow(row);

  item = new QTableWidgetItem{ "Minimum" };
  item->setFlags(item->flags() & ~Qt::ItemIsEditable);
  m_properties->setItem(row, 0, item);

  float const DISABLED_MIN_VALUE = randomFloat->disabledMin();

  auto const disabledMinValue = new QDoubleSpinBox;
  disabledMinValue->setRange(-9999999.0, 9999999.0);
  disabledMinValue->setDecimals(6);
  disabledMinValue->setValue(static_cast<qreal>(DISABLED_MIN_VALUE));
  m_properties->setCellWidget(row, 1, disabledMinValue);

  QObject::connect(disabledMinValue, static_cast<void (QDoubleSpinBox::*)(qreal)>(&QDoubleSpinBox::valueChanged),
                   [randomFloat](qreal a_value) { randomFloat->setDisabledMin(static_cast<float>(a_value)); });

  row = m_properties->rowCount();
  m_properties->insertRow(row);

  item = new QTableWidgetItem{ "Maximum" };
  item->setFlags(item->flags() & ~Qt::ItemIsEditable);
  m_properties->setItem(row, 0, item);

  float const DISABLED_MAX_VALUE = randomFloat->disabledMax();

  auto const disabledMaxValue = new QDoubleSpinBox;
  disabledMaxValue->setRange(-9999999.0, 9999999.0);
  disabledMaxValue->setDecimals(6);
  disabledMaxValue->setValue(static_cast<qreal>(DISABLED_MAX_VALUE));
  m_properties->setCellWidget(row, 1, disabledMaxValue);

  QObject::connect(disabledMaxValue, static_cast<void (QDoubleSpinBox::*)(qreal)>(&QDoubleSpinBox::valueChanged),
                   [randomFloat](qreal a_value) { randomFloat->setDisabledMax(static_cast<float>(a_value)); });
}

} // namespace spaghetti::nodes::values
