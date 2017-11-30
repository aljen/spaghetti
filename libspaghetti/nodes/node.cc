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

#include "nodes/node.h"

#include <bitset>
#include <cmath>
#include <iostream>

#include <QComboBox>
#include <QDebug>
#include <QGraphicsDropShadowEffect>
#include <QLineEdit>
#include <QSpinBox>
#include <QTableWidget>
#include <QTreeWidget>

#include "elements/package.h"
#include "ui/colors.h"
#include "ui/package_view.h"

namespace nodes {

constexpr QSizeF ICON_SIZE{ 100.0, 50.0 };

Node::Node(QGraphicsItem *const a_parent)
  : QGraphicsItem{ a_parent }
{
  setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemSendsGeometryChanges);
  //  m_boundingRect = QRectF(QPointF{ 0.0, 0.0 }, ICON_SIZE);

  QGraphicsDropShadowEffect *const effect{ new QGraphicsDropShadowEffect };
  //  effect->setColor(QColor(103, 117, 126, 255));
  QColor color(58, 66, 71, 255); // DARK GREY
  effect->setColor(color);
  //  effect->setOffset(0.0, 0.0);
  //  effect->setBlurRadius(64.0);
  effect->setBlurRadius(15.0);
  effect->setColor(QColor("#99121212"));
  setGraphicsEffect(effect);

  iconify();
}

Node::~Node()
{
  for (auto &input : m_inputs)
    input->disconnectAll();

  for (auto &output : m_outputs)
    output->disconnectAll();

  if (m_element) {
    auto const package = m_element->package();
    package->remove(m_element);
  }
}

QRectF Node::boundingRect() const
{
  return m_boundingRect;
}

void Node::paint(QPainter *a_painter, QStyleOptionGraphicsItem const *a_option, QWidget *a_widget)
{
  (void)a_option;
  (void)a_widget;

  paintBorder(a_painter);
  if (!m_centralWidget) paintIcon(a_painter);
}

QVariant Node::itemChange(QGraphicsItem::GraphicsItemChange a_change, QVariant const &a_value)
{
  (void)a_change;
  (void)a_value;

  switch (a_change) {
    case ItemSelectedHasChanged: {
      nodes::Node *lastSelected{};
      auto selectedItems = scene()->selectedItems();
      for (auto &&item : selectedItems) {
        if (item->type() == NODE_TYPE) lastSelected = static_cast<nodes::Node *>(item);
      }
      m_packageView->setSelectedNode(lastSelected);
      m_packageView->showProperties();
      break;
    }
    case ItemPositionChange: {
      QPointF const position{ a_value.toPointF() };
      qreal const x{ std::round(position.x() / 10.0) * 10.0 };
      qreal const y{ std::round(position.y() / 10.0) * 10.0 };
      return QPointF{ x, y };
      break;
    }
    case ItemPositionHasChanged: {
      if (m_element) {
        QPointF const position{ a_value.toPointF() };
        switch (m_type) {
          case Type::eElement: m_element->setPosition(position.x(), position.y()); break;
          case Type::eInputs: {
            auto const package = reinterpret_cast<elements::Package *const>(m_element);
            package->setInputsPosition(position.x(), position.y());
            break;
          }
          case Type::eOutputs: {
            auto const package = reinterpret_cast<elements::Package *const>(m_element);
            package->setOutputsPosition(position.x(), position.y());
            break;
          }
        }
      }
      break;
    }
    default: break;
  }

  return QGraphicsItem::itemChange(a_change, a_value);
}

void Node::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *a_event)
{
  (m_mode == Mode::eIconified) ? expand() : iconify();

  QGraphicsItem::mouseDoubleClickEvent(a_event);
}

void Node::advance(int a_phase)
{
  if (!a_phase) return;

  refreshCentralWidget();

  update();
}

