#ifndef EDITOR_H
#define EDITOR_H

#include <QMainWindow>
#include <QMap>

#include "core/metadata.h"

namespace Ui {
class Editor;
}

class QGraphicsItem;
class PackageView;

class Editor : public QMainWindow {
  Q_OBJECT

 public:
  explicit Editor(QWidget *a_parent = nullptr);
  ~Editor();

  void new_package();
  void newPackage(bool a_checked = false);
  void tabCloseRequested(int a_index);
  void tabChanged(int a_index);

  void populateElementsList();
  void addElement(QString a_category, core::MetaData a_metaData);

  void aboutToQuit();
  void itemMoved(QGraphicsItem *const a_item);

  PackageView *currentPackageView() const { return m_currentPackageView; }

 private:
  Ui::Editor *m_ui{};
  PackageView *m_currentPackageView{};
};

#endif // EDITOR_H
