#ifndef UI_PACKAGE_VIEW_H
#define UI_PACKAGE_VIEW_H

#include <QGraphicsView>

namespace elements {
class Package;
} // namespace elements

namespace nodes {
class Node;
} // namespace nodes

class LinkItem;

class PackageView final : public QGraphicsView {
  Q_OBJECT

 public:
  explicit PackageView(elements::Package *const a_package = nullptr);
  ~PackageView() override;

  void dragEnterEvent(QDragEnterEvent *a_event) override;
  void dragLeaveEvent(QDragLeaveEvent *a_event) override;
  void dragMoveEvent(QDragMoveEvent *a_event) override;
  void dropEvent(QDropEvent *a_event) override;
  void keyPressEvent(QKeyEvent *a_event) override;
  void keyReleaseEvent(QKeyEvent *a_event) override;
  void wheelEvent(QWheelEvent *a_event) override;

  LinkItem *dragLink() const { return m_dragLink; }
  void setDragLink(LinkItem *a_link) { m_dragLink = a_link; }
  void acceptDragLink() { m_dragLink = nullptr; }
  void cancelDragLink();

  elements::Package const *package() const { return m_package; }
  elements::Package *package() { return m_package; }

  bool canClose();
  void center();
  bool snapToGrid() const { return m_snapToGrid; }

 private:
  void createGrid();
  void updateGrid(qreal const a_scale);

 private:
  elements::Package *const m_package{};
  QGraphicsScene *const m_scene{};
  nodes::Node *const m_inputs{};
  nodes::Node *const m_outputs{};
  nodes::Node *m_dragNode{};
  LinkItem *m_dragLink{};
  int32_t m_scheduledScalings{};
  enum class GridDensity { eLarge, eSmall } m_gridDensity{};
  QGraphicsItemGroup *const m_gridLarge{};
  QGraphicsItemGroup *const m_gridSmall{};
  bool m_snapToGrid{};
  bool m_standalone{};
};

#endif // UI_PACKAGE_VIEW_H