void Node::setElement(elements::Element *const a_element)
{
  if (m_element) qDebug() << "Already have element!";

  m_element = a_element;

  auto const &inputs = m_element->inputs();
  auto const &outputs = m_element->outputs();

  switch (m_type) {
    case Type::eElement:
      for (size_t i = 0; i < inputs.size(); ++i) {
        QString const name = QString::fromStdString(inputs[i].name);
        addSocket(SocketType::eInput, static_cast<uint8_t>(i), name, inputs[i].type);
      }
      for (size_t i = 0; i < outputs.size(); ++i) {
        QString const name = QString::fromStdString(outputs[i].name);
        addSocket(SocketType::eOutput, static_cast<uint8_t>(i), name, outputs[i].type);
      }
      break;
    case Type::eInputs:
      for (size_t i = 0; i < inputs.size(); ++i) {
        QString const name = QString::fromStdString(inputs[i].name);
        addSocket(SocketType::eOutput, static_cast<uint8_t>(i), name, inputs[i].type);
      }
      break;
    case Type::eOutputs:
      for (size_t i = 0; i < outputs.size(); ++i) {
        QString const name = QString::fromStdString(outputs[i].name);
        addSocket(SocketType::eInput, static_cast<uint8_t>(i), name, outputs[i].type);
      }
      break;
  }

  m_element->onChange([&](elements::Element *const a_changed) { setOutputs(a_changed); });

  m_element->setPosition(x(), y());
  m_element->iconify(m_mode == Mode::eIconified);

  m_element->calculate();

  if (m_type == Type::eElement) {
    setName(QString::fromStdString(m_element->name()));
    setOutputs(m_element);
  }

  elementSet();

  calculateBoundingRect();
}

void Node::setName(QString a_name)
{
  QString const from{ m_name };

  m_name = a_name;

  if (m_element) {
    m_element->setName(a_name.toStdString());
    setToolTip(QString("%1 (%2)").arg(a_name).arg(m_element->id()));
  } else
    setToolTip(QString("%1").arg(a_name));
}

void Node::setIcon(QString a_icon)
{
  m_iconPath = a_icon;
  m_icon.load(a_icon);
}

void Node::iconify()
{
  if (m_element) m_element->iconify(true);
  m_mode = Mode::eIconified;

  for (auto &&input : m_inputs) input->hideName();
  for (auto &&output : m_outputs) output->hideName();

  calculateBoundingRect();
}

void Node::expand()
{
  if (m_element) m_element->iconify(false);
  m_mode = Mode::eExpanded;

  for (auto &&input : m_inputs) input->showName();
  for (auto &&output : m_outputs) output->showName();

  calculateBoundingRect();
}

void Node::setPropertiesTable(QTableWidget *const a_properties)
{
  m_properties = a_properties;
}

void Node::paintBorder(QPainter *const a_painter)
{
  auto rect = boundingRect();

  QPen pen{ get_color(Color::eSocketBorder) };
  pen.setWidth(2);
  QColor color{ 105, 105, 105, 128 };
  QBrush brush{ color };
  a_painter->setPen(pen);
  a_painter->setBrush(brush);
  a_painter->drawRect(rect);

  pen.setColor(isSelected() ? QColor(156, 156, 156, 255) : QColor(58, 66, 71, 255));
  pen.setWidth(2);
  a_painter->setPen(pen);
  a_painter->setBrush(Qt::NoBrush);
  a_painter->drawRect(rect);
}

void Node::paintIcon(QPainter *const a_painter)
{
  auto const thisSize = m_boundingRect.size();
  auto const iconSize = m_icon.size();

  auto const y = static_cast<int>((thisSize.height() / 2.0) - (iconSize.height() / 2.0));
  auto const w = iconSize.width();
  auto const h = iconSize.height();
  a_painter->drawPixmap(static_cast<int>(m_centralWidgetPosition.x()), y, w, h, m_icon);
}

void Node::showProperties()
{
  showCommonProperties();
  showInputsProperties();
  showOutputsProperties();
}

