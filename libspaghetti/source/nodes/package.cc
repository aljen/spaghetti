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
#include <QLineEdit>
#include <QTableWidget>

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
  QObject::connect(iconEdit, &QLineEdit::textChanged, [this](QString const &a_text) {
    (void)a_text;
    qDebug() << "ICON:" << a_text;
    /* TODO */
  });

  showIOProperties(IOSocketsType::eInputs);
  showIOProperties(IOSocketsType::eOutputs);
}

void Package::handleEvent(Event const &a_event)
{
  Node::handleEvent(a_event);

  if (!(m_inputsNode && m_outputsNode)) return;

  qDebug() << Q_FUNC_INFO << "package_id:" << m_element->id();
  switch (a_event.type) {
    case EventType::eElementNameChanged: break;
    case EventType::eIONameChanged: {
      auto const &EVENT = std::get<EventIONameChanged>(a_event.payload);
      if (EVENT.input) {
        m_inputsNode->outputs()[EVENT.id]->setName(QString::fromStdString(EVENT.to));
        m_inputsNode->calculateBoundingRect();
      } else {
        m_outputsNode->inputs()[EVENT.id]->setName(QString::fromStdString(EVENT.to));
        m_outputsNode->calculateBoundingRect();
      }
      break;
    }
    case EventType::eIOTypeChanged: {
      auto const &EVENT = std::get<EventIOTypeChanged>(a_event.payload);
      if (EVENT.input)
        m_inputsNode->setSocketType(IOSocketsType::eInputs, EVENT.id, EVENT.to);
      else
        m_outputsNode->setSocketType(IOSocketsType::eOutputs, EVENT.id, EVENT.to);
      break;
    }
    case EventType::eInputAdded: {
      auto const &INPUTS = m_element->inputs();
      auto const INPUTS_SIZE = static_cast<int>(INPUTS.size());
      auto const &LAST_INPUT = INPUTS.back();
      auto const OUTPUTS_SIZE = m_inputsNode->outputs().size();
      auto const ADD_SOCKET_NEEDED = OUTPUTS_SIZE < INPUTS_SIZE;
      assert(ADD_SOCKET_NEEDED == true);
      m_inputsNode->addSocket(SocketType::eOutput, static_cast<uint8_t>(OUTPUTS_SIZE),
                              QString::fromStdString(LAST_INPUT.name), LAST_INPUT.type);
      m_inputsNode->calculateBoundingRect();
      break;
    }
    case EventType::eInputRemoved: {
      m_inputsNode->removeSocket(SocketType::eOutput);
      m_inputsNode->calculateBoundingRect();
      break;
    }
    case EventType::eOutputAdded: {
      auto const &OUTPUTS = m_element->outputs();
      auto const OUTPUTS_SIZE = static_cast<int>(OUTPUTS.size());
      auto const &LAST_OUTPUT = OUTPUTS.back();
      auto const INPUTS_SIZE = m_outputsNode->inputs().size();
      auto const ADD_SOCKET_NEEDED = INPUTS_SIZE < OUTPUTS_SIZE;
      assert(ADD_SOCKET_NEEDED == true);
      m_outputsNode->addSocket(SocketType::eInput, static_cast<uint8_t>(INPUTS_SIZE),
                               QString::fromStdString(LAST_OUTPUT.name), LAST_OUTPUT.type);
      m_outputsNode->calculateBoundingRect();
      break;
    }
    case EventType::eOutputRemoved: {
      m_outputsNode->removeSocket(SocketType::eInput);
      m_outputsNode->calculateBoundingRect();
      break;
    }
  }
}

bool Package::open()
{
  qDebug() << Q_FUNC_INFO;

  auto const editor = m_packageView->editor();
  editor->openOrCreatePackageView(static_cast<spaghetti::Package *>(m_element));

  return true;
}

} // namespace spaghetti::nodes
