#include "ui/socket_item.h"

#include <QApplication>
#include <QCursor>
#include <QDebug>
#include <QDrag>
#include <QGraphicsWidget>
#include <QWidget>
#include <QGraphicsSceneMouseEvent>
#include <QMimeData>

#include "elements/package.h"
#include "ui/nodes_view.h"
#include "ui/link_item.h"
#include "ui/config.h"
#include "ui/package_view.h"

SocketItem::SocketItem(Type aType, QGraphicsItem* aParent)
  : QGraphicsItem{ aParent }
  , m_type{ aType }
{
  m_font.setFamily("Consolas");
  m_font.setPointSize(12);

  setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsFocusable | QGraphicsItem::ItemSendsScenePositionChanges);
  setAcceptHoverEvents(true);
  setAcceptedMouseButtons(Qt::LeftButton);

  setZValue(1);

  if (aType == Type::eOutput)
    setCursor(Qt::OpenHandCursor);
  else
    setAcceptDrops(true);
}

QRectF SocketItem::boundingRect() const
{
  return QRectF{ -(static_cast<qreal>(SIZE) / 2.), 0, static_cast<qreal>(SIZE), static_cast<qreal>(SIZE) };
}

void SocketItem::paint(QPainter *aPainter, const QStyleOptionGraphicsItem *aOption, QWidget *aWidget)
{
  Q_UNUSED(aOption);
  Q_UNUSED(aWidget);

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

  aPainter->setPen(pen);
  aPainter->setBrush(brush);
  aPainter->drawEllipse(rect);

  if (m_used) {
    aPainter->save();
    aPainter->setPen(Qt::NoPen);
    aPainter->setBrush(pen.color());
    aPainter->drawEllipse(rect.adjusted(4, 4, -4, -4));
    aPainter->restore();
  }

  if (!m_nameHidden) {
    pen.setColor(config.getColor(Config::Color::eFontName));
    aPainter->setPen(pen);
    QFont font{ aPainter->font() };
    font.setPointSize(12);
    aPainter->setFont(font);

    QFontMetrics const metrics{ font };

    if (m_type == Type::eInput)
      aPainter->drawText(static_cast<int>(rect.width()) - 2, 13, m_name);
    else
      aPainter->drawText(-metrics.width(m_name) - 14, 13, m_name);
  }
}

void SocketItem::hoverEnterEvent(QGraphicsSceneHoverEvent *aEvent)
{
  Q_UNUSED(aEvent);

  m_isHover = true;

  for (auto link : m_links)
    link->setHover(m_isHover);

  update();
}

void SocketItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *aEvent)
{
  Q_UNUSED(aEvent);

  m_isHover = false;

  for (auto link : m_links)
    link->setHover(m_isHover);

  update();
}

void SocketItem::dragEnterEvent(QGraphicsSceneDragDropEvent *aEvent)
{
  Q_UNUSED(aEvent);

  if (m_used) {
    aEvent->ignore();
    return;
  }

  NodesView *view{ reinterpret_cast<NodesView*>(scene()->views()[0]) };

  LinkItem *const linkItem{ view->dragLink() };
  if (!linkItem) return;

  linkItem->setTo(this);
  m_isDrop = true;

  update();
}

void SocketItem::dragLeaveEvent(QGraphicsSceneDragDropEvent *aEvent)
{
  Q_UNUSED(aEvent);

  m_isDrop = false;

  NodesView *view{ reinterpret_cast<NodesView*>(scene()->views()[0]) };

  LinkItem *const linkItem{ view->dragLink() };
  if (!linkItem) return;
  linkItem->setTo(nullptr);

  update();
}

void SocketItem::dragMoveEvent(QGraphicsSceneDragDropEvent *aEvent)
{
  Q_UNUSED(aEvent);
}

void SocketItem::dropEvent(QGraphicsSceneDragDropEvent *aEvent)
{
  Q_UNUSED(aEvent);

  NodesView *view{ reinterpret_cast<NodesView*>(scene()->views()[0]) };

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

void SocketItem::mousePressEvent(QGraphicsSceneMouseEvent *aEvent)
{
  Q_UNUSED(aEvent);

  if (m_type == Type::eInput) return;

  setCursor(Qt::ClosedHandCursor);
}

void SocketItem::mouseMoveEvent(QGraphicsSceneMouseEvent *aEvent)
{
  Q_UNUSED(aEvent);

  if (m_type == Type::eInput) return;

  if (QLineF(aEvent->screenPos(), aEvent->buttonDownScreenPos(Qt::LeftButton)).length() < QApplication::startDragDistance())
    return;

//  QRectF const rect{ boundingRect() };

  QDrag *const drag = new QDrag(aEvent->widget());

  QMimeData *const mime = new QMimeData;
//  mime->setData("data/x-element", QByteArray());
  drag->setMimeData(mime);

  LinkItem *linkItem{ new LinkItem };
  linkItem->setColors(m_colorSignalOff, m_colorSignalOn);
  linkItem->setFrom(this);

  scene()->addItem(linkItem);

  NodesView *view{ reinterpret_cast<NodesView*>(scene()->views()[0]) };
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

void SocketItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *aEvent)
{
  Q_UNUSED(aEvent);

  if (m_type == Type::eInput) return;

  setCursor(Qt::OpenHandCursor);
}

QVariant SocketItem::itemChange(QGraphicsItem::GraphicsItemChange aChange, const QVariant &aValue)
{
  if (aChange == QGraphicsItem::ItemScenePositionHasChanged) {
    for (LinkItem *const link : m_links)
      link->trackNodes();
  }
  return QGraphicsItem::itemChange(aChange, aValue);
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
    for (LinkItem *const link : m_links)
      link->setSignal(a_signal);
  }
}
