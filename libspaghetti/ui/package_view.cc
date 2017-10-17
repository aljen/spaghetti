#include "ui/package_view.h"

#include <QDebug>
#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QDragMoveEvent>
#include <QGraphicsScene>
#include <QMimeData>
#include <QTableWidget>
#include <QHeaderView>
#include <QTimeLine>

#include "core/registry.h"
#include "elements/package.h"
#include "nodes/node.h"
#include "ui/elements_list.h"
#include "ui/link_item.h"

PackageView::PackageView(QTableWidget *const a_properties, elements::Package *const a_package)
  : QGraphicsView{ new QGraphicsScene }
  , m_properties{ a_properties }
  , m_package{ (a_package ? a_package : new elements::Package) }
  , m_scene{ scene() }
  , m_inputs{ new nodes::Node }
  , m_outputs{ new nodes::Node }
  , m_gridLarge{ new QGraphicsItemGroup }
  , m_gridSmall{ new QGraphicsItemGroup }
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
  setObjectName("PackageView");

  m_scene->setSceneRect(-32000, -32000, 64000, 64000);
  m_scene->setObjectName("PackageViewScene");

  QBrush brush{ QColor(169, 169, 169, 32) };
  //  QBrush brush{ QColor(244, 53, 64, 255) }; // RED
  //  QBrush brush{ QColor(232, 0, 99, 255) }; // PINK
  //  QBrush brush{ QColor(154, 41, 169, 255) }; // PURPLE
  //  QBrush brush{ QColor(101, 63, 176, 255) }; // DEEP PURPLE
  //  QBrush brush{ QColor(62, 84, 174, 255) }; // INDIGO
  //  QBrush brush{ QColor(29, 151, 237, 255) }; // BLUE
  //  QBrush brush{ QColor(0, 170, 238, 255) }; // LIGHT BLUE
  //  QBrush brush{ QColor(0, 188, 208, 255) }; // CYAN
  //  QBrush brush{ QColor(0, 149, 134, 255) }; // TEAL
  //  QBrush brush{ QColor(75, 173, 88, 255) }; // GREEN
  //  QBrush brush{ QColor(137, 193, 86, 255) }; // LIGHT GREEN
  //  QBrush brush{ QColor(203, 217, 81, 255) }; // LIME
  //  QBrush brush{ QColor(254, 231, 86, 255) }; // YELLOW
  //  QBrush brush{ QColor(254, 187, 59, 255) }; // AMBER
  //  QBrush brush{ QColor(254, 144, 50, 255) }; // ORANGE
  //  QBrush brush{ QColor(254, 74, 53, 255) }; // DEEP ORANGE
  //  QBrush brush{ QColor(120, 83, 74, 255) }; // BROWN
  //  QBrush brush{ QColor(156, 156, 156, 255) }; // GREY
  //  QBrush brush{ QColor(95, 124, 136, 255) }; // BLUE GREY
  //  QBrush brush{ QColor(58, 66, 71, 255) }; // DARK GREY
  m_scene->setBackgroundBrush(brush);

  createGrid();

  setAcceptDrops(true);

  using NodeType = nodes::Node::Type;
  m_inputs->setPos(-600.0, 0.0);
  m_inputs->setType(NodeType::eInputs);
  m_inputs->setElement(m_package);
  m_inputs->setIcon(":/elements/logic/inputs.png");
  m_inputs->setPackageView(this);
  m_inputs->iconify();
  m_outputs->setPos(600.0, 0.0);
  m_outputs->setType(NodeType::eOutputs);
  m_outputs->setElement(m_package);
  m_outputs->setIcon(":/elements/logic/outputs.png");
  m_outputs->setPackageView(this);
  m_outputs->iconify();

  core::Registry &registry{ core::Registry::get() };

  m_package->setInputsPosition(m_inputs->x(), m_inputs->y());
  m_package->setOutputsPosition(m_outputs->x(), m_outputs->y());
  m_package->setName(registry.elementName("logic/package"));

  m_scene->addItem(m_inputs);
  m_scene->addItem(m_outputs);
  m_package->startDispatchThread();
}

PackageView::~PackageView()
{
  qDebug() << Q_FUNC_INFO;
  if (m_standalone) {
    m_package->quitDispatchThread();
    delete m_package;
  }
}

