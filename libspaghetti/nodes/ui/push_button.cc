#include "nodes/ui/push_button.h"

#include "elements/ui/push_button.h"

#include <QCheckBox>
#include <QTableWidget>
#include <QPushButton>
#include <QGraphicsProxyWidget>

namespace nodes::ui {

class PushButton;

class PushButtonWidget : public QGraphicsItem {
 public:
  QRectF boundingRect() const override { return m_boundingRect; }

  void mousePressEvent(QGraphicsSceneMouseEvent *a_event) override
  {
    (void)a_event;
    m_state = true;
    m_pushButton->set(m_state);
  }

  void mouseReleaseEvent(QGraphicsSceneMouseEvent *a_event) override
  {
    (void)a_event;
    m_state = false;
    m_pushButton->set(m_state);
  }

  void paint(QPainter *a_painter, QStyleOptionGraphicsItem const *a_option, QWidget *a_widget) override
  {
    (void)a_option;
    (void)a_widget;

    QBrush brush{ (m_state ? Qt::green : Qt::red) };
    a_painter->setPen(Qt::NoPen);
    a_painter->setBrush(brush);
    a_painter->drawRect(m_boundingRect);
  }

  void setPushButton(elements::ui::PushButton *const a_pushButton)
  {
    m_pushButton = a_pushButton;
  }

 private:
  bool m_state{};
  QRectF m_boundingRect{ -20, -10, 40, 20 };
  elements::ui::PushButton *m_pushButton{};
};

PushButton::PushButton()
{
  auto const widget = new PushButtonWidget;
  setCentralWidget(widget);

  auto const pushButton = new QPushButton{ "Test" };
  auto const pushButtonProxy = new QGraphicsProxyWidget{ this };
  pushButtonProxy->setWidget(pushButton);
}

void PushButton::paint(QPainter *a_painter, const QStyleOptionGraphicsItem *a_option, QWidget *a_widget)
{
  (void)a_option;
  (void)a_widget;

  paintBorder(a_painter);
}

void PushButton::showProperties()
{
  showCommonProperties();
  showOutputsProperties();

  propertiesInsertTitle("Push Button");

  int row = m_properties->rowCount();
  m_properties->insertRow(row);

  QTableWidgetItem *item{};

  item = new QTableWidgetItem{ "Value" };
  item->setFlags(item->flags() & ~Qt::ItemIsEditable);
  m_properties->setItem(row, 0, item);

  auto const element = static_cast<elements::ui::PushButton *const>(m_element);
  bool const current = element->currentValue();

  QCheckBox *const value = new QCheckBox;
  m_properties->setCellWidget(row, 1, value);
  value->setChecked(current);

  QObject::connect(value, &QCheckBox::stateChanged, [element](int a_state) { element->set(a_state == 2); });
}

void PushButton::elementSet()
{
  auto const pushButtonWidget = static_cast<PushButtonWidget *const>(m_centralWidget);
  pushButtonWidget->setPushButton(static_cast<elements::ui::PushButton *const>(m_element));
}

} // namespace nodes::ui
