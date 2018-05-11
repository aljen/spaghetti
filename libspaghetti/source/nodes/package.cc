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

#include <QDebug>
#include <QTableWidget>
#include <QLineEdit>

#include "nodes/package.h"
#include "spaghetti/editor.h"
#include "spaghetti/package.h"
#include "ui/package_view.h"

namespace spaghetti::nodes {

Package::Package() {}

void Package::showProperties()
{
  showCommonProperties();

  auto const package = static_cast<spaghetti::Package *>(m_element);
  auto const PATH = QString::fromStdString(std::string(package->packagePath()));
  auto const ICON = QString::fromStdString(std::string(package->packageIcon()));

  propertiesInsertTitle("Package");

  QTableWidgetItem *item{};

  int row = m_properties->rowCount();
  m_properties->insertRow(row);
  item = new QTableWidgetItem{ "Path" };
  item->setFlags(item->flags() & ~Qt::ItemIsEditable);
  m_properties->setItem(row, 0, item);

  QLineEdit *pathEdit = new QLineEdit{ PATH };
  pathEdit->setPlaceholderText("<path>");
  m_properties->setCellWidget(row, 1, pathEdit);
  QObject::connect(pathEdit, &QLineEdit::textChanged, [this](QString const &a_text) {
    (void)a_text;
    qDebug() << "PATH:" << a_text;
    /* TODO */
  });

  row = m_properties->rowCount();
  m_properties->insertRow(row);
  item = new QTableWidgetItem{ "Icon" };
  item->setFlags(item->flags() & ~Qt::ItemIsEditable);
  m_properties->setItem(row, 0, item);

  QLineEdit *iconEdit = new QLineEdit{ ICON };
  iconEdit->setPlaceholderText("<icon>");
  m_properties->setCellWidget(row, 1, iconEdit);
  QObject::connect(iconEdit, &QLineEdit::textChanged, [this](QString const &a_text)  {
    (void)a_text;
    qDebug() << "ICON:" << a_text;
    /* TODO */
  });

  showIOProperties(IOSocketsType::eInputs);
  showIOProperties(IOSocketsType::eOutputs);
}

bool Package::open()
{
  qDebug() << Q_FUNC_INFO;

  auto const editor = m_packageView->editor();
  editor->openOrCreatePackageView(static_cast<spaghetti::Package *>(m_element));

  return true;
}

} // namespace spaghetti::nodes
