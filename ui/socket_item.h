#ifndef SOCKETITEM_H
#define SOCKETITEM_H

#include <QPainter>
#include <QGraphicsItem>
#include <QVector>

class LinkItem;

class SocketItem : public QGraphicsItem {
 public:
  enum class Type {
    eInput,
    eOutput
  };

  constexpr static int SIZE{ 16 };

  SocketItem(Type aType, QGraphicsItem* aParent = nullptr);

  bool isInput() const{ return m_type == Type::eInput; }
  bool isOutput() const{ return m_type == Type::eOutput; }

  QRectF boundingRect() const override;

  void paint(QPainter* aPainter, QStyleOptionGraphicsItem const* aOption, QWidget* aWidget) override;

  void hoverEnterEvent(QGraphicsSceneHoverEvent* aEvent) override;
  void hoverLeaveEvent(QGraphicsSceneHoverEvent* aEvent) override;

  void dragEnterEvent(QGraphicsSceneDragDropEvent* aEvent) override;
  void dragLeaveEvent(QGraphicsSceneDragDropEvent* aEvent) override;
  void dragMoveEvent(QGraphicsSceneDragDropEvent* aEvent) override;
  void dropEvent(QGraphicsSceneDragDropEvent* aEvent) override;

  void mousePressEvent(QGraphicsSceneMouseEvent* aEvent) override;
  void mouseMoveEvent(QGraphicsSceneMouseEvent* aEvent) override;
  void mouseReleaseEvent(QGraphicsSceneMouseEvent* aEvent) override;

  QVariant itemChange(GraphicsItemChange aChange, const QVariant &aValue) override;

  void setHover(bool aHover) { m_isHover = aHover; update(); }

  QString name() const { return m_name; }
  void setName(QString aName) { m_name = aName; }

  void showName() { m_nameHidden = false; }
  void hideName() { m_nameHidden = true; }

  void markAsUsed() { m_used = true; update(); }

  int nameWidth() const;

  void setElementId(size_t const a_elementId) { m_elementId = a_elementId; }
  size_t elementId() const { return m_elementId; }
  void setSocketId(size_t const a_socketId) { m_socketId = a_socketId; }
  size_t socketId() const { return m_socketId; }

  void setColors(QColor const a_signalOff, QColor const a_signalOn);
  void setSignal(bool const a_signal);

 private:
  QString m_name{};
  QFont m_font{};

  size_t m_elementId{};
  size_t m_socketId{};

  QColor m_colorSignalOn{};
  QColor m_colorSignalOff{};
  bool m_isSignalOn{};

  Type m_type{};
  bool m_isHover{};
  bool m_isDrop{};
  bool m_used{};
  bool m_nameHidden{};

  QVector<LinkItem*> m_links{};
};

#endif // SOCKETITEM_H
