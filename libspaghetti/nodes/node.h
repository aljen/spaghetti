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

#include "core/api.h"
#include "elements/element.h"
#include "ui/socket_item.h"

class PackageView;
class QTableWidget;

namespace nodes {

constexpr int NODE_TYPE{ QGraphicsItem::UserType + 1 };

class SPAGHETTI_API Node : public QGraphicsItem {
 public:
  using Sockets = QVector<SocketItem *>;

  enum AllowedValueType {
    eBoolType = 1 << 0,
    eIntType = 1 << 1,
    eFloatType = 1 << 2,
    eAnyType = eBoolType | eIntType | eFloatType
  };

  explicit Node(QGraphicsItem *a_parent = nullptr);
  ~Node() override;

  int type() const override { return NODE_TYPE; }
  QRectF boundingRect() const override;
  void paint(QPainter *a_painter, QStyleOptionGraphicsItem const *a_option, QWidget *a_widget) override;
  QVariant itemChange(GraphicsItemChange a_change, QVariant const &a_value) override;
  void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *a_event) override;
  void advance(int a_phase) override;

  enum class Type { eElement, eInputs, eOutputs };
  using SocketType = SocketItem::Type;
  using ValueType = elements::Element::ValueType;

  void setType(Type const a_type) { m_type = a_type; }
  void setPackageView(PackageView *const a_packageView) { m_packageView = a_packageView; }

  void setElement(elements::Element *const a_element);

  void setName(QString a_name);
  QString name() const { return m_name; }

  void setPath(QString a_path) { m_path = a_path; }
  QString path() const { return m_path; }

  void setIcon(QString a_icon);
  QPixmap icon() const { return m_icon; }
  QString iconPath() const { return m_iconPath; }

  void iconify();
  void expand();

  elements::Element *element() const { return m_element; }

  Sockets const &inputs() const { return m_inputs; }
  Sockets const &outputs() const { return m_outputs; }

  void setPropertiesTable(QTableWidget *const a_properties);

  void paintBorder(QPainter *const a_painter);
  void paintIcon(QPainter *const a_painter);

  virtual void showProperties();
  virtual void refreshCentralWidget() {}
  virtual void elementSet() {}

  void showCommonProperties();
  void showInputsProperties(int a_allowedTypes = eAnyType);
  void showOutputsProperties(int a_allowedTypes = eAnyType);

 protected:
  void setCentralWidget(QGraphicsItem *a_centralWidget);
  void propertiesInsertTitle(QString a_title);
  void calculateBoundingRect();

 private:
  void addInput(ValueType const a_type);
  void removeInput();
  void setInputName(uint8_t a_socketId, QString const a_name);
  void setInputType(uint8_t a_socketId, ValueType const a_type);

  void addOutput(ValueType const a_type);
  void removeOutput();
  void setOutputName(uint8_t a_socketId, QString const a_name);
  void setOutputType(uint8_t a_socketId, ValueType const a_type);

  void addSocket(SocketType const a_type, uint8_t const a_id, QString const a_name, ValueType const a_valueType);
  void removeSocket(SocketType const a_type);
  void setOutputs(elements::Element *const a_element);

 protected:
  QGraphicsItem *m_centralWidget{};
  QTableWidget *m_properties{};
  elements::Element *m_element{};
  PackageView *m_packageView{};

 private:
  enum class Mode { eIconified, eExpanded } m_mode{};
  Type m_type{};
  QString m_name{};
  QString m_path{};
  QString m_iconPath{};
  QPixmap m_icon{};
  QRectF m_boundingRect{};
  QPointF m_centralWidgetPosition{};

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
