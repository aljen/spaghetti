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

#include "package_view.h"

#include <QDebug>
#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QDragMoveEvent>
#include <QGraphicsScene>
#include <QHeaderView>
#include <QMimeData>
#include <QSortFilterProxyModel>
#include <QTableWidget>
#include <QTimeLine>

// clang-format off
#ifdef SPAGHETTI_USE_OPENGL
# include <QGLWidget>
#endif
// clang-format on

#include "spaghetti/editor/editor.h"
#include "spaghetti/editor/node.h"
#include "spaghetti/package.h"
#include "spaghetti/registry.h"
#include "elements_list.h"
#include "link_item.h"
#include "nodes/package.h"
#include "nodes_registry.h"

namespace spaghetti {

NodesListModel::NodesListModel(QObject *const a_parent)
  : QAbstractListModel{ a_parent }
{
}

int NodesListModel::rowCount(QModelIndex const &a_parent) const
{
  (void)a_parent;
  return m_nodes.count();
}

QVariant NodesListModel::data(QModelIndex const &a_index, int a_role) const
{
  if (!a_index.isValid()) return QVariant{};

  auto const node = m_nodes[a_index.row()];
  if (a_role == Qt::DecorationRole)
    return node->icon().scaled(QSize(50, 25));
  else if (a_role == Qt::DisplayRole)
    return QString("%1 (%2)").arg(node->name()).arg(node->element()->id());

  return QVariant{};
}

void NodesListModel::add(Node *const a_node)
{
  auto const ROW = rowCount();
  beginInsertRows(QModelIndex(), ROW, ROW);
  m_nodes.append(a_node);
  endInsertRows();
}

void NodesListModel::remove(Node *const a_node)
{
  auto const INDEX = m_nodes.indexOf(a_node);
  beginRemoveRows(QModelIndex(), INDEX, INDEX);
  m_nodes.removeAt(INDEX);
  endRemoveRows();
}

void NodesListModel::update(Node *const a_node)
{
  (void)a_node;
  auto const INDEX = m_nodes.indexOf(a_node);
  emit dataChanged(index(INDEX), index(INDEX));
}

Node *NodesListModel::nodeFor(QModelIndex const &a_index)
{
  if (!a_index.isValid()) return nullptr;
  auto const ROW = a_index.row();
  if (ROW < 0 || ROW > m_nodes.size()) return nullptr;

  return m_nodes[ROW];
}

PackageView::PackageView(Editor *const a_editor, Package *const a_package)
  : QGraphicsView{ new QGraphicsScene }
  , m_editor{ a_editor }
  , m_elements{ a_editor->elementsList() }
  , m_properties{ a_editor->propertiesTable() }
  , m_nodesModel{ new NodesListModel{ this } }
  , m_nodesProxyModel{ new QSortFilterProxyModel{ this } }
  , m_package{ a_package }
  , m_scene{ scene() }
  , m_inputs{ new Node }
  , m_outputs{ new Node }
  , m_standalone{ m_package->package() == nullptr }
{
  if (m_standalone) {
    m_packageNode = static_cast<nodes::Package *>(NodesRegistry::get().create("logic/package"));
    m_package->setNode(m_packageNode);
  } else
    m_packageNode = m_package->node<nodes::Package>();
  Q_ASSERT(m_package->node<Node*>());
#ifdef SPAGHETTI_USE_OPENGL
  QGLFormat format{ QGL::DoubleBuffer | QGL::SampleBuffers | QGL::DirectRendering };
  format.setProfile(QGLFormat::CoreProfile);
  setViewport(new QGLWidget{ QGLFormat{ format } });
#endif
  setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
  setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::HighQualityAntialiasing |
                 QPainter::SmoothPixmapTransform);
  setDragMode(QGraphicsView::ScrollHandDrag);
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
  setResizeAnchor(QGraphicsView::NoAnchor);
  setTransformationAnchor(QGraphicsView::AnchorViewCenter);
  setOptimizationFlags(QGraphicsView::DontSavePainterState | QGraphicsView::DontAdjustForAntialiasing);

  setObjectName("PackageView");

  m_nodesProxyModel->setSourceModel(m_nodesModel);

  m_scene->setItemIndexMethod(QGraphicsScene::NoIndex);
  m_scene->setSceneRect(-32000, -32000, 64000, 64000);
  m_scene->setObjectName("PackageViewScene");

  QBrush brush{ QColor(169, 169, 169, 32) };
  m_scene->setBackgroundBrush(brush);

  m_inputs->setPropertiesTable(m_properties);
  m_outputs->setPropertiesTable(m_properties);

  m_packageNode->setPropertiesTable(m_properties);

  setAcceptDrops(true);

  using NodeType = Node::Type;
  m_inputs->setType(NodeType::eInputs);
  m_inputs->setPos(m_package->inputsPosition().x, m_package->inputsPosition().y);
  m_inputs->setElement(m_package);
  m_inputs->setIcon(":/logic/inputs.png");
  m_inputs->setPackageView(this);
  m_inputs->iconify();
  m_outputs->setType(NodeType::eOutputs);
  m_outputs->setPos(m_package->outputsPosition().x, m_package->outputsPosition().y);
  m_outputs->setElement(m_package);
  m_outputs->setIcon(":/logic/outputs.png");
  m_outputs->setPackageView(this);
  m_outputs->iconify();

  m_packageNode->setInputsNode(m_inputs);
  m_packageNode->setOutputsNode(m_outputs);
  m_packageNode->setElement(m_package);

