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

#pragma once
#ifndef UI_SOCKET_ITEM_H
#define UI_SOCKET_ITEM_H

#include <QGraphicsItem>
#include <QPainter>
#include <QVector>

#include "elements/element.h"

class LinkItem;

namespace nodes {
class Node;
}

constexpr int SOCKET_TYPE{ QGraphicsItem::UserType + 3 };

class SocketItem final : public QGraphicsItem {
 public:
  enum class Type { eInput, eOutput };
  using ValueType = elements::Element::ValueType;

  constexpr static int SIZE{ 16 };

  SocketItem(Type aType, nodes::Node *const a_node);

  int type() const override { return SOCKET_TYPE; }

  bool isInput() const { return m_type == Type::eInput; }
  bool isOutput() const { return m_type == Type::eOutput; }

  QRectF boundingRect() const override;

  void paint(QPainter *aPainter, QStyleOptionGraphicsItem const *a_option, QWidget *a_widget) override;

  void hoverEnterEvent(QGraphicsSceneHoverEvent *a_event) override;
  void hoverLeaveEvent(QGraphicsSceneHoverEvent *a_event) override;

  void dragEnterEvent(QGraphicsSceneDragDropEvent *a_event) override;
  void dragLeaveEvent(QGraphicsSceneDragDropEvent *a_event) override;
  void dragMoveEvent(QGraphicsSceneDragDropEvent *a_event) override;
  void dropEvent(QGraphicsSceneDragDropEvent *a_event) override;

  void mousePressEvent(QGraphicsSceneMouseEvent *a_event) override;
  void mouseMoveEvent(QGraphicsSceneMouseEvent *a_event) override;
  void mouseReleaseEvent(QGraphicsSceneMouseEvent *a_event) override;

  QVariant itemChange(GraphicsItemChange a_change, QVariant const &a_value) override;
//  void advance(int a_phase) override;

  void setHover(bool a_hover) { m_isHover = a_hover; }

  QString name() const { return m_name; }
  void setName(QString a_name) { m_name = a_name; }

  void showName() { m_nameHidden = false; }
  void hideName() { m_nameHidden = true; }

  void markAsUsed() { m_used = true; }

  int nameWidth() const;

  void setElementId(size_t const a_elementId) { m_elementId = a_elementId; }
  size_t elementId() const { return m_elementId; }
  void setSocketId(uint8_t const a_socketId) { m_socketId = a_socketId; }
  uint8_t socketId() const { return m_socketId; }

  void setColors(QColor const a_signalOff, QColor const a_signalOn);
  void setSignal(bool const a_signal);

  void connect(SocketItem *const a_other);

  void setCurrentValue(qreal const a_currentValue) { m_currentValue = a_currentValue; }
  qreal currentValue() const { return m_currentValue; }

  void setValueType(ValueType const a_type) { m_valueType = a_type; }
  ValueType valueType() const { return m_valueType; }

 private:
  nodes::Node *const m_node{};
  QString m_name{};
  QFont m_font{};

  qreal m_currentValue{};

  ValueType m_valueType{};

  size_t m_elementId{};
  uint8_t m_socketId{};

  QColor m_colorSignalOn{};
  QColor m_colorSignalOff{};
  bool m_isSignalOn{};

  Type m_type{};
  bool m_isHover{};
  bool m_isDrop{};
  bool m_used{};
  bool m_nameHidden{};

  QVector<LinkItem *> m_links{};
};

#endif // UI_SOCKET_ITEM_H
