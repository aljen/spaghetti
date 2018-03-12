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

#include "nodes/values/random_int_if.h"
#include "elements/values/random_int_if.h"

#include <QSpinBox>
#include <QTableWidget>

namespace spaghetti::nodes::values {

RandomIntIf::RandomIntIf() {}

void RandomIntIf::showProperties()
{
  showCommonProperties();
  showIOProperties(IOSocketsType::eOutputs);

  propertiesInsertTitle("Random Int");

  propertiesInsertTitle("Enabled");

  int row = m_properties->rowCount();
  m_properties->insertRow(row);

  auto const randomInt = static_cast<elements::values::RandomIntIf *const>(m_element);

  QTableWidgetItem *item{};

  item = new QTableWidgetItem{ "Minimum" };
  item->setFlags(item->flags() & ~Qt::ItemIsEditable);
  m_properties->setItem(row, 0, item);

  int const ENABLED_MIN_VALUE = randomInt->enabledMin();

  QSpinBox *const enabledMinValue = new QSpinBox;
  enabledMinValue->setRange(std::numeric_limits<int32_t>::min(), std::numeric_limits<int32_t>::max());
  enabledMinValue->setValue(static_cast<int>(ENABLED_MIN_VALUE));
  m_properties->setCellWidget(row, 1, enabledMinValue);

  QObject::connect(enabledMinValue, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
                   [randomInt](int a_value) { randomInt->setEnabledMin(a_value); });

  row = m_properties->rowCount();
  m_properties->insertRow(row);

  item = new QTableWidgetItem{ "Maximum" };
  item->setFlags(item->flags() & ~Qt::ItemIsEditable);
  m_properties->setItem(row, 0, item);

  int const ENABLED_MAX_VALUE = randomInt->enabledMax();

  QSpinBox *const enabledMaxValue = new QSpinBox;
  enabledMaxValue->setRange(std::numeric_limits<int32_t>::min(), std::numeric_limits<int32_t>::max());
  enabledMaxValue->setValue(static_cast<int>(ENABLED_MAX_VALUE));
  m_properties->setCellWidget(row, 1, enabledMaxValue);

  QObject::connect(enabledMaxValue, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
                   [randomInt](int a_value) { randomInt->setEnabledMax(a_value); });

  propertiesInsertTitle("Disabled");

  row = m_properties->rowCount();
  m_properties->insertRow(row);

  item = new QTableWidgetItem{ "Minimum" };
  item->setFlags(item->flags() & ~Qt::ItemIsEditable);
  m_properties->setItem(row, 0, item);

  int const DISABLED_MIN_VALUE = randomInt->disabledMin();

  QSpinBox *const disabledMinValue = new QSpinBox;
  disabledMinValue->setRange(std::numeric_limits<int32_t>::min(), std::numeric_limits<int32_t>::max());
  disabledMinValue->setValue(static_cast<int>(DISABLED_MIN_VALUE));
  m_properties->setCellWidget(row, 1, disabledMinValue);

  QObject::connect(disabledMinValue, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
                   [randomInt](int a_value) { randomInt->setDisabledMin(a_value); });

  row = m_properties->rowCount();
  m_properties->insertRow(row);

  item = new QTableWidgetItem{ "Maximum" };
  item->setFlags(item->flags() & ~Qt::ItemIsEditable);
  m_properties->setItem(row, 0, item);

  int const DISABLED_MAX_VALUE = randomInt->disabledMax();

  QSpinBox *const disabledMaxValue = new QSpinBox;
  disabledMaxValue->setRange(std::numeric_limits<int32_t>::min(), std::numeric_limits<int32_t>::max());
  disabledMaxValue->setValue(static_cast<int>(DISABLED_MAX_VALUE));
  m_properties->setCellWidget(row, 1, disabledMaxValue);

  QObject::connect(disabledMaxValue, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
                   [randomInt](int a_value) { randomInt->setDisabledMax(a_value); });
}

} // namespace spaghetti::nodes::values
