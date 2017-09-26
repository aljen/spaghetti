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

  PackageView *currentPackageView() const { return m_currentPackageView; }

 protected:
  void showEvent(QShowEvent *a_event);

 private:
  Ui::Editor *m_ui{};
  PackageView *m_currentPackageView{};
};

#endif // EDITOR_H
