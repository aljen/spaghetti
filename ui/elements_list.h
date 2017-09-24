#ifndef ELEMENTSLIST_H
#define ELEMENTSLIST_H

#include <QListWidget>

class Editor;

class ElementsList : public QListWidget {
  Q_OBJECT

 public:
  enum { eMetaDataPath = Qt::UserRole, eMetaDataName = Qt::UserRole + 1, eMetaDataIcon = Qt::UserRole + 2 };

  explicit ElementsList(Editor *const a_parent = nullptr);

  static QString elementsMimeType() { return QStringLiteral("data/x-element"); }

 protected:
  void dragEnterEvent(QDragEnterEvent *a_event) override;
  void dragMoveEvent(QDragMoveEvent *a_event) override;
  void dropEvent(QDropEvent *a_event) override;
  void startDrag(Qt::DropActions a_supportedActions) override;

 private:
  Editor *const m_editor{};
};

#endif // ELEMENTSLIST_H
