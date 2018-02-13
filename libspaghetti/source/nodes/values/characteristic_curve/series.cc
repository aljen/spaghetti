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

#include "nodes/values/characteristic_curve/series.h"

#include <QDebug>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>

#include "nodes/values/characteristic_curve/axis.h"
#include "nodes/values/characteristic_curve/generators.h"
#include "nodes/values/characteristic_curve/point.h"
#include "spaghetti/utils.h"

using namespace QtCharts;

namespace spaghetti::nodes::values::characteristic_curve {

Series::Series()
  : m_series{ new QLineSeries }
{
  setName("Series");

  m_min = 0.0;
  m_max = 1.0;
}

Series::~Series()
{
  delete m_series;
}

void Series::setName(const QString a_name)
{
  m_series->setName(a_name);
}

void Series::setInput(qreal const a_input)
{
  m_input = a_input;

  QPointF previous{ m_series->at(0) };
  QPointF current{};

  int const SIZE = m_series->count();
  for (int i = 0; i < SIZE; ++i) {
    current = m_series->at(i);
    if (current.x() < m_input)
      previous = current;
    else
      break;
  }

  qreal y{};
  qreal percent{};

  if (m_input == previous.x())
    y = previous.y();
  else if (m_input == current.x())
    y = current.y();
  else {
    percent = (m_input - previous.x()) / (current.x() - previous.x());
    y = lerp(previous.y(), current.y(), percent);
  }
  //  qDebug() << "percent:" << percent << "y:" << y;

  QPointF const position{ m_input, y };
  //  m_currentValue->setPos(chart()->mapToPosition(position));

  m_output = y;
}

void Series::setRange(qreal const a_min, qreal const a_max)
{
  m_min = a_min;
  m_max = a_max;
}

void Series::generate(const Series::GeneratorType a_type, const int a_steps, qreal const a_start, qreal const a_stop)
{
  qreal const step = (m_max - m_min) / static_cast<qreal>(a_steps - 1);

  GeneratorFunc generator{};

  switch (a_type) {
    case GeneratorType::eQuadraticEasyIn: generator = gen_quadratic_easy_in; break;
    case GeneratorType::eQuadraticEasyOut: generator = gen_quadratic_easy_out; break;
    case GeneratorType::eQuadraticEasyInOut: generator = gen_quadratic_easy_in_out; break;

    case GeneratorType::eCubicEasyIn: generator = gen_cubic_easy_in; break;
    case GeneratorType::eCubicEasyOut: generator = gen_cubic_easy_out; break;
    case GeneratorType::eCubicEasyInOut: generator = gen_cubic_easy_in_out; break;

    case GeneratorType::eQuarticEasyIn: generator = gen_quartic_easy_in; break;
    case GeneratorType::eQuarticEasyOut: generator = gen_quartic_easy_out; break;
    case GeneratorType::eQuarticEasyInOut: generator = gen_quartic_easy_in_out; break;

    case GeneratorType::eQuinticEasyIn: generator = gen_quintic_easy_in; break;
    case GeneratorType::eQuinticEasyOut: generator = gen_quintic_easy_out; break;
    case GeneratorType::eQuinticEasyInOut: generator = gen_quintic_easy_in_out; break;

    case GeneratorType::eSinusoidalEasyIn: generator = gen_sinusoidal_easy_in; break;
    case GeneratorType::eSinusoidalEasyOut: generator = gen_sinusoidal_easy_out; break;
    case GeneratorType::eSinusoidalEasyInOut: generator = gen_sinusoidal_easy_in_out; break;

    case GeneratorType::eExponentialEasyIn: generator = gen_exponential_easy_in; break;
    case GeneratorType::eExponentialEasyOut: generator = gen_exponential_easy_out; break;
    case GeneratorType::eExponentialEasyInOut: generator = gen_exponential_easy_in_out; break;

    case GeneratorType::eCircularEasyIn: generator = gen_circular_easy_in; break;
    case GeneratorType::eCircularEasyOut: generator = gen_circular_easy_out; break;
    case GeneratorType::eCircularEasyInOut: generator = gen_circular_easy_in_out; break;

    case GeneratorType::eRandom: generator = gen_random; break;

    default: break;
  }

  m_series->clear();

  qreal x = m_min;
  for (int i = 0; i < a_steps; ++i) {
    if (i == 0)
      x = m_min;
    else if (i == a_steps - 1)
      x = m_max;
    else
      x += step;
    qreal y = generator(x, a_start, a_stop);
    m_series->append(x, y);
  }
}

void Series::attachAxis(Axis *const a_axis)
{
  if (a_axis == m_axis) return;

  m_series->attachAxis(a_axis->axis());

  m_axis = a_axis;
}

void Series::detachAxis()
{
  if (m_axis == nullptr) return;

  m_series->detachAxis(m_axis->axis());
  m_axis = nullptr;
}

} // namespace spaghetti::nodes::values::characteristic_curve
