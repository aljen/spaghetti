#ifndef NODES_NODE_H
#define NODES_NODE_H

#include <QGraphicsItem>
#include <QPainter>
#include <QVector>

#include "elements/element.h"

class SocketItem;

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

  void setElement(elements::Element *const a_element);
  void setName(QString a_name) { m_name = a_name; }
  void setPath(QString a_path) { m_path = a_path; }
  void setIcon(QString a_icon);

  void iconify();
  void expand();

 private:
  void addInput(uint8_t const a_id, elements::Element::Input const &a_input);
  void addOutput(uint8_t const a_id, elements::Element::Output const &a_output);
  void calculateBoundingRect();

 private:
  enum class Mode { eIconified, eExpanded } m_mode{};
  QString m_name{};
  QString m_path{};
  QPixmap m_icon{};
  QRectF m_boundingRect{};
  elements::Element *m_element{};

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
