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
