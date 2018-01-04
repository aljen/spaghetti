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

#include "spaghetti/socket_item.h"

#include <QApplication>
#include <QCursor>
#include <QDebug>
#include <QDrag>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsWidget>
#include <QMimeData>
#include <QWidget>

#include "spaghetti/node.h"
#include "spaghetti/package.h"
#include "ui/colors.h"
#include "ui/link_item.h"
#include "ui/package_view.h"

namespace spaghetti {

SocketItem::SocketItem(Node *const a_node, Type const a_type)
  : QGraphicsItem{ a_node }
  , m_node{ a_node }
  , m_type{ a_type }
{
  m_font.setFamily("Consolas");
  m_font.setPointSize(10);

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
  return QRectF{ -(static_cast<qreal>(SIZE) / 2.), -(static_cast<qreal>(SIZE) / 2.), static_cast<qreal>(SIZE),
                 static_cast<qreal>(SIZE) };
}

void SocketItem::paint(QPainter *a_painter, const QStyleOptionGraphicsItem *a_option, QWidget *a_widget)
{
  Q_UNUSED(a_option);
  Q_UNUSED(a_widget);

  QRectF const rect{ boundingRect() };

  QPen pen{ get_color(Color::eSocketBorder) };
  pen.setWidth(2);

  QBrush brush{};
  if (m_isHover)
    brush.setColor(get_color(Color::eSocketHover));
  else if (m_isDrop)
    brush.setColor(get_color(Color::eSocketDrop));
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
  if (m_type == Type::eOutput)
    a_painter->drawEllipse(rect);
  else
    a_painter->drawRect(rect);

  if (m_used) {
    a_painter->save();
    a_painter->setPen(Qt::NoPen);
    a_painter->setBrush(pen.color());
    if (m_type == Type::eOutput)
      a_painter->drawEllipse(rect.adjusted(4, 4, -4, -4));
    else
      a_painter->drawRect(rect.adjusted(4, 4, -4, -4));
    a_painter->restore();
  }

  if (!m_nameHidden) {
    pen.setColor(get_color(Color::eFontName));
    a_painter->setPen(pen);
    a_painter->setFont(m_font);

    QFontMetrics const metrics{ m_font };
    int const FONT_HEIGHT = metrics.height();

    if (m_type == Type::eInput)
      a_painter->drawText(static_cast<int>(rect.width()) - 4, (FONT_HEIGHT / 2) - metrics.strikeOutPos(), m_name);
    else
      a_painter->drawText(-metrics.width(m_name) - SIZE + SIZE / 3, (FONT_HEIGHT / 2) - metrics.strikeOutPos(), m_name);
  }
}

void SocketItem::hoverEnterEvent(QGraphicsSceneHoverEvent *a_event)
{
  Q_UNUSED(a_event);

  m_isHover = true;

  for (auto const link : m_links) link->setHover(m_isHover);
}

void SocketItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *a_event)
{
  Q_UNUSED(a_event);

  m_isHover = false;

  for (auto const link : m_links) link->setHover(m_isHover);
}

void SocketItem::dragEnterEvent(QGraphicsSceneDragDropEvent *a_event)
{
  Q_UNUSED(a_event);

  if (m_used) {
    a_event->ignore();
    return;
  }

  PackageView *const view{ reinterpret_cast<PackageView *const>(scene()->views()[0]) };

  LinkItem *const linkItem{ view->dragLink() };
  if (!linkItem || m_valueType != linkItem->valueType()) {
    a_event->ignore();
    return;
  }

  linkItem->setTo(this);
  m_isDrop = true;
}

void SocketItem::dragLeaveEvent(QGraphicsSceneDragDropEvent *a_event)
{
  Q_UNUSED(a_event);

  m_isDrop = false;

  PackageView *const view{ reinterpret_cast<PackageView *const>(scene()->views()[0]) };

  LinkItem *const linkItem{ view->dragLink() };
  if (!linkItem) return;
  linkItem->setTo(nullptr);
}

void SocketItem::dragMoveEvent(QGraphicsSceneDragDropEvent *a_event)
{
  Q_UNUSED(a_event);
}

