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

#include "nodes/values/characteristic_curve/editor_window.h"
#include "elements/values/characteristic_curve.h"
#include "nodes/values/characteristic_curve.h"
#include "nodes/values/characteristic_curve/ui_editor_window.h"

#include <algorithm>

#include <QCheckBox>
#include <QComboBox>
#include <QDebug>
#include <QDoubleValidator>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QIntValidator>
#include <QItemDelegate>
#include <QKeyEvent>
#include <QLabel>
#include <QLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>
#include <QTabWidget>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QtCharts/QLineSeries>

#include "ui/package_view.h"
//#include "nodes/values/characteristic_curve/axis_widget.h"
#include "nodes/values/characteristic_curve/axis.h"
//#include "nodes/values/characteristic_curve/series_widget.h"
#include "nodes/values/characteristic_curve/series.h"

template<class T>
class VPtr {
 public:
  static T *asPtr(QVariant v) { return reinterpret_cast<T *>(v.value<void *>()); }
  static QVariant asQVariant(T *ptr) { return qVariantFromValue(static_cast<void *>(ptr)); }
};

namespace spaghetti::nodes::values::characteristic_curve {

class Delegate : public QItemDelegate {
 public:
  Delegate(EditorWindow *const a_window)
    : QItemDelegate{ a_window }
    , m_window{ a_window }
  {
  }

  QWidget *createEditor(QWidget *a_parent, QStyleOptionViewItem const &a_option,
                        QModelIndex const &a_index) const override
  {
    (void)a_option;

    auto const lineEdit = new QLineEdit{ a_parent };

    EditorWindow::ValueType const TYPE{ (a_index.column() == 0 ? m_window->xValueType() : m_window->yValueType()) };
    QValidator *validator{};
    if (TYPE == EditorWindow::ValueType::eInt)
      validator = new QIntValidator;
    else
      validator = new QDoubleValidator;
    QLocale const LOCALE(QLocale::Language::English);
    validator->setLocale(LOCALE);
    lineEdit->setValidator(validator);
    return lineEdit;
  }