void Node::showCommonProperties()
{
  m_properties->setRowCount(0);
  //  m_properties->clear();
  propertiesInsertTitle("Element");

  QTableWidgetItem *item{};
  int const id{ static_cast<int>(m_element->id()) };
  QString const type{ QString::fromLocal8Bit(m_element->type()) };
  QString const name{ QString::fromStdString(m_element->name()) };

  int row = m_properties->rowCount();
  m_properties->insertRow(row);
  item = new QTableWidgetItem{ "ID" };
  item->setFlags(item->flags() & ~Qt::ItemIsEditable);
  m_properties->setItem(row, 0, item);

  item = new QTableWidgetItem{ id };
  item->setFlags(item->flags() & ~Qt::ItemIsEditable);
  item->setData(Qt::DisplayRole, id);
  m_properties->setItem(row, 1, item);

  row = m_properties->rowCount();
  m_properties->insertRow(row);
  item = new QTableWidgetItem{ "Type" };
  item->setFlags(item->flags() & ~Qt::ItemIsEditable);
  m_properties->setItem(row, 0, item);

  item = new QTableWidgetItem{ type };
  item->setFlags(item->flags() & ~Qt::ItemIsEditable);
  m_properties->setItem(row, 1, item);

  row = m_properties->rowCount();
  m_properties->insertRow(row);
  item = new QTableWidgetItem{ "Name" };
  item->setFlags(item->flags() & ~Qt::ItemIsEditable);
  m_properties->setItem(row, 0, item);

  QLineEdit *nameEdit = new QLineEdit{ name };
  m_properties->setCellWidget(row, 1, nameEdit);
  QObject::connect(nameEdit, &QLineEdit::textChanged, [this](QString const &a_text) { setName(a_text); });
}

QString valueType2QString(elements::Element::ValueType a_type)
{
  switch (a_type) {
    case elements::Element::ValueType::eBool: return "Bool";
    case elements::Element::ValueType::eInt: return "Int";
    case elements::Element::ValueType::eFloat: return "Float";
  }
  return "Unknown";
}

void Node::showInputsProperties(int a_allowedTypes)
{
  (void)a_allowedTypes;

  auto &inputs = m_element->inputs();
  int const INPUTS_SIZE = static_cast<int>(inputs.size());

  uint8_t const MIN_INPUTS_SIZE = m_element->minInputs();
  uint8_t const MAX_INPUTS_SIZE = m_element->maxInputs();
  bool const ADDING_DISABLED = MIN_INPUTS_SIZE == MAX_INPUTS_SIZE;
  //  qDebug() << "ADDING_DISABLED:" << ADDING_DISABLED;

  QTableWidgetItem *item{};
  int row = m_properties->rowCount();

  if (m_inputs.size()) propertiesInsertTitle("Inputs");

  if (!ADDING_DISABLED) {
    row = m_properties->rowCount();
    m_properties->insertRow(row);
    item = new QTableWidgetItem{ "Count" };
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    m_properties->setItem(row, 0, item);

    QSpinBox *const count = new QSpinBox{};
    count->setRange(MIN_INPUTS_SIZE, MAX_INPUTS_SIZE);
    count->setValue(static_cast<int>(inputs.size()));
    m_properties->setCellWidget(row, 1, count);
    QObject::connect(count, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [this](int a_value) {
      auto const size = m_element->inputs().size();
      if (static_cast<int>(size) < a_value)
        addInput(ValueType::eBool);
      else
        removeInput();
    });
  }

  std::bitset<3> const allowedTypes(static_cast<uint64_t>(a_allowedTypes));

  for (int i = 0; i < INPUTS_SIZE; ++i) {
    //    qDebug() << "MIN:" << MIN_INPUTS_SIZE << "MAX:" << MAX_INPUTS_SIZE << "INDEX:" << i;

    row = m_properties->rowCount();
    m_properties->insertRow(row);
    QLineEdit *const inputName = new QLineEdit{ QString::fromStdString(inputs[i].name) };
    m_properties->setCellWidget(row, 0, inputName);

    QString const inputType = valueType2QString(inputs[i].type);

    if (allowedTypes.count() <= 1) {
      item = new QTableWidgetItem{ inputType };
      item->setFlags(item->flags() & ~Qt::ItemIsEditable);
      m_properties->setItem(row, 1, item);
    } else {
      QComboBox *const comboBox{ new QComboBox };
      if (a_allowedTypes & eBoolType)
        comboBox->addItem(valueType2QString(ValueType::eBool), static_cast<int>(ValueType::eBool));
      if (a_allowedTypes & eIntType)
        comboBox->addItem(valueType2QString(ValueType::eInt), static_cast<int>(ValueType::eInt));
      if (a_allowedTypes & eFloatType)
        comboBox->addItem(valueType2QString(ValueType::eFloat), static_cast<int>(ValueType::eFloat));
      m_properties->setCellWidget(row, 1, comboBox);
      QObject::connect(comboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::activated),
                       [i, comboBox, this](int a_index) {
                         ValueType const valueType = static_cast<ValueType>(comboBox->itemData(a_index).toInt());
                         setInputType(static_cast<uint8_t>(i), valueType);
                       });
    }
  }
}

