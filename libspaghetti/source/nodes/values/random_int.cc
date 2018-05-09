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

#include "nodes/values/random_int.h"
#include "elements/values/random_int.h"

#include <QSpinBox>
#include <QTableWidget>

namespace spaghetti::nodes::values {

RandomInt::RandomInt() {}

void RandomInt::showProperties()
{
  showCommonProperties();
  showIOProperties(IOSocketsType::eOutputs);

  propertiesInsertTitle("Random Int");

  int row = m_properties->rowCount();
  m_properties->insertRow(row);

  auto const randomInt = static_cast<elements::values::RandomInt *>(m_element);

  QTableWidgetItem *item{};

  item = new QTableWidgetItem{ "Minimum" };
  item->setFlags(item->flags() & ~Qt::ItemIsEditable);
  m_properties->setItem(row, 0, item);

  int const MIN_VALUE = randomInt->min();

  QSpinBox *const minValue = new QSpinBox;
  minValue->setRange(std::numeric_limits<int32_t>::min(), std::numeric_limits<int32_t>::max());
  minValue->setValue(static_cast<int>(MIN_VALUE));
  m_properties->setCellWidget(row, 1, minValue);

  QObject::connect(minValue, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
                   [randomInt](int a_value) { randomInt->setMin(a_value); });

  row = m_properties->rowCount();
  m_properties->insertRow(row);

  item = new QTableWidgetItem{ "Maximum" };
  item->setFlags(item->flags() & ~Qt::ItemIsEditable);
  m_properties->setItem(row, 0, item);

  int const MAX_VALUE = randomInt->max();

  QSpinBox *const maxValue = new QSpinBox;
  maxValue->setRange(std::numeric_limits<int32_t>::min(), std::numeric_limits<int32_t>::max());
  maxValue->setValue(static_cast<int>(MAX_VALUE));
  m_properties->setCellWidget(row, 1, maxValue);

  QObject::connect(maxValue, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
                   [randomInt](int a_value) { randomInt->setMax(a_value); });
}

} // namespace spaghetti::nodes::values
