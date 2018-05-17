// MIT License
//
// Copyright (c) 2017-2018 Artur Wyszyński, aljen at hitomi dot pl
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
#ifndef SPAGHETTI_EDITOR_H
#define SPAGHETTI_EDITOR_H

#include <QMainWindow>
#include <QMap>

#include <spaghetti/api.h>

namespace Ui {
class Editor;
}

class QGraphicsItem;
class QListView;
class QTableWidget;

namespace spaghetti {

class Package;
class PackageView;

class SPAGHETTI_API Editor final : public QMainWindow {
  Q_OBJECT

 public:
  explicit Editor(QWidget *const a_parent = nullptr);
  ~Editor() override;

  void tabCloseRequested(int const a_index);
  void tabChanged(int const a_index);

  void populateLibrary();
  void addElement(QString const &a_category, QString const &a_name, QString const &a_type, QString const &a_icon);
  void addPackage(QString const &a_category, QString const &a_filename, QString const &a_path, QString const &a_icon);

  void aboutToQuit();

  void openPackageFile(QString const &a_filename);
  void openOrCreatePackageView(Package *const a_package);
  PackageView *packageView() const { return packageViewForIndex(m_packageViewIndex); }
  int packageViewIndex() const { return m_packageViewIndex; }
  int indexForPackageView(PackageView *const a_packageView) const;

  QListView* elementsList();
  QTableWidget* propertiesTable();

 protected:
  void showEvent(QShowEvent *a_event) override;

  PackageView *packageViewForIndex(int const a_index = -1) const;
  int openPackageViews() const;

  void newPackage();
  void openPackage();
  void savePackage();
  void saveAsPackage();
  void closePackage();
  void closeAllPackages();

  void savePackageView(bool const a_saveAs);
  void closePackageView(int const a_index);

  void deleteElement();

  void showLibrary(bool a_checked);
  void showProperties(bool a_checked);

  void buildCommit();
  void recentChanges();
  void about();
  void aboutQt();

 private:
  Ui::Editor *const m_ui{};
  int m_packageViewIndex{ -1 };
  QMap<QString, int> m_openFiles{};
  QMap<Package*, int> m_openPackages{};
};

} // namespace spaghetti

#endif // SPAGHETTI_EDITOR_H