void PackageView::open()
{
  qDebug() << Q_FUNC_INFO;
  m_package->open(m_filename.toStdString());

  auto const &inputsPosition = m_package->inputsPosition();
  auto const &outputsPosition = m_package->outputsPosition();
  m_inputs->setPos(inputsPosition.x, inputsPosition.y);
  m_outputs->setPos(outputsPosition.x, outputsPosition.y);

  core::Registry &registry{ core::Registry::get() };

  auto const &elements = m_package->elements();
  size_t const SIZE{ elements.size() };
  for (size_t i = 1; i < SIZE; ++i) {
    auto const element = elements[i];
    auto const node = registry.createNode(element->hash());
    auto const nodeName = QString::fromStdString(registry.elementName(element->hash()));
    auto const nodeIcon= QString::fromStdString(registry.elementIcon(element->hash()));
    auto const nodePath = QString::fromLocal8Bit(element->type());

    m_nodes[element->id()] = node;

    node->setPackageView(this);
    node->setName(nodeName);
    node->setPath(nodePath);
    node->setIcon(nodeIcon);
    node->setPos(element->position().x, element->position().y);
    node->setElement(element);
    element->isIconified() ? node->iconify() : node->expand();
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
  if (a_event->mimeData()->hasText()) {
    auto const pathString = a_event->mimeData()->text();
    auto const name = a_event->mimeData()->data("metadata/name");
    auto const icon = a_event->mimeData()->data("metadata/icon");
    auto const stringData = pathString.toLatin1();
    auto const path = stringData.data();

    auto const dropPosition = mapToScene(a_event->pos());

    core::Registry &registry{ core::Registry::get() };

    assert(m_dragNode == nullptr);
    m_dragNode = registry.createNode(path);
    m_dragNode->setPackageView(this);
    m_dragNode->setName(name);
    m_dragNode->setPath(pathString);
    m_dragNode->setIcon(icon);
    m_dragNode->setPos(dropPosition);
    m_scene->addItem(m_dragNode);

    a_event->accept();
  } else
    QGraphicsView::dragEnterEvent(a_event);
}

void PackageView::dragLeaveEvent(QDragLeaveEvent *)
{
  m_scene->removeItem(m_dragNode);
  delete m_dragNode;
  m_dragNode = nullptr;
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
  if (a_event->mimeData()->hasText()) {
    auto const pathString = a_event->mimeData()->text();
    auto const stringData = pathString.toLatin1();
    char const *const path{ stringData.data() };

    auto const element = m_package->add(path);
    element->setName(m_dragNode->name().toStdString());
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
  qDebug() << a_event;
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
    if (item->type() == nodes::NODE_TYPE) {
      qDebug() << "Node:" << item;
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
  m_properties->horizontalHeader()->setStretchLastSection(true);
  m_properties->setRowCount(3);

  QTableWidgetItem *item{};

  int const id{ static_cast<int>(m_package->id()) };
  QString const type{ QString::fromLocal8Bit(m_package->type()) };
  QString const name{ QString::fromStdString(m_package->name()) };

  item = new QTableWidgetItem{ "ID" };
  item->setFlags(item->flags() & ~Qt::ItemIsEditable);
  m_properties->setItem(0, 0, item);

  item = new QTableWidgetItem{ id };
  item->setFlags(item->flags() & ~Qt::ItemIsEditable);
  item->setData(Qt::DisplayRole, id );
  m_properties->setItem(0, 1, item);

  item = new QTableWidgetItem{ "Type" };
  item->setFlags(item->flags() & ~Qt::ItemIsEditable);
  m_properties->setItem(1, 0, item);

  item = new QTableWidgetItem{ type };
  item->setFlags(item->flags() & ~Qt::ItemIsEditable);
  m_properties->setItem(1, 1, item);

  item = new QTableWidgetItem{ "Name" };
  item->setFlags(item->flags() & ~Qt::ItemIsEditable);
  m_properties->setItem(2, 0, item);

  item = new QTableWidgetItem{ name };
  m_properties->setItem(2, 1, item);
}

void PackageView::createGrid()
{
  constexpr int32_t SIZE = 10000;
  constexpr int32_t SPACING = 10;
  QPen penNormal{ QColor(156, 156, 156, 32) };
  QPen penAxis{ QColor(156, 156, 156, 128) };
  QGraphicsLineItem *horizontal{};
  QGraphicsLineItem *vertical{};

  for (int32_t i = -SIZE; i <= SIZE; i += SPACING) {
    penNormal.setWidth(1);
    penAxis.setWidth(1);

    horizontal = new QGraphicsLineItem{ static_cast<qreal>(i), -SIZE, static_cast<qreal>(i), SIZE };
    horizontal->setPen(i == 0 ? penAxis : penNormal);
    horizontal->setZValue(-100.0);
    m_gridLarge->addToGroup(horizontal);

    vertical = new QGraphicsLineItem{ -SIZE, static_cast<qreal>(i), SIZE, static_cast<qreal>(i) };
    vertical->setPen(i == 0 ? penAxis : penNormal);
    vertical->setZValue(-100.0);
    m_gridLarge->addToGroup(vertical);

    if (i % 100 == 0 || i == 0) {
      penNormal.setWidth(3);
      penAxis.setWidth(3);

      horizontal = new QGraphicsLineItem{ static_cast<qreal>(i), -SIZE, static_cast<qreal>(i), SIZE };
      horizontal->setPen(i == 0 ? penAxis : penNormal);
      horizontal->setZValue(-100.0);
      m_gridSmall->addToGroup(horizontal);

      vertical = new QGraphicsLineItem{ -SIZE, static_cast<qreal>(i), SIZE, static_cast<qreal>(i) };
      vertical->setPen(i == 0 ? penAxis : penNormal);
      vertical->setZValue(-100.0);
      m_gridSmall->addToGroup(vertical);
    }
  }

  m_gridDensity = GridDensity::eLarge;
  m_scene->addItem(m_gridLarge);
  m_scene->addItem(m_gridSmall);
  m_gridSmall->hide();

  updateGrid(matrix().m11());
}

void PackageView::updateGrid(qreal const a_scale)
{
  GridDensity const newDensity{ (a_scale >= 0.85 ? GridDensity::eLarge : GridDensity::eSmall) };

  if (newDensity == m_gridDensity) return;

  switch (m_gridDensity = newDensity; m_gridDensity) {
    case GridDensity::eLarge:
      m_gridLarge->show();
      m_gridSmall->hide();
      break;
    case GridDensity::eSmall:
      m_gridLarge->hide();
      m_gridSmall->show();
      break;
  }
}
