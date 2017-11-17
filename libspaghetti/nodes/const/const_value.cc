// MIT License
//
// Copyright (c) 2017 Artur Wyszyński, aljen at hitomi dot pl
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

#include "nodes/const_value.h"

#include "elements/logic/const_bool.h"
#include "elements/logic/const_float.h"
#include "elements/logic/const_int.h"

#include <QCheckBox>
#include <QDebug>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QTableWidget>

namespace nodes {

void ConstBool::showProperties()
{
  showCommonProperties();
  showOutputsProperties();

  propertiesInsertTitle("Const Bool");

  int row = m_properties->rowCount();
  m_properties->insertRow(row);

  QTableWidgetItem *item{};

  item = new QTableWidgetItem{ "Value" };
  item->setFlags(item->flags() & ~Qt::ItemIsEditable);
  m_properties->setItem(row, 0, item);

  auto const constBool = static_cast<elements::logic::ConstBool *const>(m_element);
  bool const current = constBool->currentValue();

  QCheckBox *const value = new QCheckBox;
  m_properties->setCellWidget(row, 1, value);
  value->setChecked(current);

  QObject::connect(value, &QCheckBox::stateChanged, [constBool](int a_state) {
    constBool->set(a_state == 2);
  });
}

void ConstInt::showProperties()
{
  showCommonProperties();
  showOutputsProperties();

  propertiesInsertTitle("Const Int");

  int row = m_properties->rowCount();
  m_properties->insertRow(row);

  QTableWidgetItem *item{};

  item = new QTableWidgetItem{ "Value" };
  item->setFlags(item->flags() & ~Qt::ItemIsEditable);
  m_properties->setItem(row, 0, item);

  auto const constInt = static_cast<elements::logic::ConstInt *const>(m_element);
  int const current = constInt->currentValue();

  QSpinBox *const value = new QSpinBox;
  value->setRange(std::numeric_limits<int32_t>::min(), std::numeric_limits<int32_t>::max());
  value->setValue(static_cast<int>(current));
  m_properties->setCellWidget(row, 1, value);

  QObject::connect(value, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [constInt](int a_value) {
    constInt->set(a_value);
  });
}

void ConstFloat::showProperties()
{
  showCommonProperties();
  showOutputsProperties();

  propertiesInsertTitle("Const Float");

  int row = m_properties->rowCount();
  m_properties->insertRow(row);

  QTableWidgetItem *item{};

  item = new QTableWidgetItem{ "Value" };
  item->setFlags(item->flags() & ~Qt::ItemIsEditable);
  m_properties->setItem(row, 0, item);

  auto const constFloat = static_cast<elements::logic::ConstFloat *const>(m_element);
  float const current = constFloat->currentValue();

  QDoubleSpinBox *const value = new QDoubleSpinBox;
  value->setRange(std::numeric_limits<float>::min(), std::numeric_limits<float>::max());
  value->setValue(current);
  m_properties->setCellWidget(row, 1, value);

  QObject::connect(value, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [constFloat](double a_value) {
    constFloat->set(static_cast<float>(a_value));
  });
}

} // namespace nodes
