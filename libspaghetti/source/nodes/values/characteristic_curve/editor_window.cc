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
#include "nodes/values/characteristic_curve/ui_editor_window.h"
#include "nodes/values/characteristic_curve.h"
#include "elements/values/characteristic_curve.h"

#include <algorithm>

#include <QDebug>
#include <QKeyEvent>
#include <QLabel>
#include <QLayout>
#include <QSpinBox>
#include <QtCharts/QLineSeries>

#include <QComboBox>
#include <QGroupBox>
#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>

#include <QGridLayout>
#include <QHBoxLayout>
#include <QTabWidget>
#include <QVBoxLayout>

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
  : QMainWindow{}
  , m_ui{ new Ui::EditorWindow }
  , m_characteristicCurve{ a_characteristicCurve }
{
  m_ui->setupUi(this);

  m_ui->xAxisName->setText(m_ui->editor->xName());
  m_ui->xAxisMajorTicks->setValue(m_ui->editor->xMajorTicks());
  m_ui->xAxisMinorTicks->setValue(m_ui->editor->xMinorTicks());
  m_ui->xAxisMinimum->setValue(m_ui->editor->xMinimum());
  m_ui->xAxisMaximum->setValue(m_ui->editor->xMaximum());

  connect(m_ui->xAxisName, &QLineEdit::textChanged,
          [this](QString const &a_name) { m_ui->editor->setXName(a_name); });

  connect(m_ui->xAxisMajorTicks, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
          [this](int a_value) { m_ui->editor->setXMajorTicks(a_value); });

  connect(m_ui->xAxisMinorTicks, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
          [this](int a_value) { m_ui->editor->setXMinorTicks(a_value); });

  connect(m_ui->xAxisMinimum, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
          [this](qreal a_value) { m_ui->editor->setXMinimum(a_value); });

  connect(m_ui->xAxisMaximum, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
          [this](qreal a_value) { m_ui->editor->setXMaximum(a_value); });

  connect(m_ui->xAxisValueSlider, &QSlider::valueChanged, [this](int a_value) {
    qreal const VALUE = a_value / 1000.0;
    m_ui->editor->setX(VALUE);
  });

  m_ui->yAxes->clear();
  m_ui->series->clear();

  m_ui->removeYAxis->setDisabled(true);
  m_ui->removeSeries->setDisabled(true);
}

EditorWindow::~EditorWindow()
{
  delete m_ui;
}

void EditorWindow::showEvent(QShowEvent *a_event)
{
  //  qDebug() << Q_FUNC_INFO;

  static bool first{ true };
  if (!first) QMainWindow::showEvent(a_event);

  first = true;

  m_ui->editor->updatePoints();
}

void EditorWindow::resizeEvent(QResizeEvent *a_event)
{
  //  qDebug() << Q_FUNC_INFO << a_event;
  QMainWindow::resizeEvent(a_event);
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
