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

#pragma once
#ifndef UI_LINK_ITEM_H
#define UI_LINK_ITEM_H

#include <QGraphicsPathItem>
#include <QPainter>
#include "spaghetti/element.h"

class QGraphicsDropShadowEffect;

namespace spaghetti {

constexpr int LINK_TYPE{ QGraphicsItem::UserType + 2 };

class SocketItem;

class LinkItem final : public QGraphicsPathItem {
 public:
  LinkItem(QGraphicsItem *a_parent = nullptr);

  int type() const override { return LINK_TYPE; }

  QRectF boundingRect() const override;
  QPainterPath shape() const override;

  void paint(QPainter *a_painter, QStyleOptionGraphicsItem const *a_option, QWidget *a_widget) override;

  void hoverEnterEvent(QGraphicsSceneHoverEvent *a_event) override;
  void hoverLeaveEvent(QGraphicsSceneHoverEvent *a_event) override;

  void advance(int a_phase) override;

  SocketItem *from() const { return m_from; }
  SocketItem *to() const { return m_to; }
  void setFrom(SocketItem *const a_from);
  void setTo(SocketItem *const a_to);
  void setTo(QPointF const a_to);

  void setHover(bool const a_hover);
  void setColors(QColor const a_signalOff, QColor const a_signalOn);
  void setSignal(bool const a_signal);

  void setValueType(ValueType const a_type) { m_valueType = a_type; }
  ValueType valueType() const { return m_valueType; }

  bool isSnapped() const { return m_isSnapped; }
  bool isSignalOn() const { return m_isSignalOn; }

  void trackNodes();

 private:
  SocketItem *m_from{};
  SocketItem *m_to{};

  QRectF m_boundingRect{};
  QPainterPath m_path{};
  QPainterPath m_shape{};

  QColor m_colorSignalOn{};
  QColor m_colorSignalOff{};

  QPointF m_toPoint{};

  qreal m_dashOffset{};
  ValueType m_valueType{};
  bool m_isHover{};
  bool m_isSnapped{};
  bool m_isSignalOn{};
};

} // namespace spaghetti

#endif // UI_LINK_ITEM_H