void Node::showOutputsProperties(int a_allowedTypes)
{
  (void)a_allowedTypes;

  auto &outputs = m_element->outputs();
  int const OUTPUTS_SIZE = static_cast<int>(outputs.size());

  uint8_t const MIN_OUTPUTS_SIZE = m_element->minOutputs();
  uint8_t const MAX_OUTPUTS_SIZE = m_element->maxOutputs();
  bool const ADDING_DISABLED = MIN_OUTPUTS_SIZE == MAX_OUTPUTS_SIZE;
  //  qDebug() << "ADDING_DISABLED:" << ADDING_DISABLED;

  QTableWidgetItem *item{};
  int row = m_properties->rowCount();

  if (OUTPUTS_SIZE) propertiesInsertTitle("Outputs");

  if (!ADDING_DISABLED) {
    row = m_properties->rowCount();
    m_properties->insertRow(row);
    item = new QTableWidgetItem{ "Count" };
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    m_properties->setItem(row, 0, item);

    QSpinBox *const count = new QSpinBox{};
    count->setRange(MIN_OUTPUTS_SIZE, MAX_OUTPUTS_SIZE);
    count->setValue(static_cast<int>(OUTPUTS_SIZE));
    m_properties->setCellWidget(row, 1, count);
    QObject::connect(count, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [this](int a_value) {
      auto const size = m_element->outputs().size();
      if (static_cast<int>(size) < a_value)
        addOutput(ValueType::eBool);
      else
        removeOutput();
    });
  }

  for (int i = 0; i < OUTPUTS_SIZE; ++i) {
    //    qDebug() << "MIN:" << MIN_OUTPUTS_SIZE << "MAX:" << MAX_OUTPUTS_SIZE << "INDEX:" << i;

    row = m_properties->rowCount();
    m_properties->insertRow(row);
    QLineEdit *const outputName = new QLineEdit{ QString::fromStdString(outputs[i].name) };
    m_properties->setCellWidget(row, 0, outputName);

    QString const outputType = valueType2QString(outputs[i].type);

    item = new QTableWidgetItem{ outputType };
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    m_properties->setItem(row, 1, item);
  }
}

void Node::setCentralWidget(QGraphicsItem *a_centralWidget)
{
  if (m_centralWidget) delete m_centralWidget;
  m_centralWidget = a_centralWidget;
  m_centralWidget->setParentItem(this);
  m_centralWidget->setPos(m_centralWidgetPosition);
}

