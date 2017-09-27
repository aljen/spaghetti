#ifndef NODESVIEW_H
#define NODESVIEW_H

#include <QGraphicsView>

class AbstractNode;
class LinkItem;
class PackageView;

namespace nodes {
class Node;
} // namespace nodes

class NodesView : public QGraphicsView {
  Q_OBJECT

 public:
  NodesView(QGraphicsScene *const a_scene, PackageView *a_parent);
  ~NodesView() override;

  void dragEnterEvent(QDragEnterEvent *a_event) override;
  void dragLeaveEvent(QDragLeaveEvent *a_event) override;
  void dragMoveEvent(QDragMoveEvent *a_event) override;
  void dropEvent(QDropEvent *a_event) override;
  void keyPressEvent(QKeyEvent *a_event) override;
  void keyReleaseEvent(QKeyEvent *a_event) override;
  void wheelEvent(QWheelEvent *a_event) override;

  void mouseMoveEvent(QMouseEvent *a_event) override;

  LinkItem *dragLink() const { return m_dragLink; }
  void setDragLink(LinkItem *a_link);
  void acceptDragLink();
  void cancelDragLink();

  PackageView *packageView() const { return m_packageView; }
  bool snapToGrid() const { return m_snapToGrid; }

 private:
  void createGrid();
  void updateGrid(qreal const a_scale);

 private:
  PackageView *m_packageView{};
  nodes::Node *m_inputs{};
  nodes::Node *m_outputs{};
  nodes::Node *m_dragNode{};
  LinkItem *m_dragLink{};
  int32_t m_scheduledScalings{};
  enum class GridDensity { eLarge, eSmall } m_gridDensity{};
  QGraphicsItemGroup *const m_gridLarge{};
  QGraphicsItemGroup *const m_gridSmall{};
  bool m_snapToGrid{};
};

#endif // NODESVIEW_H
