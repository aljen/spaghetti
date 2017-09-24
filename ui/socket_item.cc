#include "ui/socket_item.h"

#include <QApplication>
#include <QCursor>
#include <QDebug>
#include <QDrag>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsWidget>
#include <QMimeData>
#include <QWidget>

#include "elements/package.h"
#include "ui/config.h"
#include "ui/link_item.h"
#include "ui/nodes_view.h"
#include "ui/package_view.h"

SocketItem::SocketItem(Type a_type, QGraphicsItem *const a_parent)
  : QGraphicsItem{ a_parent }
  , m_type{ a_type }
{
  m_font.setFamily("Consolas");
  m_font.setPointSize(12);

  setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsFocusable |
           QGraphicsItem::ItemSendsScenePositionChanges);
  setAcceptHoverEvents(true);
  setAcceptedMouseButtons(Qt::LeftButton);

  setZValue(1);

  if (a_type == Type::eOutput)
    setCursor(Qt::OpenHandCursor);
  else
    setAcceptDrops(true);
}

QRectF SocketItem::boundingRect() const
{
  return QRectF{ -(static_cast<qreal>(SIZE) / 2.), 0, static_cast<qreal>(SIZE), static_cast<qreal>(SIZE) };
}

void SocketItem::paint(QPainter *a_painter, const QStyleOptionGraphicsItem *a_option, QWidget *a_widget)
{
  Q_UNUSED(a_option);
  Q_UNUSED(a_widget);

  QRectF const rect{ boundingRect() };
  Config const &config{ Config::get() };

  QPen pen{ config.getColor(Config::Color::eSocketBorder) };
  pen.setWidth(2);

  QBrush brush{};
  if (m_isHover)
    brush.setColor(config.getColor(Config::Color::eSocketHover));
  else if (m_isDrop)
    brush.setColor(config.getColor(Config::Color::eSocketDrop));
  else if (m_isSignalOn)
    brush.setColor(m_colorSignalOn);
  else if (!m_isSignalOn)
    brush.setColor(m_colorSignalOff);
  //  else if (m_type == Type::eInput)
  //    brush.setColor(config.getColor(Config::Color::eSocketInput));
  //  else if (m_type == Type::eOutput)
  //    brush.setColor(config.getColor(Config::Color::eSocketOutput));
  brush.setStyle(Qt::SolidPattern);

  a_painter->setPen(pen);
  a_painter->setBrush(brush);
  a_painter->drawEllipse(rect);

  if (m_used) {
    a_painter->save();
    a_painter->setPen(Qt::NoPen);
    a_painter->setBrush(pen.color());
    a_painter->drawEllipse(rect.adjusted(4, 4, -4, -4));
    a_painter->restore();
  }

  if (!m_nameHidden) {
    pen.setColor(config.getColor(Config::Color::eFontName));
    a_painter->setPen(pen);
    QFont font{ a_painter->font() };
    font.setPointSize(12);
    a_painter->setFont(font);

    QFontMetrics const metrics{ font };

    if (m_type == Type::eInput)
      a_painter->drawText(static_cast<int>(rect.width()) - 2, 13, m_name);
    else
      a_painter->drawText(-metrics.width(m_name) - 14, 13, m_name);
  }
}

void SocketItem::hoverEnterEvent(QGraphicsSceneHoverEvent *a_event)
{
  Q_UNUSED(a_event);

  m_isHover = true;

  for (auto link : m_links) link->setHover(m_isHover);

  update();
}

void SocketItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *a_event)
{
  Q_UNUSED(a_event);

  m_isHover = false;

  for (auto link : m_links) link->setHover(m_isHover);

  update();
}

void SocketItem::dragEnterEvent(QGraphicsSceneDragDropEvent *a_event)
{
  Q_UNUSED(a_event);

  if (m_used) {
    a_event->ignore();
    return;
  }

  NodesView *view{ reinterpret_cast<NodesView *>(scene()->views()[0]) };

  LinkItem *const linkItem{ view->dragLink() };
  if (!linkItem) return;

  linkItem->setTo(this);
  m_isDrop = true;

  update();
}

