#include "nodes/ui/toggle_button.h"

#include "elements/ui/toggle_button.h"

#include "ui/colors.h"

#include <QCheckBox>
#include <QTableWidget>

namespace nodes::ui {

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

    QBrush brush{ (m_state ? QColor{ 203, 217, 81 } : QColor{ 244, 53, 64 }) };
    a_painter->setPen(Qt::NoPen);
    a_painter->setBrush(brush);
    a_painter->drawRect(m_boundingRect);

    QSizeF const toggleSize{ 30.0, m_boundingRect.height() };
    qreal const toggleX{ (m_state ? m_boundingRect.right() - toggleSize.width() : m_boundingRect.left()) };
    QRectF const toggleRect{ QPointF{ toggleX, m_boundingRect.top() }, toggleSize };

//    brush.setColor(QColor{ 120,  83,  74, 255 });
    brush.setColor(Qt::black);
    a_painter->setBrush(brush);
    a_painter->drawRect(toggleRect);
  }

  void setToggleButton(elements::ui::ToggleButton *const a_toggleButton) { m_toggleButton = a_toggleButton; }

 private:
  bool m_state{};
  QRectF m_boundingRect{ -40, -10, 80, 20 };
  elements::ui::ToggleButton *m_toggleButton{};
};

ToggleButton::ToggleButton()
{
  auto const widget = new ToggleButtonWidget;
  setCentralWidget(widget);
}

void ToggleButton::paint(QPainter *a_painter, const QStyleOptionGraphicsItem *a_option, QWidget *a_widget)
{
  (void)a_option;
  (void)a_widget;

  paintBorder(a_painter);
}

void ToggleButton::showProperties()
{
  showCommonProperties();
  showOutputsProperties();

  propertiesInsertTitle("Toggle Button");

  int row = m_properties->rowCount();
  m_properties->insertRow(row);

  QTableWidgetItem *item{};

  item = new QTableWidgetItem{ "Value" };
  item->setFlags(item->flags() & ~Qt::ItemIsEditable);
  m_properties->setItem(row, 0, item);

  auto const element = static_cast<elements::ui::ToggleButton *const>(m_element);
  bool const current = element->currentValue();

  QCheckBox *const value = new QCheckBox;
  m_properties->setCellWidget(row, 1, value);
  value->setChecked(current);

  QObject::connect(value, &QCheckBox::stateChanged, [element](int a_state) { element->set(a_state == 2); });
}

void ToggleButton::elementSet()
{
  auto const widget = static_cast<ToggleButtonWidget *const>(m_centralWidget);
  widget->setToggleButton(static_cast<elements::ui::ToggleButton *const>(m_element));
}

} // namespace nodes::ui