 private:
  EditorWindow *const m_window{};
};

EditorWindow::EditorWindow(CharacteristicCurve *const a_characteristicCurve)
  : QDialog{ a_characteristicCurve->packageView() }
  , m_ui{ new Ui::EditorWindow }
  , m_characteristicCurve{ a_characteristicCurve }
{
  m_ui->setupUi(this);

  m_ui->editor->setEditorWindow(this);
  QLocale const LOCALE{ QLocale::Language::English };
  m_ui->editor->setLocale(LOCALE);
  m_ui->seriesTable->setLocale(LOCALE);

  m_ui->xAxisName->setText(m_ui->editor->xName());
  m_ui->xAxisMajorTicks->setValue(m_ui->editor->xMajorTicks());
  m_ui->xAxisMinorTicks->setValue(m_ui->editor->xMinorTicks());
  m_ui->xAxisMinimum->setValue(m_ui->editor->xMinimum());
  m_ui->xAxisMaximum->setValue(m_ui->editor->xMaximum());

  connect(m_ui->xAxisName, &QLineEdit::textChanged, [this](QString const &a_name) {
    m_characteristicCurve->changeInputName(0, a_name);
    m_ui->editor->setXName(a_name);
  });

  connect(m_ui->xAxisMajorTicks, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
          [this](int a_value) { m_ui->editor->setXMajorTicks(a_value); });

  connect(m_ui->xAxisMinorTicks, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
          [this](int a_value) { m_ui->editor->setXMinorTicks(a_value); });

  connect(m_ui->xAxisMinimum, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
          [this](qreal a_value) { m_ui->editor->setXMinimum(a_value); });

  connect(m_ui->xAxisMaximum, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
          [this](qreal a_value) { m_ui->editor->setXMaximum(a_value); });

  connect(m_ui->yAxisName, &QLineEdit::textChanged, [this](QString const &a_name) {
    m_characteristicCurve->changeOutputName(0, a_name);
    m_ui->editor->setYName(a_name);
  });

  connect(m_ui->yAxisMajorTicks, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
          [this](int a_value) { m_ui->editor->setYMajorTicks(a_value); });

  connect(m_ui->yAxisMinorTicks, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
          [this](int a_value) { m_ui->editor->setYMinorTicks(a_value); });

  connect(m_ui->yAxisMinimum, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
          [this](qreal a_value) { m_ui->editor->setYMinimum(a_value); });

  connect(m_ui->yAxisMaximum, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
          [this](qreal a_value) { m_ui->editor->setYMaximum(a_value); });

  connect(m_ui->seriesTable, &QTableWidget::cellChanged, [this](int a_row, int a_column) {
    auto const xItem = m_ui->seriesTable->item(a_row, 0);
    auto const yItem = m_ui->seriesTable->item(a_row, 1);
    if (!xItem || !yItem) return;

    auto const X = xItem->data(Qt::DisplayRole).toDouble();
    auto const Y = yItem->data(Qt::DisplayRole).toDouble();

    m_ui->editor->changePoint(a_row, QPointF{ X, Y });
  });
  connect(m_ui->live, &QCheckBox::stateChanged, [this](int const a_state) { setLive(a_state == 2); });

  setLive(false);
}

EditorWindow::~EditorWindow()
{
  delete m_ui;
}

void EditorWindow::showEvent(QShowEvent *a_event)
{
  //  qDebug() << Q_FUNC_INFO;

  static bool first{ true };
  if (!first) QWidget::showEvent(a_event);

  first = true;

  m_ui->editor->updatePoints();
}

void EditorWindow::resizeEvent(QResizeEvent *a_event)
{
  //  qDebug() << Q_FUNC_INFO << a_event;
  QWidget::resizeEvent(a_event);
}

void EditorWindow::synchronizeToNode()
{
  auto const editor = m_ui->editor;
  auto const element = static_cast<elements::values::CharacteristicCurve *>(m_characteristicCurve->element());

  element->setXMajorTicks(editor->xMajorTicks());
  element->setXMinorTicks(editor->xMinorTicks());
  element->setXMinimum(static_cast<float>(editor->xMinimum()));
  element->setXMaximum(static_cast<float>(editor->xMaximum()));

  element->setYMajorTicks(editor->yMajorTicks());
  element->setYMinorTicks(editor->yMinorTicks());
  element->setYMinimum(static_cast<float>(editor->yMinimum()));
  element->setYMaximum(static_cast<float>(editor->yMaximum()));

  auto &series = element->series();
  auto const nodeSeries = m_characteristicCurve->series();
  auto const editorSeries = editor->series();
  auto const SIZE = editorSeries->count();

  series.clear();
  nodeSeries->clear();
  for (int i = 0; i < SIZE; ++i) {
    auto const &point = editorSeries->at(i);
    series.push_back(Element::vec2f{ static_cast<float>(point.x()), static_cast<float>(point.y()) });
    nodeSeries->append(point);
  }
}

void EditorWindow::synchronizeFromNode()
{
  auto const editor = m_ui->editor;
  auto const node = m_characteristicCurve;
  auto const element = static_cast<elements::values::CharacteristicCurve *>(m_characteristicCurve->element());

  editor->setXName(node->inputs()[0]->name());
  editor->setXMajorTicks(element->xMajorTicks());
  editor->setXMinorTicks(element->xMinorTicks());
  editor->setXMinimum(static_cast<qreal>(element->xMinimum()));
  editor->setXMaximum(static_cast<qreal>(element->xMaximum()));
  m_ui->xAxisName->setText(node->inputs()[0]->name());
  m_ui->xAxisMajorTicks->setValue(element->xMajorTicks());
  m_ui->xAxisMinorTicks->setValue(element->xMinorTicks());
  m_ui->xAxisMinimum->setValue(static_cast<qreal>(element->xMinimum()));
  m_ui->xAxisMaximum->setValue(static_cast<qreal>(element->xMaximum()));

  editor->setYName(node->outputs()[0]->name());
  editor->setYMajorTicks(element->yMajorTicks());
  editor->setYMinorTicks(element->yMinorTicks());
  editor->setYMinimum(static_cast<qreal>(element->yMinimum()));
  editor->setYMaximum(static_cast<qreal>(element->yMaximum()));
  m_ui->yAxisName->setText(node->outputs()[0]->name());
  m_ui->yAxisMajorTicks->setValue(element->yMajorTicks());
  m_ui->yAxisMinorTicks->setValue(element->yMinorTicks());
  m_ui->yAxisMinimum->setValue(static_cast<qreal>(element->yMinimum()));
  m_ui->yAxisMaximum->setValue(static_cast<qreal>(element->yMaximum()));

  m_xValueType = element->inputs()[0].type == Element::ValueType::eInt ? ValueType::eInt : ValueType::eDouble;
  m_yValueType = element->outputs()[0].type == Element::ValueType::eInt ? ValueType::eInt : ValueType::eDouble;

  recreateSeries();
}

void EditorWindow::setValue(qreal const a_value)
{
  m_ui->editor->setX(a_value);
}

void EditorWindow::setLive(bool const a_live)
{
  m_live = a_live;
  m_ui->load->setDisabled(a_live);
  m_ui->save->setDisabled(a_live);
}

void EditorWindow::addPoint(int const a_index, QPointF const a_point)
{
  auto const seriesTable = m_ui->seriesTable;

  auto const BLOCKED{ seriesTable->blockSignals(true) };

  seriesTable->insertRow(a_index);

  auto const xItem = new QTableWidgetItem;
  xItem->setData(Qt::DisplayRole, a_point.x());
  if (a_index == 0 || a_index == seriesTable->rowCount() - 1) xItem->setFlags(xItem->flags() & ~Qt::ItemIsEditable);
  seriesTable->setItem(a_index, 0, xItem);
  auto const yItem = new QTableWidgetItem;
  yItem->setData(Qt::DisplayRole, a_point.y());
  seriesTable->setItem(a_index, 1, yItem);

  seriesTable->blockSignals(BLOCKED);
}

void EditorWindow::changePoint(int const a_index, QPointF const a_point)
{
  auto const xItem = m_ui->seriesTable->item(a_index, 0);
  xItem->setData(Qt::DisplayRole, a_point.x());
  auto const yItem = m_ui->seriesTable->item(a_index, 1);
  yItem->setData(Qt::DisplayRole, a_point.y());
}

void EditorWindow::removePoint(int const a_index)
{
  m_ui->seriesTable->removeRow(a_index);
}

void EditorWindow::recreateSeries()
{
  auto const editor = m_ui->editor;
  auto const editorSeries = editor->series();
  auto const element = static_cast<elements::values::CharacteristicCurve *>(m_characteristicCurve->element());
  auto const seriesTable = m_ui->seriesTable;
  auto &series = element->series();

  editorSeries->clear();
  seriesTable->setRowCount(0);

  seriesTable->setItemDelegate(new Delegate{ this });

  for (auto &point : series) {
    editorSeries->append(static_cast<qreal>(point.x), static_cast<qreal>(point.y));

    auto const ROW = seriesTable->rowCount();
    addPoint(ROW, QPointF{ static_cast<qreal>(point.x), static_cast<qreal>(point.y) });
  }

  editor->updatePoints();
}

void EditorWindow::setXValueType(ValueType const a_type)
{
  m_xValueType = a_type;
}

void EditorWindow::setYValueType(ValueType const a_type)
{
  m_yValueType = a_type;
}

} // namespace spaghetti::nodes::values::characteristic_curve