void Node::propertiesInsertTitle(QString a_title)
{
  int const ROW = m_properties->rowCount();
  m_properties->insertRow(ROW);
  QTableWidgetItem *const item{ new QTableWidgetItem{ a_title } };
  item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
  item->setFlags(item->flags() & ~Qt::ItemIsEditable);
  item->setBackgroundColor(Qt::darkGray);
  item->setTextColor(Qt::black);
  m_properties->setItem(ROW, 0, item);
  m_properties->setSpan(ROW, 0, 1, 2);
}

template<typename Container, class Comparator>
auto max_element(Container &a_container, Comparator a_comparator)
{
  return std::max_element(std::begin(a_container), std::end(a_container), a_comparator);
}

void Node::calculateBoundingRect()
{
  prepareGeometryChange();

  constexpr int32_t const SOCKET_SIZE = SocketItem::SIZE;
  qreal const ROUNDED_SOCKET_SIZE = std::round(static_cast<qreal>(SOCKET_SIZE) / 10.0) * 10.0;
  auto const INPUTS_COUNT = m_inputs.count();
  auto const OUTPUTS_COUNT = m_outputs.count();
  auto const SOCKETS_COUNT = std::max(INPUTS_COUNT, OUTPUTS_COUNT);
  auto const CENTRAL_SIZE = m_centralWidget ? m_centralWidget->boundingRect().size() : ICON_SIZE;
  auto const SOCKETS_HEIGHT = SOCKETS_COUNT * ROUNDED_SOCKET_SIZE;

  auto maxNameWidth = [](auto &&a_a, auto &&a_b) { return a_a->nameWidth() < a_b->nameWidth(); };
  auto const LONGEST_INPUT = max_element(m_inputs, maxNameWidth);
  auto const LONGEST_OUTPUT = max_element(m_outputs, maxNameWidth);
  int const LONGEST_INPUTS_NAME_WIDTH = LONGEST_INPUT != std::end(m_inputs) ? (*LONGEST_INPUT)->nameWidth() : 0;
  int const LONGEST_OUTPUTS_NAME_WIDTH = LONGEST_OUTPUT != std::end(m_outputs) ? (*LONGEST_OUTPUT)->nameWidth() : 0;
  int const INPUTS_NAME_WIDTH = m_mode == Mode::eExpanded ? LONGEST_INPUTS_NAME_WIDTH : 0;
  int const OUTPUTS_NAME_WIDTH = m_mode == Mode::eExpanded ? LONGEST_OUTPUTS_NAME_WIDTH : 0;

  qreal width{ CENTRAL_SIZE.width() };
  qreal height{};

  if (SOCKETS_HEIGHT > CENTRAL_SIZE.height())
    height = SOCKETS_HEIGHT + ROUNDED_SOCKET_SIZE;
  else {
    height = CENTRAL_SIZE.height() + ROUNDED_SOCKET_SIZE / 2.0;
    if (SOCKETS_COUNT < 2) height += ROUNDED_SOCKET_SIZE / 2.0;
  }

  width = ROUNDED_SOCKET_SIZE + INPUTS_NAME_WIDTH + CENTRAL_SIZE.width() + OUTPUTS_NAME_WIDTH + ROUNDED_SOCKET_SIZE;
  width = std::round(width / 10.0) * 10.0;
  height = std::round(height / 10.0) * 10.0;

  qreal const CENTRAL_X = ROUNDED_SOCKET_SIZE + INPUTS_NAME_WIDTH;
  qreal const CENTRAL_Y = (height / 2.0) - (CENTRAL_SIZE.height() / 2.0);
  m_centralWidgetPosition = QPointF{ CENTRAL_X, CENTRAL_Y };
  if (m_centralWidget) m_centralWidget->setPos(m_centralWidgetPosition);

  qreal yOffset{ ROUNDED_SOCKET_SIZE };
  for (auto &&input : m_inputs) {
    input->setPos(0.0, yOffset);
    yOffset += ROUNDED_SOCKET_SIZE;
  }

  yOffset = ROUNDED_SOCKET_SIZE;
  for (auto &&output : m_outputs) {
    output->setPos(width, yOffset);
    yOffset += ROUNDED_SOCKET_SIZE;
  }

  m_boundingRect = QRectF{ 0.0, 0.0, width, height };
}

