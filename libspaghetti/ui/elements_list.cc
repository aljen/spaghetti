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

void ElementsList::doResize()
{
  doItemsLayout();
  setFixedHeight(qMax(contentsSize().height(), 1));
}

void ElementsList::startDrag(Qt::DropActions a_supportedActions)
{
  (void)a_supportedActions;

  auto const packageView = m_editor->packageView();
  if (!packageView) return;

  auto const item = currentItem();
  auto const type = item->data(ElementsList::eMetaDataType).toString();
  auto const name = item->data(ElementsList::eMetaDataName).toByteArray();
  auto const icon = item->data(ElementsList::eMetaDataIcon).toByteArray();

  auto const mimeData = new QMimeData;
  mimeData->setText(type);
  mimeData->setData("metadata/name", name);
  mimeData->setData("metadata/icon", icon);

  auto const drag = new QDrag{ this };
  drag->setMimeData(mimeData);
  drag->exec(Qt::CopyAction);
}
