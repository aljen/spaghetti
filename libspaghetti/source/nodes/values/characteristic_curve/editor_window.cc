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

#include <algorithm>

#include <QDebug>
#include <QKeyEvent>
#include <QLabel>
#include <QLayout>
#include <QSpinBox>

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

EditorWindow::EditorWindow(QWidget *parent)
  : QMainWindow{ parent }
  , m_ui{ new Ui::EditorWindow }
{
  m_ui->setupUi(this);

  m_ui->xAxisName->setText(m_ui->editor->inputName());
  m_ui->xAxisMajorTicks->setValue(m_ui->editor->inputMajorTicks());
  m_ui->xAxisMinorTicks->setValue(m_ui->editor->inputMinorTicks());
  m_ui->xAxisMinimum->setValue(m_ui->editor->inputMinimum());
  m_ui->xAxisMaximum->setValue(m_ui->editor->inputMaximum());

  connect(m_ui->xAxisName, &QLineEdit::textChanged,
          [this](QString const &a_name) { m_ui->editor->setInputName(a_name); });

  connect(m_ui->xAxisMajorTicks, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
          [this](int a_value) { m_ui->editor->setInputMajorTicks(a_value); });

  connect(m_ui->xAxisMinorTicks, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
          [this](int a_value) { m_ui->editor->setInputMinorTicks(a_value); });

  connect(m_ui->xAxisMinimum, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
          [this](qreal a_value) { m_ui->editor->setInputMinimum(a_value); });

  connect(m_ui->xAxisMaximum, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
          [this](qreal a_value) { m_ui->editor->setInputMaximum(a_value); });

  connect(m_ui->xAxisValueSlider, &QSlider::valueChanged, [this](int a_value) {
    qreal const VALUE = a_value / 1000.0;
    m_ui->editor->setInput(VALUE);
  });

  connect(m_ui->addYAxis, &QPushButton::clicked, [this]() { addAxis(); });

  connect(m_ui->removeYAxis, &QPushButton::clicked, [this]() { removeAxis(); });

  connect(m_ui->addSeries, &QPushButton::clicked, [this]() { addSeries(); });

  connect(m_ui->removeSeries, &QPushButton::clicked, [this]() { removeSeries(); });

  m_ui->yAxes->clear();
  m_ui->series->clear();

  m_ui->removeYAxis->setDisabled(true);
  m_ui->removeSeries->setDisabled(true);

  addAxis();
  addSeries();
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
#if 0
  m_ui->editor->createPoints();
#endif
}

void EditorWindow::resizeEvent(QResizeEvent *a_event)
{
  //  qDebug() << Q_FUNC_INFO << a_event;
  QMainWindow::resizeEvent(a_event);
}

void EditorWindow::addAxis()
{
  Axis *const axis{ new Axis };
  m_ui->editor->addAxis(axis);

  QString axisName = QString("Y #%1").arg(m_ui->yAxes->count() + 1);
  axis->setName(axisName);

  //  auto const tab = new AxisWidget{ axis, m_ui->yAxes };
  //  auto const index = m_ui->yAxes->addTab(tab, axisName);
  //  m_ui->yAxes->setCurrentIndex(index);

  //  m_ui->removeYAxis->setEnabled(true);
}

void EditorWindow::removeAxis()
{
  auto const index = m_ui->yAxes->currentIndex();
  auto const tab = m_ui->yAxes->widget(index);
  m_ui->yAxes->removeTab(index);
  delete tab;

  m_ui->editor->removeAxis(index);

  if (m_ui->yAxes->count() == 0) m_ui->removeYAxis->setDisabled(true);
}

