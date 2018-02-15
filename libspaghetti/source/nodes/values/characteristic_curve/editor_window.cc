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

void EditorWindow::synchronizeFromNode()
{
  auto const editor = m_ui->editor;
  auto const editorSeries = editor->series();
  auto const node = m_characteristicCurve;
  auto const element = static_cast<elements::values::CharacteristicCurve *>(m_characteristicCurve->element());

  editor->setXName(node->inputs()[0]->name());
  editor->setXMajorTicks(element->xMajorTicks());
  editor->setXMinorTicks(element->xMinorTicks());
  editor->setXMinimum(static_cast<qreal>(element->xMinimum()));
  editor->setXMaximum(static_cast<qreal>(element->xMaximum()));

  editor->setYName(node->outputs()[0]->name());
  editor->setYMajorTicks(element->yMajorTicks());
  editor->setYMinorTicks(element->yMinorTicks());
  editor->setYMinimum(static_cast<qreal>(element->yMinimum()));
  editor->setYMaximum(static_cast<qreal>(element->yMaximum()));

  editorSeries->clear();

  auto &series = element->series();
  for (auto &point : series) editorSeries->append(static_cast<qreal>(point.x), static_cast<qreal>(point.y));

  editor->updatePoints();
}

void EditorWindow::setValue(qreal const a_value)
{
  m_ui->editor->setX(a_value);
}

} // namespace spaghetti::nodes::values::characteristic_curve
