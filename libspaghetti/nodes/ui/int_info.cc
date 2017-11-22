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

#include "nodes/ui/int_info.h"
#include "elements/ui/int_info.h"

#include <QGraphicsSimpleTextItem>
#include <QTableWidget>

namespace nodes::ui {

IntInfo::IntInfo()
{
  QFont font{};
  font.setPixelSize(32);
  auto widget = new QGraphicsSimpleTextItem("0");
  widget->setFont(font);
  QPointF widgetPosition{};
  widgetPosition.rx() = -(widget->boundingRect().width() / 2.0);
  widgetPosition.ry() = -(widget->boundingRect().height() / 2.0);
  widget->setPos(widgetPosition);
  setCentralWidget(widget);

  m_info = widget;
}

void IntInfo::refreshCentralWidget()
{
  if (!m_element || !m_element->inputs()[0].value) return;
  int32_t const value{ std::get<int32_t>(*m_element->inputs()[0].value) };
  m_info->setText(QString::number(value));
}

void IntInfo::showProperties()
{
  showCommonProperties();
  showInputsProperties();
  showOutputsProperties();
}

} // namespace nodes::ui
