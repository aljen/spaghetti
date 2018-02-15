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

#include "nodes/values/characteristic_curve/axis.h"

#include <QtCharts/QValueAxis>

using namespace QtCharts;

namespace spaghetti::nodes::values::characteristic_curve {

Axis::Axis()
  : m_axis{ new QValueAxis }
{
  setName("Name");
  setMinimum(0.0);
  setMaximum(100.0);

  m_majorTicks = m_axis->tickCount();
  m_minorTicks = m_axis->minorTickCount();
}

Axis::~Axis()
{
  delete m_axis;
}

void Axis::setName(QString a_name)
{
  m_name = a_name;
  m_axis->setTitleText(a_name);
}

void Axis::setType(const Axis::Type a_type)
{
  m_type = a_type;
}

void Axis::setMajorTicks(const int a_majorTicks)
{
  m_axis->setTickCount(a_majorTicks);
}

void Axis::setMinorTicks(const int a_minorTicks)
{
  m_axis->setMinorTickCount(a_minorTicks);
}

void Axis::setMinimum(const qreal a_min)
{
  m_axis->setMin(a_min);
}

void Axis::setMaximum(const qreal a_max)
{
  m_axis->setMax(a_max);
}

void Axis::attachSeries(Series *const a_series)
{
  m_series.push_back(a_series);
}

void Axis::detachSeries(Series *const a_series)
{
  m_series.removeOne(a_series);
}

} // namespace spaghetti::nodes::values::characteristic_curve
