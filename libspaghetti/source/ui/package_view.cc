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

#include "ui/package_view.h"

#include <QDebug>
#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QDragMoveEvent>
#include <QGraphicsScene>
#include <QHeaderView>
#include <QMimeData>
#include <QTableWidget>
#include <QTimeLine>

#include "spaghetti/node.h"
#include "spaghetti/package.h"
#include "spaghetti/registry.h"
#include "ui/elements_list.h"
#include "ui/link_item.h"

namespace spaghetti {

PackageView::PackageView(QTableWidget *const a_properties, Package *const a_package)
  : QGraphicsView{ new QGraphicsScene }
  , m_properties{ a_properties }
  , m_package{ (a_package ? a_package : new Package) }
  , m_scene{ scene() }
  , m_inputs{ new Node }
  , m_outputs{ new Node }
  , m_packageNode{ Registry::get().createNode("logic/package") }
  , m_standalone{ !a_package }
{
  setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::HighQualityAntialiasing |
                 QPainter::SmoothPixmapTransform);
  setDragMode(QGraphicsView::ScrollHandDrag);
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
  setResizeAnchor(QGraphicsView::NoAnchor);
  setTransformationAnchor(QGraphicsView::AnchorViewCenter);
  setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
  setOptimizationFlags(QGraphicsView::DontSavePainterState | QGraphicsView::DontAdjustForAntialiasing);

  setObjectName("PackageView");

  m_scene->setItemIndexMethod(QGraphicsScene::NoIndex);
  m_scene->setSceneRect(-32000, -32000, 64000, 64000);
  m_scene->setObjectName("PackageViewScene");

  QBrush brush{ QColor(169, 169, 169, 32) };
  m_scene->setBackgroundBrush(brush);

  m_inputs->setPropertiesTable(m_properties);
  m_outputs->setPropertiesTable(m_properties);

  m_packageNode->setPropertiesTable(m_properties);
  m_packageNode->setElement(m_package);

  setAcceptDrops(true);

  using NodeType = Node::Type;
  m_inputs->setPos(-600.0, 0.0);
  m_inputs->setType(NodeType::eInputs);
  m_inputs->setElement(m_package);
  m_inputs->setIcon(":/logic/inputs.png");
  m_inputs->setPackageView(this);
  m_inputs->iconify();
  m_outputs->setPos(600.0, 0.0);
  m_outputs->setType(NodeType::eOutputs);
  m_outputs->setElement(m_package);
  m_outputs->setIcon(":/logic/outputs.png");
  m_outputs->setPackageView(this);
  m_outputs->iconify();

  Registry &registry{ Registry::get() };

  m_package->setInputsPosition(m_inputs->x(), m_inputs->y());
  m_package->setOutputsPosition(m_outputs->x(), m_outputs->y());
  m_package->setName(registry.elementName("logic/package"));

  m_scene->addItem(m_inputs);
  m_scene->addItem(m_outputs);

  m_timer.setInterval(1000 / 60);

  connect(&m_timer, &QTimer::timeout, [this]() { m_scene->advance(); });
  m_timer.start();

  m_package->startDispatchThread();
}

PackageView::~PackageView()
{
  m_timer.stop();
  if (m_standalone) {
    m_package->quitDispatchThread();
    delete m_package;
  }
}

void PackageView::open()
{
  m_package->open(m_filename.toStdString());

  auto const &inputsPosition = m_package->inputsPosition();
  auto const &outputsPosition = m_package->outputsPosition();
  m_inputs->setPos(inputsPosition.x, inputsPosition.y);
  m_outputs->setPos(outputsPosition.x, outputsPosition.y);

  Registry &registry{ Registry::get() };

  auto const &elements = m_package->elements();
  size_t const SIZE{ elements.size() };
  for (size_t i = 1; i < SIZE; ++i) {
    auto const element = elements[i];
    auto const node = registry.createNode(element->hash());
    auto const nodeName = QString::fromStdString(registry.elementName(element->hash()));
    auto const nodeIcon = QString::fromStdString(registry.elementIcon(element->hash()));
    auto const nodePath = QString::fromLocal8Bit(element->type());

    m_nodes[element->id()] = node;

    element->isIconified() ? node->iconify() : node->expand();
    node->setPackageView(this);
    node->setPropertiesTable(m_properties);
    node->setName(nodeName);
    node->setPath(nodePath);
    node->setIcon(nodeIcon);
    node->setPos(element->position().x, element->position().y);
    node->setElement(element);
    m_scene->addItem(node);
  }

  auto const &connections = m_package->connections();
  for (auto const &connection : connections) {
    auto const source = getNode(connection.from_id);
    auto const target = getNode(connection.to_id);
    auto const sourceSocket = source->outputs()[connection.from_socket];
    auto const targetSocket = target->inputs()[connection.to_socket];
    sourceSocket->connect(targetSocket);
  }
}