  if (m_package->name().empty()) {
//    auto &registry = Registry::get();
//    m_package->setName(registry.elementName("logic/package"));
  }

  m_scene->addItem(m_inputs);
  m_scene->addItem(m_outputs);

  m_timer.setInterval(1000 / 60);

  connect(&m_timer, &QTimer::timeout, [this]() { m_scene->advance(); });
  m_timer.start();

  if (m_standalone) m_package->startDispatchThread();
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
  auto const &inputsPosition = m_package->inputsPosition();
  auto const &outputsPosition = m_package->outputsPosition();
  m_inputs->setPos(inputsPosition.x, inputsPosition.y);
  m_outputs->setPos(outputsPosition.x, outputsPosition.y);

  auto &registry = NodesRegistry::get();

  auto const &elements = m_package->elements();
  size_t const SIZE{ elements.size() };
  for (size_t i = 1; i < SIZE; ++i) {
    auto const element = elements[i];
    auto const node = create_node_for(element->hash());
    auto const nodeName = node_name_for(element->hash()); //QString::fromStdString(element->type()); //QString::fromStdString(registry.elementName(element->type()));
    auto const nodeIcon = node_icon_for(element->hash()); // "icon"; //QString::fromStdString(registry.elementIcon(element->hash()));
    auto const nodePath = QString::fromLocal8Bit(element->type());
    qDebug() << "nodeIcon:" << nodeIcon;

    element->setNode(node);
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

    m_nodesModel->add(node);
    m_nodesProxyModel->sort(0);
  }

  auto const &connections = m_package->connections();
  for (auto const &connection : connections) {
    auto const SOURCE_ID = connection.from_id;
    auto const SOURCE_SOCKET = connection.from_socket;
    auto const TARGET_ID = connection.to_id;
    auto const TARGET_SOCKET = connection.to_socket;

    auto const source = SOURCE_ID != 0 ? getNode(SOURCE_ID) : m_packageNode->inputsNode();
    auto const target = TARGET_ID != 0 ? getNode(TARGET_ID) : m_packageNode->outputsNode();
    auto const sourceSocket = source->outputs()[SOURCE_SOCKET];
    auto const targetSocket = target->inputs()[TARGET_SOCKET];
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

  //  mimeData->setData("metadata/is_package", IS_PACKAGE);
  //  mimeData->setData("metadata/name", NAME);
  //  mimeData->setData("metadata/icon", ICON);
  //  mimeData->setData("metadata/filename", FILE);

  if (mimeData->hasFormat("metadata/name") && mimeData->hasFormat("metadata/icon")) {
    auto const isPackage = mimeData->data("metadata/is_package") == "true";
    auto const pathString = isPackage ? QString{ "logic/package" } : mimeData->text();
    auto const name = mimeData->data("metadata/name");
    auto const icon = mimeData->data("metadata/icon");
    auto const file = mimeData->data("metadata/filename");
    auto const stringData = pathString.toLatin1();
    auto const path = stringData.data();

    auto const DROP_POSITION = mapToScene(a_event->pos());

    qDebug() << "path:" << path << "pathString:" << pathString;

    assert(m_dragNode == nullptr);
    m_dragNode = create_node_for(0); // registry.createNode(path);
    m_dragNode->setPackageView(this);
    m_dragNode->setPropertiesTable(m_properties);
    m_dragNode->setName(name);
    m_dragNode->setPath(isPackage ? name : "");
    m_dragNode->setIcon(icon);
    m_dragNode->setPos(DROP_POSITION);
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
    m_package->pauseDispatchThread();

    auto const isPackage = mimeData->data("metadata/is_package") == "true";
    auto const file = mimeData->data("metadata/filename");
    auto const pathString = a_event->mimeData()->text();
    auto const stringData = pathString.toLatin1();
    char const *const path{ isPackage ? "logic/package" : stringData.data() };

    auto const element = m_package->add(path);
    element->setNode(m_dragNode);
    if (element->name().empty()) element->setName(m_dragNode->name().toStdString());
    if (isPackage) {
      auto const package = static_cast<Package *>(element);
      package->open(QString{ file }.toStdString());
    }
    m_dragNode->setElement(element);
    m_dragNode->iconify();

    m_nodes[element->id()] = m_dragNode;
    m_nodesModel->add(m_dragNode);
    m_nodesProxyModel->sort(0);

    m_dragNode = nullptr;

    m_package->resumeDispatchThread();
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
    auto const REAL_SCALE = matrix().m11();
    updateGrid(REAL_SCALE);
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
        auto const node = reinterpret_cast<Node *>(item);
        auto const ID = node->element()->id();
        m_nodes.remove(ID);
        m_nodesModel->remove(node);
        m_nodesProxyModel->sort(0);

        if (node == m_selectedNode) setSelectedNode(nullptr);
        delete node;
        break;
      }
      case LINK_TYPE: {
        auto const link = reinterpret_cast<LinkItem *>(item);
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

void PackageView::updateName(Node *const a_node)
{
  m_nodesModel->update(a_node);
  m_nodesProxyModel->sort(0);
}

void PackageView::selectItem(QModelIndex const &a_index)
{
  auto const INDEX = m_nodesProxyModel->mapToSource(a_index);
  auto const node = m_nodesModel->nodeFor(INDEX);

  assert(node);

  scene()->clearSelection();

  node->setSelected(true);
  setSelectedNode(node);
  showProperties();

  centerOn(node);
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
