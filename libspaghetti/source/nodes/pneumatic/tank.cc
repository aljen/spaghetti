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

#include "nodes/pneumatic/tank.h"
#include "elements/pneumatic/tank.h"

#include <QDoubleSpinBox>
#include <QTableWidget>

namespace spaghetti::nodes::pneumatic {

void Tank::showProperties()
{
  showCommonProperties();
  showIOProperties(IOSocketsType::eInputs);

  propertiesInsertTitle("Tank");

  auto const tank{ static_cast<elements::pneumatic::Tank *const>(m_element) };

  int currentIndex = m_properties->rowCount();
  m_properties->insertRow(currentIndex);

  QTableWidgetItem *item{};
  item = new QTableWidgetItem{ "Pressure" };
  item->setFlags(item->flags() & ~Qt::ItemIsEditable);
  m_properties->setItem(currentIndex, 0, item);

  QDoubleSpinBox *pressureValue = new QDoubleSpinBox;
  pressureValue->setRange(1.0, 10000.0);
  pressureValue->setValue(static_cast<qreal>(tank->initialPressure()));
  m_properties->setCellWidget(currentIndex, 1, pressureValue);

  QObject::connect(pressureValue, static_cast<void (QDoubleSpinBox::*)(qreal)>(&QDoubleSpinBox::valueChanged),
                   [this](qreal a_value) {
                     auto const tankElement{ static_cast<elements::pneumatic::Tank *const>(m_element) };
                     tankElement->setInitialPressure(static_cast<float>(a_value));
                   });

  currentIndex = m_properties->rowCount();
  m_properties->insertRow(currentIndex);

  item = new QTableWidgetItem{ "Volume" };
  item->setFlags(item->flags() & ~Qt::ItemIsEditable);
  m_properties->setItem(currentIndex, 0, item);

  QDoubleSpinBox *volumeValue = new QDoubleSpinBox;
  volumeValue->setRange(1.0, 10000.0);
  volumeValue->setValue(static_cast<qreal>(tank->volume()));
  m_properties->setCellWidget(currentIndex, 1, volumeValue);

  QObject::connect(volumeValue, static_cast<void (QDoubleSpinBox::*)(qreal)>(&QDoubleSpinBox::valueChanged),
                   [this](qreal a_value) {
                     auto const tankElement{ static_cast<elements::pneumatic::Tank *const>(m_element) };
                     tankElement->setVolume(static_cast<float>(a_value));
                   });
}

} // namespace spaghetti::nodes::pneumatic
