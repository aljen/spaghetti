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

#include "nodes/values/const_int.h"
#include "elements/values/const_int.h"

#include <QSpinBox>
#include <QTableWidget>

namespace spaghetti::nodes::values {

ConstInt::ConstInt()
{
  QFont font{};
  font.setFamily("Consolas");
  font.setPointSize(10);
  auto widget = new QGraphicsSimpleTextItem(QString::number(0));
  widget->setFont(font);

  auto brush = widget->brush();
  brush.setColor(Qt::white);
  widget->setBrush(brush);

  setCentralWidget(widget);

  m_info = widget;
}

void ConstInt::refreshCentralWidget()
{
  if (!m_element) return;
  int32_t const VALUE{ std::get<int32_t>(m_element->outputs()[0].value) };
  m_info->setText(QString::number(VALUE));

  calculateBoundingRect();
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

  auto const CONST_INT = static_cast<elements::values::ConstInt *const>(m_element);
  int const CURRENT = CONST_INT->currentValue();

  QSpinBox *const value = new QSpinBox;
  value->setRange(std::numeric_limits<int32_t>::min(), std::numeric_limits<int32_t>::max());
  value->setValue(static_cast<int>(CURRENT));
  m_properties->setCellWidget(row, 1, value);

  QObject::connect(value, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
                   [CONST_INT](int a_value) { CONST_INT->set(a_value); });
}

} // namespace spaghetti::nodes::values
