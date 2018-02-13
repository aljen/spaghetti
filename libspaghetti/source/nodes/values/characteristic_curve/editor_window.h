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

#pragma once
#ifndef NODES_VALUES_CHARACTERISTIC_CURVE_EDITOR_WINDOW_H
#define NODES_VALUES_CHARACTERISTIC_CURVE_EDITOR_WINDOW_H

#include <QGraphicsItem>
#include <QMainWindow>

namespace QtCharts {
class QChart;
class QLineSeries;
} // namespace QtCharts

namespace spaghetti::nodes::values::characteristic_curve {

namespace Ui {
class EditorWindow;
}

class EditorWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit EditorWindow(QWidget *const a_parent = nullptr);
  ~EditorWindow();

  void showEvent(QShowEvent *a_event) override;
  void resizeEvent(QResizeEvent *a_event) override;

  void addAxis();
  void removeAxis();

  void addSeries();
  void removeSeries();

 private:
  Ui::EditorWindow *const m_ui{};
};

} // namespace spaghetti::nodes::values::characteristic_curve

#endif // NODES_VALUES_CHARACTERISTIC_CURVE_EDITOR_WINDOW_H
