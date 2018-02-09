#include "scale_widget.h"
#include "ui_scale_widget.h"

ScaleWidget::ScaleWidget(QWidget *const a_parent)
  : QWidget{ a_parent }
  , m_ui{ new Ui::ScaleWidget }
{
  m_ui->setupUi(this);
}

ScaleWidget::~ScaleWidget()
{
  delete m_ui;
}