void SocketItem::dropEvent(QGraphicsSceneDragDropEvent *a_event)
{
  Q_UNUSED(a_event);

  PackageView *const packageView{ reinterpret_cast<PackageView *const>(scene()->views()[0]) };

  LinkItem *const linkItem{ packageView->dragLink() };
  if (!linkItem) return;

  m_links.push_back(linkItem);
  linkItem->setTo(this);
  m_used = true;
  m_isDrop = false;

  packageView->acceptDragLink();

  auto const package = packageView->package();
  auto const from = linkItem->from();

  package->connect(from->elementId(), from->socketId(), m_elementId, m_socketId);

  setSignal(linkItem->isSignalOn());
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

  QDrag *const drag = new QDrag(a_event->widget());

  QMimeData *const mime = new QMimeData;
  drag->setMimeData(mime);

  LinkItem *linkItem{ new LinkItem };
  linkItem->setColors(m_colorSignalOff, m_colorSignalOn);
  linkItem->setValueType(m_valueType);
  linkItem->setFrom(this);
  linkItem->setSignal(m_isSignalOn);

  scene()->addItem(linkItem);
  m_links.push_back(linkItem);

  PackageView *const view{ reinterpret_cast<PackageView *const>(scene()->views()[0]) };
  view->setDragLink(linkItem);

  Qt::DropAction const action{ drag->exec() };
  if (action == Qt::IgnoreAction) {
    removeLink(linkItem);
    scene()->removeItem(linkItem);
    view->cancelDragLink();
  } else
    m_used = true;

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

  if (m_type == Type::eOutput)
    for (LinkItem *const link : m_links) link->setSignal(a_signal);
}

void SocketItem::connect(SocketItem *const a_other)
{
  auto const linkItem = new LinkItem;
  linkItem->setColors(m_colorSignalOff, m_colorSignalOn);
  linkItem->setValueType(m_valueType);
  linkItem->setFrom(this);
  linkItem->setTo(a_other);

  m_links.push_back(linkItem);
  m_used = true;

  a_other->m_links.push_back(linkItem);
  a_other->m_used = true;
  a_other->m_isDrop = false;

  scene()->addItem(linkItem);
}

void SocketItem::disconnect(SocketItem *const a_other)
{
  auto const link = linkBetween(this, a_other);
  if (!link) return;

  auto const FROM_ID = elementId();
  auto const FROM_SOCKET_ID = socketId();
  auto const TO_ID = a_other->elementId();
  auto const TO_SOCKET_ID = a_other->socketId();

  auto const packageView = m_node->packageView();
  auto const package = packageView->package();

  package->disconnect(FROM_ID, FROM_SOCKET_ID, TO_ID, TO_SOCKET_ID);

  removeLink(link);
  a_other->removeLink(link);

  if (m_links.empty()) m_used = false;
  m_isHover = false;
  a_other->m_used = false;
  a_other->m_isHover = false;

  delete link;
}

void SocketItem::disconnectAll()
{
  if (m_type == Type::eInput)
    disconnectAllInputs();
  else
    disconnectAllOutputs();
}

void SocketItem::disconnectAllInputs()
{
  auto links = m_links;
  for (auto &link : links) {
    auto const from = link->from();
    from->disconnect(this);
  }
}

void SocketItem::disconnectAllOutputs()
{
  auto links = m_links;
  for (auto &link : links) disconnect(link->to());
}

void SocketItem::setValueType(ValueType const a_type)
{
  m_valueType = a_type;

  switch (m_valueType) {
    case ValueType::eBool: setColors(get_color(Color::eBoolSignalOff), get_color(Color::eBoolSignalOn)); break;
    case ValueType::eFloat: setColors(get_color(Color::eFloatSignalOn), get_color(Color::eFloatSignalOff)); break;
    case ValueType::eInt: setColors(get_color(Color::eIntegerSignalOn), get_color(Color::eIntegerSignalOn)); break;
  }
}

void SocketItem::removeLink(LinkItem *const a_linkItem)
{
  auto const it = std::remove(std::begin(m_links), std::end(m_links), a_linkItem);
  m_links.erase(it, std::end(m_links));
}

LinkItem *SocketItem::linkBetween(SocketItem *const a_from, SocketItem *const a_to) const
{
  auto const it = std::find_if(std::begin(m_links), std::end(m_links), [a_from, a_to](LinkItem *const a_link) {
    return a_link->from() == a_from && a_link->to() == a_to;
  });

  if (it == std::end(m_links)) return nullptr;

  return *it;
}

} // namespace spaghetti
