#include "nodes/ui/float_info.h"

#include "elements/ui/float_info.h"

#include <QTableWidget>
#include <QGraphicsSimpleTextItem>

namespace nodes::ui {

FloatInfo::FloatInfo()
{
  QFont font{};
  font.setPixelSize(32);
  auto widget = new QGraphicsSimpleTextItem("0.0");
  widget->setFont(font);
  QPointF widgetPosition{};
  widgetPosition.rx() = -(widget->boundingRect().width() / 2.0);
  widgetPosition.ry() = -(widget->boundingRect().height() / 2.0);
  widget->setPos(widgetPosition);
  setCentralWidget(widget);

  m_info = widget;
}

void FloatInfo::refreshCentralWidget()
{
  if (!m_element || !m_element->inputs()[0].value) return;
  float const value{ std::get<float>(*m_element->inputs()[0].value) };
  m_info->setText(QString::number(value, 'f', 2));
}

void FloatInfo::showProperties()
{
  showCommonProperties();
  showInputsProperties();
  showOutputsProperties();
}

} // namespace nodes::ui
