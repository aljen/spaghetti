#ifndef UI_PACKAGE_VIEW_H
#define UI_PACKAGE_VIEW_H

#include <QWidget>

namespace elements {
class Package;
} // namespace elements

class NodesView;

class PackageView : public QWidget {
  Q_OBJECT

 public:
  explicit PackageView(elements::Package *const a_package = nullptr);
  ~PackageView();

  elements::Package const *package() const { return m_package; }
  elements::Package *package() { return m_package; }

  bool canClose();

 private:
  elements::Package *const m_package{};
  NodesView *const m_nodesView{};
  bool m_standalone{};
};

#endif // UI_PACKAGE_VIEW_H
