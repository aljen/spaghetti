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

#include "nodes/clock.h"
#include "elements/logic/clock.h"

#include <QSpinBox>
#include <QTableWidget>

using elements::logic::Clock;

namespace nodes {

void Clock::showProperties()
{
  showCommonProperties();
  showOutputsProperties();

  propertiesInsertTitle("Clock");

  int currentIndex = m_properties->rowCount();
  m_properties->insertRow(currentIndex);

  QTableWidgetItem *item{};
  item = new QTableWidgetItem{ "Rate" };
  item->setFlags(item->flags() & ~Qt::ItemIsEditable);
  m_properties->setItem(currentIndex, 0, item);

  elements::logic::Clock *const clock{ static_cast<elements::logic::Clock *const>(m_element) };

  QSpinBox *rateValue = new QSpinBox{};
  rateValue->setRange(10, 10000);
  rateValue->setValue(static_cast<int>(clock->duration().count()));
  rateValue->setSuffix("ms");
  m_properties->setCellWidget(currentIndex, 1, rateValue);

  QObject::connect(rateValue, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [this](int a_value) {
    elements::logic::Clock *const clockElement{ static_cast<elements::logic::Clock *const>(m_element) };
    clockElement->setDuration(std::chrono::milliseconds(a_value));
  });
}

} // namespace nodes
