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

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QCheckBox>
#include <QDebug>
#include <chrono>
#include <iostream>

#include "core/registry.h"
#include "elements/logic/all.h"

MainWindow::MainWindow(QWidget *a_parent)
  : QMainWindow{ a_parent }
  , m_ui{ new Ui::MainWindow }
{
  m_ui->setupUi(this);

  core::register_internal_elements();

  m_package.startDispatchThread();

  using namespace elements;

  auto el1 = m_package.add("logic/switch");
  el1->setName("Switch A");
  auto el2 = m_package.add("logic/switch");
  el2->setName("Switch B");
  auto el3 = m_package.add("logic/nand");
  el3->setName("Nand A");
  el3->onChange([this](Element *const a_element) {
    bool const v{ std::get<bool>(a_element->outputs()[0].value) };
    m_ui->ag1->setCheckState(v ? Qt::Checked : Qt::Unchecked);
  });
  el3->connect(el1->id(), 0, 0);
  el3->connect(el2->id(), 0, 1);

  auto ng1 = m_package.add("logic/not");
  ng1->setName("Not");
  ng1->onChange([this](Element *const a_element) {
    bool const v{ std::get<bool>(a_element->outputs()[0].value) };
    m_ui->ng->setCheckState(v ? Qt::Checked : Qt::Unchecked);
  });
  ng1->connect(el3->id(), 0, 0);

  auto el4 = m_package.add("logic/switch");
  el4->setName("Switch C");
  auto el5 = m_package.add("logic/switch");
  el5->setName("Switch D");
  auto el6 = m_package.add("logic/nor");
  el6->setName("Nor");
  el6->onChange([this](Element *const a_element) {
    bool const v{ std::get<bool>(a_element->outputs()[0].value) };
    m_ui->ag2->setCheckState(v ? Qt::Checked : Qt::Unchecked);
  });
  el6->connect(el4->id(), 0, 0);
  el6->connect(el5->id(), 0, 1);

  auto el7 = m_package.add("logic/and");
  el7->setName("And C");
  el7->onChange([this](Element *const a_element) {
    bool const v{ std::get<bool>(a_element->outputs()[0].value) };
    m_ui->ag3->setCheckState(v ? Qt::Checked : Qt::Unchecked);
  });
  el7->connect(ng1->id(), 0, 0);
  el7->connect(el6->id(), 0, 1);

  (void)el1; (void)el2; (void)el3; (void)ng1; (void)el4; (void)el5; (void)el6; (void)el7;

  connect(m_ui->swA, &QCheckBox::stateChanged, [el1](int value) { reinterpret_cast<logic::Switch*>(el1)->set(value == 2); });
  connect(m_ui->swB, &QCheckBox::stateChanged, [el2](int value) { reinterpret_cast<logic::Switch*>(el2)->set(value == 2); });
  connect(m_ui->swC, &QCheckBox::stateChanged, [el4](int value) { reinterpret_cast<logic::Switch*>(el4)->set(value == 2); });
  connect(m_ui->swD, &QCheckBox::stateChanged, [el5](int value) { reinterpret_cast<logic::Switch*>(el5)->set(value == 2); });
}

MainWindow::~MainWindow()
{
  delete m_ui;
}

void MainWindow::aboutToQuit()
{
  qDebug() << __PRETTY_FUNCTION__ << ">";
  m_package.quitDispatchThread();
  qDebug() << __PRETTY_FUNCTION__ << "<";
}