void SocketItem::dragLeaveEvent(QGraphicsSceneDragDropEvent *a_event)
{
  Q_UNUSED(a_event);

  m_isDrop = false;

  NodesView *view{ reinterpret_cast<NodesView *>(scene()->views()[0]) };

  LinkItem *const linkItem{ view->dragLink() };
  if (!linkItem) return;
  linkItem->setTo(nullptr);

  update();
}

void SocketItem::dragMoveEvent(QGraphicsSceneDragDropEvent *a_event)
{
  Q_UNUSED(a_event);
}

void SocketItem::dropEvent(QGraphicsSceneDragDropEvent *a_event)
{
  Q_UNUSED(a_event);

  NodesView *view{ reinterpret_cast<NodesView *>(scene()->views()[0]) };

  LinkItem *const linkItem{ view->dragLink() };
  if (!linkItem) return;

  m_links.push_back(linkItem);
  linkItem->setTo(this);
  m_used = true;
  m_isDrop = false;

  view->acceptDragLink();

  auto *const packageView{ view->packageView() };
  elements::Package *const package{ packageView->package() };

  auto const *const from = linkItem->from();

  package->connect(from->elementId(), from->socketId(), m_elementId, m_socketId);

  update();
}

void SocketItem::mousePressEvent(QGraphicsSceneMouseEvent *a_event)
{
  Q_UNUSED(a_event);

  if (m_type == Type::eInput) return;

  setCursor(Qt::ClosedHandCursor);
}

void SocketItem::mouseMoveEvent(QGraphicsSceneMouseEvent *a_event)
{
  Q_UNUSED(a_event);

  if (m_type == Type::eInput) return;

  if (QLineF(a_event->screenPos(), a_event->buttonDownScreenPos(Qt::LeftButton)).length() <
      QApplication::startDragDistance())
    return;

  //  QRectF const rect{ boundingRect() };

  QDrag *const drag = new QDrag(a_event->widget());

  QMimeData *const mime = new QMimeData;
  //  mime->setData("data/x-element", QByteArray());
  drag->setMimeData(mime);

  LinkItem *linkItem{ new LinkItem };
  linkItem->setColors(m_colorSignalOff, m_colorSignalOn);
  linkItem->setFrom(this);

  scene()->addItem(linkItem);

  NodesView *view{ reinterpret_cast<NodesView *>(scene()->views()[0]) };
  view->setDragLink(linkItem);

  Qt::DropAction const action{ drag->exec() };
  if (action == Qt::IgnoreAction)
    view->cancelDragLink();
  else {
    m_links.push_back(linkItem);
    m_used = true;
    update();
  }

  setCursor(Qt::OpenHandCursor);
}

void SocketItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *a_event)
{
  Q_UNUSED(a_event);

  if (m_type == Type::eInput) return;

  setCursor(Qt::OpenHandCursor);
}

QVariant SocketItem::itemChange(QGraphicsItem::GraphicsItemChange a_change, const QVariant &a_value)
{
  if (a_change == QGraphicsItem::ItemScenePositionHasChanged) {
    for (LinkItem *const link : m_links) link->trackNodes();
  }
  return QGraphicsItem::itemChange(a_change, a_value);
}

int SocketItem::nameWidth() const
{
  QFontMetrics const metrics{ m_font };
  return metrics.width(m_name);
}

void SocketItem::setColors(QColor const a_signalOff, QColor const a_signalOn)
{
  m_colorSignalOff = a_signalOff;
  m_colorSignalOn = a_signalOn;
}

void SocketItem::setSignal(const bool a_signal)
{
  m_isSignalOn = a_signal;
  update();

  if (m_type == Type::eOutput) {
    for (LinkItem *const link : m_links) link->setSignal(a_signal);
  }
}