void Node::addInput(ValueType const a_type)
{
  uint8_t const size = static_cast<uint8_t>(m_element->inputs().size());
  QString const inputName = QString("#%1").arg(size);

  m_element->addInput(a_type, inputName.toStdString());
  addSocket(SocketType::eInput, size, inputName, a_type);

  calculateBoundingRect();
  m_packageView->showProperties();
}

void Node::removeInput()
{
  m_element->removeInput();
  removeSocket(SocketType::eInput);
  calculateBoundingRect();
  m_packageView->showProperties();
}

void Node::setInputName(uint8_t a_socketId, QString const a_name)
{
  m_element->setInputName(a_socketId, a_name.toStdString());
  m_inputs[a_socketId]->setName(a_name);
  calculateBoundingRect();
  m_packageView->showProperties();
}

void Node::setInputType(uint8_t a_socketId, const Node::ValueType a_type)
{
  (void)a_socketId;
  (void)a_type;
}

void Node::addOutput(ValueType const a_type)
{
  uint8_t const size = static_cast<uint8_t>(m_element->outputs().size());
  QString const name = QString("#%1").arg(size);

  m_element->addOutput(a_type, name.toStdString());
  addSocket(SocketType::eOutput, size, name, a_type);

  calculateBoundingRect();
  m_packageView->showProperties();
}

void Node::removeOutput()
{
  m_element->removeOutput();
  removeSocket(SocketType::eOutput);
  calculateBoundingRect();
  m_packageView->showProperties();
}

void Node::setOutputName(uint8_t a_socketId, QString const a_name)
{
  m_element->setOutputName(a_socketId, a_name.toStdString());
  m_outputs[a_socketId]->setName(a_name);
  calculateBoundingRect();
  m_packageView->showProperties();
}

void Node::setOutputType(uint8_t a_socketId, const Node::ValueType a_type)
{
  (void)a_socketId;
  (void)a_type;
}

void Node::addSocket(SocketType const a_type, uint8_t const a_id, QString const a_name, ValueType const a_valueType)
{
  auto *const socket{ new SocketItem{ this, a_type } };
  socket->setElementId(m_element->id());
  socket->setSocketId(a_id);

  switch (a_valueType) {
    case ValueType::eBool: socket->setColors(get_color(Color::eBoolSignalOff), get_color(Color::eBoolSignalOn)); break;
    case ValueType::eFloat:
      socket->setColors(get_color(Color::eFloatSignalOn), get_color(Color::eFloatSignalOff));
      break;
    case ValueType::eInt:
      socket->setColors(get_color(Color::eIntegerSignalOn), get_color(Color::eIntegerSignalOn));
      break;
  }

  socket->setName(a_name);
  socket->setValueType(a_valueType);

  if (m_mode == Mode::eIconified)
    socket->hideName();
  else
    socket->showName();

  if (a_type == SocketType::eInput)
    m_inputs.push_back(socket);
  else
    m_outputs.push_back(socket);
}

void Node::removeSocket(const Node::SocketType a_type)
{
  switch (a_type) {
    case SocketType::eInput:
      delete m_inputs.back();
      m_inputs.pop_back();
      break;
    case SocketType::eOutput:
      delete m_outputs.back();
      m_outputs.pop_back();
      break;
  }
}

void Node::setOutputs(elements::Element *const a_element)
{
  assert(a_element == m_element);
  auto const &changedOutputs = a_element->outputs();
  for (size_t i = 0; i < changedOutputs.size(); ++i) {
    switch (changedOutputs[i].type) {
      case ValueType::eBool: {
        bool const signal{ std::get<bool>(changedOutputs[i].value) };
        m_outputs[static_cast<int>(i)]->setSignal(signal);
        break;
      }
      default: break;
    }
  }
}

} // namespace nodes