void PackageView::save()
{
  m_package->save(m_filename.toStdString());
}

void PackageView::dragEnterEvent(QDragEnterEvent *a_event)
{
  auto const mimeData = a_event->mimeData();

  if (mimeData->hasFormat("metadata/name") && mimeData->hasFormat("metadata/icon")) {
    auto const pathString = mimeData->text();
    auto const name = mimeData->data("metadata/name");
    auto const icon = mimeData->data("metadata/icon");
    auto const stringData = pathString.toLatin1();
    auto const path = stringData.data();

    auto const dropPosition = mapToScene(a_event->pos());

    Registry &registry{ Registry::get() };

    assert(m_dragNode == nullptr);
    m_dragNode = registry.createNode(path);
    m_dragNode->setPackageView(this);
    m_dragNode->setPropertiesTable(m_properties);
    m_dragNode->setName(name);
    m_dragNode->setPath(pathString);
    m_dragNode->setIcon(icon);
    m_dragNode->setPos(dropPosition);
    m_scene->addItem(m_dragNode);
    m_dragNode->calculateBoundingRect();
    a_event->accept();
  } else
    QGraphicsView::dragEnterEvent(a_event);
}

void PackageView::dragLeaveEvent(QDragLeaveEvent *)
{
  if (m_dragNode) {
    m_scene->removeItem(m_dragNode);
    delete m_dragNode;
    m_dragNode = nullptr;
  }
}

void PackageView::dragMoveEvent(QDragMoveEvent *a_event)
{
  auto const dropPosition = mapToScene(a_event->pos());
  if (m_dragNode)
    m_dragNode->setPos(dropPosition);
  else if (m_dragLink) {
    QGraphicsView::dragMoveEvent(a_event);
    m_dragLink->setTo(mapToScene(a_event->pos()));
  }
}

void PackageView::dropEvent(QDropEvent *a_event)
{
  auto const mimeData = a_event->mimeData();

  if (mimeData->hasFormat("text/uri-list")) {
    QString const FILENAME{ mimeData->text().trimmed() };
    QString const STRIPPED{ FILENAME.right(FILENAME.size() - static_cast<int>(strlen("file://"))) };
    emit requestOpenFile(STRIPPED);
    a_event->accept();
  } else if (mimeData->hasFormat("metadata/name") && mimeData->hasFormat("metadata/icon")) {
    auto const pathString = a_event->mimeData()->text();
    auto const stringData = pathString.toLatin1();
    char const *const path{ stringData.data() };

    auto const element = m_package->add(path);
    if (element->name().empty()) element->setName(m_dragNode->name().toStdString());
    m_dragNode->setElement(element);
    m_dragNode->iconify();

    m_nodes[element->id()] = m_dragNode;

    m_dragNode = nullptr;
  }

  QGraphicsView::dropEvent(a_event);
}

void PackageView::keyPressEvent(QKeyEvent *a_event)
{
  m_snapToGrid = a_event->modifiers() & Qt::ShiftModifier;
}

void PackageView::keyReleaseEvent(QKeyEvent *a_event)
{
  m_snapToGrid = a_event->modifiers() & Qt::ShiftModifier;

  switch (a_event->key()) {
    case Qt::Key_R:
      centerOn(0.0, 0.0);
      resetMatrix();
      updateGrid(matrix().m11());
      break;
    default: break;
  }

  auto const selected = m_scene->selectedItems();
  for (auto &&item : selected) {
    if (item->type() == NODE_TYPE) {
      //      qDebug() << "Node:" << item;
    }
  }
}

