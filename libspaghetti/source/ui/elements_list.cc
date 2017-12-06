// MIT License
//
// Copyright (c) 2017 Artur Wyszyński, aljen at hitomi dot pl
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

#include "ui/elements_list.h"

#include "spaghetti/editor.h"
#include "ui/package_view.h"

#include <QDebug>
#include <QDrag>
#include <QMimeData>
#include <cassert>

namespace spaghetti {

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

} // namespace spaghetti