void EditorWindow::addSeries()
{
  Series *const series{ new Series };
  m_ui->editor->addSeries(series);

  QString seriesName = QString("Series #%1").arg(m_ui->series->count() + 1);
  series->setName(seriesName);

#if 0
  auto const tab = new QWidget{};
  auto const tabLayout = new QVBoxLayout{};
  tab->setLayout(tabLayout);

  auto const nameBox = new QGroupBox{ "Name" };
  auto const nameBoxLayout = new QVBoxLayout{};
  nameBox->setLayout(nameBoxLayout);

  auto const name = new QLineEdit{ seriesName };
  nameBoxLayout->addWidget(name);

  tabLayout->addWidget(nameBox);

  auto const yAxisBox = new QGroupBox{ "Y Axis" };
  auto const yAxisBoxLayout = new QVBoxLayout{};
  yAxisBox->setLayout(yAxisBoxLayout);

  auto const yAxis = new QComboBox{};

  yAxis->addItem("<None>", VPtr<Axis>::asQVariant(nullptr));
  auto axes = m_ui->editor->axes();
  for (int i = 0; i < axes.count(); ++i) yAxis->addItem(axes.at(i)->name(), VPtr<Axis>::asQVariant(axes.at(i)));

  yAxisBoxLayout->addWidget(yAxis);

  tabLayout->addWidget(yAxisBox);

  auto const csvBox = new QGroupBox{ "CSV" };
  auto const csvBoxLayout = new QGridLayout{};

  csvBox->setLayout(csvBoxLayout);

  auto const csvImport = new QPushButton{ "Import" };
  auto const csvExport = new QPushButton{ "Export" };
  auto const csvToClipboard = new QPushButton{ "To Clipboard" };
  auto const csvFromClipboard = new QPushButton{ "From Clipboard" };
  csvBoxLayout->addWidget(csvImport, 0, 0);
  csvBoxLayout->addWidget(csvExport, 0, 1);
  csvBoxLayout->addWidget(csvToClipboard, 1, 0);
  csvBoxLayout->addWidget(csvFromClipboard, 1, 1);

  tabLayout->addWidget(csvBox);

  auto const templatesBox = new QGroupBox{ "Templates" };
  auto const templatesBoxLayout = new QGridLayout{};
  templatesBox->setLayout(templatesBoxLayout);

  auto const templatesTypeLabel = new QLabel{ "Type" };
  auto const templatesType = new QComboBox{};

  for (size_t i = 0; i < static_cast<size_t>(Series::GeneratorType::eCount); ++i) {
    templatesType->addItem(generatorTypeToName(static_cast<Series::GeneratorType>(i)), i);
  }

  auto const templatesStartLabel = new QLabel{ "Start" };
  auto const templatesStart = new QDoubleSpinBox{};

  templatesStart->setMinimum(-200.0);
  templatesStart->setMaximum(200.0);
  templatesStart->setSuffix("%");

  auto const templatesStopLabel = new QLabel{ "Stop" };
  auto const templatesStop = new QDoubleSpinBox{};

  templatesStop->setMinimum(-200.0);
  templatesStop->setMaximum(200.0);
  templatesStop->setValue(100.0);
  templatesStop->setSuffix("%");

  auto const templatesStepsLabel = new QLabel{ "Steps" };
  auto const templatesSteps = new QSpinBox{};

  templatesSteps->setMinimum(2);
  templatesSteps->setMaximum(1000);
  templatesSteps->setValue(10);

  auto const templatesGenerate = new QPushButton{ "Generate" };

  templatesBoxLayout->addWidget(templatesTypeLabel, 0, 0);
  templatesBoxLayout->addWidget(templatesType, 0, 1);
  templatesBoxLayout->addWidget(templatesStartLabel, 1, 0);
  templatesBoxLayout->addWidget(templatesStart, 1, 1);
  templatesBoxLayout->addWidget(templatesStopLabel, 2, 0);
  templatesBoxLayout->addWidget(templatesStop, 2, 1);
  templatesBoxLayout->addWidget(templatesStepsLabel, 3, 0);
  templatesBoxLayout->addWidget(templatesSteps, 3, 1);
  templatesBoxLayout->addWidget(templatesGenerate, 4, 0, 1, 2);

  connect(templatesGenerate, &QPushButton::clicked,
          [series, templatesType, templatesSteps, templatesStart, templatesStop]() {
            int const steps = templatesSteps->value();
            qreal const start = templatesStart->value();
            qreal const stop = templatesStop->value();
            series->generate(static_cast<Series::GeneratorType>(templatesType->currentData().toInt()), steps, start,
                             stop);
          });

  tabLayout->addWidget(templatesBox);

  auto const table = new QTableWidget{};
  tabLayout->addWidget(table);
#endif

  //  auto tab = new SeriesWidget{ series };

  //  auto const index = m_ui->series->addTab(tab, seriesName);
  //  m_ui->series->setCurrentIndex(index);

  //  m_ui->removeSeries->setEnabled(true);
}

void EditorWindow::removeSeries()
{
  auto const index = m_ui->series->currentIndex();
  auto const tab = m_ui->series->widget(index);
  m_ui->series->removeTab(index);
  delete tab;

  m_ui->editor->removeSeries(index);

  if (m_ui->series->count() == 0) m_ui->removeSeries->setDisabled(true);
}

} // namespace spaghetti::nodes::values::characteristic_curve
