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

#include "nodes/ui/toggle_button.h"
#include "elements/ui/toggle_button.h"
#include "ui/colors.h"

#include <QCheckBox>
#include <QTableWidget>

namespace spaghetti::nodes::ui {

class ToggleButton;

class ToggleButtonWidget : public QGraphicsItem {
 public:
  QRectF boundingRect() const override { return m_boundingRect; }

  void mousePressEvent(QGraphicsSceneMouseEvent *a_event) override { (void)a_event; }

  void mouseReleaseEvent(QGraphicsSceneMouseEvent *a_event) override
  {
    (void)a_event;
    m_state = !m_state;
    m_toggleButton->set(m_state);
  }

  void paint(QPainter *a_painter, QStyleOptionGraphicsItem const *a_option, QWidget *a_widget) override
  {
    (void)a_option;
    (void)a_widget;

    auto innerRect = m_boundingRect.adjusted(5.0, 10.0, -5.0, -20.0).translated(0.0, 5.0);
    QBrush brush{ (m_state ? QColor{ 203, 217, 81 } : QColor{ 244, 53, 64 }) };
    QPen pen{ Qt::black };
    a_painter->setPen(pen);
    a_painter->setBrush(brush);
    a_painter->drawRect(innerRect);

    QSizeF const toggleSize{ 30.0, m_boundingRect.height() };
    qreal const toggleX{ (m_state ? m_boundingRect.right() - toggleSize.width() : m_boundingRect.left()) };
    QRectF const toggleRect{ QPointF{ toggleX, m_boundingRect.top() }, toggleSize };

    //    brush.setColor(QColor{ 120,  83,  74, 255 });
    brush.setColor(Qt::lightGray);
    pen.setColor(Qt::black);
    a_painter->setPen(pen);
    a_painter->setBrush(brush);
    a_painter->drawRect(toggleRect);
  }

  void setToggleButton(elements::ui::ToggleButton *const a_toggleButton) { m_toggleButton = a_toggleButton; }

 private:
  bool m_state{};
  QRectF m_boundingRect{ 0, 0, 80, 20 };
  elements::ui::ToggleButton *m_toggleButton{};
};

ToggleButton::ToggleButton()
{
  auto const widget = new ToggleButtonWidget;
  setCentralWidget(widget);
}

void ToggleButton::paint(QPainter *a_painter, QStyleOptionGraphicsItem const *a_option, QWidget *a_widget)
{
  (void)a_option;
  (void)a_widget;

  paintBorder(a_painter);
}

void ToggleButton::showProperties()
{
  showCommonProperties();
  showIOProperties(IOSocketsType::eOutputs);

  propertiesInsertTitle("Toggle Button");

  int row = m_properties->rowCount();
  m_properties->insertRow(row);

  QTableWidgetItem *item{};

  item = new QTableWidgetItem{ "Value" };
  item->setFlags(item->flags() & ~Qt::ItemIsEditable);
  m_properties->setItem(row, 0, item);

  auto const element = static_cast<elements::ui::ToggleButton *>(m_element);
  bool const current = element->currentValue();

  QCheckBox *const value = new QCheckBox;
  m_properties->setCellWidget(row, 1, value);
  value->setChecked(current);

  QObject::connect(value, &QCheckBox::stateChanged, [element](int a_state) { element->set(a_state == 2); });
}

void ToggleButton::elementSet()
{
  auto const widget = static_cast<ToggleButtonWidget *>(m_centralWidget);
  widget->setToggleButton(static_cast<elements::ui::ToggleButton *>(m_element));
}

} // namespace spaghetti::nodes::ui
