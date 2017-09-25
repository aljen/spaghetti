#ifndef LINKITEM_H
#define LINKITEM_H

#include <QGraphicsPathItem>
#include <QPainter>

class SocketItem;
class QGraphicsDropShadowEffect;

constexpr int LINK_TYPE{ QGraphicsItem::UserType + 2 };

class LinkItem final : public QGraphicsPathItem {
 public:
  LinkItem(QGraphicsItem *a_parent = nullptr);

  int type() const override { return LINK_TYPE; }

  QRectF boundingRect() const override;
  QPainterPath shape() const override;

  void paint(QPainter *a_painter, QStyleOptionGraphicsItem const *a_option, QWidget *a_widget) override;

  void hoverEnterEvent(QGraphicsSceneHoverEvent *a_event) override;
  void hoverLeaveEvent(QGraphicsSceneHoverEvent *a_event) override;

  SocketItem const *from() const { return m_from; }
  void setFrom(SocketItem *const a_from);
  void setTo(SocketItem *const a_to);
  void setTo(QPointF const a_to);

  void setHover(bool const a_hover);
  void setColors(QColor const a_signalOff, QColor const a_signalOn);
  void setSignal(bool const a_signal);

  bool isSnapped() const { return m_isSnapped; }

  void trackNodes();

 private:
  QGraphicsDropShadowEffect *m_effect{};

  SocketItem *m_from{};
  SocketItem *m_to{};

  QRectF m_boundingRect{};
  QPainterPath m_path{};
  QPainterPath m_shape{};

  QColor m_colorSignalOn{};
  QColor m_colorSignalOff{};
  bool m_isSignalOn{};

  QPointF m_toPoint{};
  bool m_isHover{};
  bool m_isSnapped{};
};

#endif // LINKITEM_H
