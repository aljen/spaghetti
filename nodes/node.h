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
#ifndef NODES_NODE_H
#define NODES_NODE_H

#include <QGraphicsItem>
#include <QPainter>
#include <QVector>

#include "elements/element.h"
#include "ui/socket_item.h"

class PackageView;

namespace nodes {

constexpr int NODE_TYPE{ QGraphicsItem::UserType + 1 };

class Node : public QGraphicsItem {
 public:
  explicit Node(QGraphicsItem *a_parent = nullptr);
  ~Node() override;

  int type() const override { return NODE_TYPE; }
  QRectF boundingRect() const override;
  void paint(QPainter *a_painter, QStyleOptionGraphicsItem const *a_option, QWidget *a_widget) override;
  QVariant itemChange(GraphicsItemChange a_change, QVariant const &a_value) override;
  void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *a_event) override;

  enum class Type { eElement, eInputs, eOutputs };
  using SocketType = SocketItem::Type;
  using ValueType = elements::Element::ValueType;

  void setType(Type const a_type) { m_type = a_type; }
  void setPackageView(PackageView *const a_packageView) { m_packageView = a_packageView; }

  void setElement(elements::Element *const a_element);
  void setName(QString a_name) { m_name = a_name; }
  void setPath(QString a_path) { m_path = a_path; }
  void setIcon(QString a_icon);

  void iconify();
  void expand();

 private:
  void addSocket(SocketType const a_type, uint8_t const a_id, QString const a_name, ValueType const a_valueType);
  void calculateBoundingRect();

 private:
  enum class Mode { eIconified, eExpanded } m_mode{};
  Type m_type{};
  QString m_name{};
  QString m_path{};
  QPixmap m_icon{};
  QRectF m_boundingRect{};
  elements::Element *m_element{};
  PackageView *m_packageView{};

  using Sockets = QVector<SocketItem *>;
  Sockets m_inputs{};
  Sockets m_outputs{};
};

class Package : public Node {
};

namespace logic {

class Test : public Node {
};

} // namespace logic

} // namespace nodes

#endif // NODES_NODE_H
