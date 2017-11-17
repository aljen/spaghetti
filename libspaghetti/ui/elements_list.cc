#include "ui/elements_list.h"

#include "ui/editor.h"
#include "ui/package_view.h"

#include <QDebug>
#include <QDrag>
#include <QMimeData>
#include <cassert>

ElementsList::ElementsList(Editor *const a_parent)
  : QListWidget{}
  , m_editor{ a_parent }
{
  assert(a_parent);
  setDragEnabled(true);
  setViewMode(QListView::ListMode);
  setIconSize(QSize(50, 25));
  setSpacing(5);
}

void ElementsList::startDrag(Qt::DropActions a_supportedActions)
{
  (void)a_supportedActions;

  PackageView *const packageView{ m_editor->packageView() };
  if (!packageView) {
    qDebug() << "There is no active package view!";
    return;
  }

  QListWidgetItem *const item{ currentItem() };
  QDrag *const drag{ new QDrag{ this } };

  auto const type = item->data(ElementsList::eMetaDataType).toString();
  auto const name = item->data(ElementsList::eMetaDataName).toByteArray();
  auto const icon = item->data(ElementsList::eMetaDataIcon).toByteArray();

  QMimeData *const mimeData{ new QMimeData };
  mimeData->setText(type);
  mimeData->setData("metadata/name", name);
  mimeData->setData("metadata/icon", icon);
  drag->setMimeData(mimeData);

  auto const ret = drag->exec(Qt::CopyAction);
  switch (ret) {
    case Qt::CopyAction: qDebug() << "Item" << type << "added!"; break;
    default: qDebug() << Q_FUNC_INFO << "ret:" << ret; break;
  }
}