void PackageView::wheelEvent(QWheelEvent *a_event)
{
  int32_t const numDegrees{ a_event->delta() / 8 };
  int32_t const numSteps{ numDegrees / 15 };

  m_scheduledScalings += numSteps;

  if (m_scheduledScalings * numSteps < 0) m_scheduledScalings = numSteps;

  QTimeLine *const animation{ new QTimeLine{ 350, this } };
  animation->setUpdateInterval(20);

  connect(animation, &QTimeLine::valueChanged, [&](qreal) {
    qreal const factor{ 1.0 + static_cast<qreal>(m_scheduledScalings) / 300.0 };
    QMatrix temp{ matrix() };
    temp.scale(factor, factor);
    if (temp.m11() >= 0.2 && temp.m11() <= 4.0) scale(factor, factor);
  });
  connect(animation, &QTimeLine::finished, [&]() {
    if (m_scheduledScalings > 0)
      m_scheduledScalings--;
    else
      m_scheduledScalings++;
    if (sender()) sender()->deleteLater();
    qreal const realScale = matrix().m11();
    updateGrid(realScale);
  });

  animation->start();
}

void PackageView::drawBackground(QPainter *a_painter, QRectF const &a_rect)
{
  QPen penNormal{ QColor(156, 156, 156, 32) };
  QPen penAxis{ QColor(156, 156, 156, 128) };

  qreal const LEFT{ a_rect.left() };
  qreal const RIGHT{ a_rect.right() };
  qreal const TOP{ a_rect.top() };
  qreal const BOTTOM{ a_rect.bottom() };
  qreal const GRID_DENSITY{ (m_gridDensity == GridDensity::eSmall ? 100.0 : 10.0) };

  qreal const START_X{ std::round(LEFT / GRID_DENSITY) * GRID_DENSITY };
  qreal const START_Y{ std::round(TOP / GRID_DENSITY) * GRID_DENSITY };

  if (m_gridDensity == GridDensity::eSmall) {
    penAxis.setWidth(2);
    penNormal.setWidth(2);
  }

  for (qreal x = START_X; x < RIGHT; x += GRID_DENSITY) {
    QPen const PEN{ ((x >= -0.1 && x <= 0.1) ? penAxis : penNormal) };
    a_painter->setPen(PEN);
    a_painter->drawLine(QPointF{ x, TOP }, QPointF{ x, BOTTOM });
  }

  for (qreal y = START_Y; y < BOTTOM; y += GRID_DENSITY) {
    QPen const PEN{ ((y >= -0.1 && y <= 0.1) ? penAxis : penNormal) };
    a_painter->setPen(PEN);
    a_painter->drawLine(QPointF{ LEFT, y }, QPointF{ RIGHT, y });
  }
}

void PackageView::cancelDragLink()
{
  delete m_dragLink;
  m_dragLink = nullptr;
}

bool PackageView::canClose()
{
  return true;
}

void PackageView::center()
{
  centerOn(0.0, 0.0);
}

void PackageView::showProperties()
{
  m_properties->clear();
  m_properties->setColumnCount(2);
  m_properties->setHorizontalHeaderLabels(QString("Name;Value").split(";"));

  m_selectedNode->showProperties();
  m_properties->horizontalHeader()->setStretchLastSection(true);
}

void PackageView::deleteElement()
{
  auto selectedItems = m_scene->selectedItems();

  m_timer.stop();
  while (m_timer.isActive()) {
    // empty
  }

  for (auto &&item : selectedItems) {
    switch (item->type()) {
      case NODE_TYPE: {
        auto const node = reinterpret_cast<Node *const>(item);
        auto const ID = node->element()->id();
        m_nodes.remove(ID);

        if (node == m_selectedNode) setSelectedNode(nullptr);
        delete node;
        break;
      }
      case LINK_TYPE: {
        auto const link = reinterpret_cast<LinkItem *const>(item);
        auto const from = link->from();
        auto const to = link->to();

        from->disconnect(to);
        break;
      }
    }
  }

  m_timer.start();
  showProperties();
}

void PackageView::setSelectedNode(Node *const a_node)
{
  if (a_node == nullptr)
    m_selectedNode = m_packageNode;
  else
    m_selectedNode = a_node;
}

void PackageView::updateGrid(qreal const a_scale)
{
  GridDensity const newDensity{ (a_scale >= 0.85 ? GridDensity::eLarge : GridDensity::eSmall) };

  if (newDensity == m_gridDensity) return;

  m_gridDensity = newDensity;
}

} // namespace spaghetti
