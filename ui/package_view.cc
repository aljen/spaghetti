#include "ui/package_view.h"
#include <QDebug>
#include <QGraphicsScene>
#include <QVBoxLayout>
#include "elements/package.h"
#include "ui/nodes_view.h"

PackageView::PackageView(elements::Package *const a_package)
  : QWidget{}
  , m_scene{ new QGraphicsScene{ this } }
  , m_nodesView{ new NodesView{ m_scene, this } }
  , m_package{ (a_package ? a_package : new elements::Package) }
  , m_standalone{ !a_package }
{
  setObjectName("PackageView");

  m_scene->setSceneRect(-32000, -32000, 64000, 64000);
  m_scene->setObjectName("PackageViewScene");

  QBrush brush{ QColor(169, 169, 169, 32) };
  //  QBrush brush{ QColor(244, 53, 64, 255) }; // RED
  //  QBrush brush{ QColor(232, 0, 99, 255) }; // PINK
  //  QBrush brush{ QColor(154, 41, 169, 255) }; // PURPLE
  //  QBrush brush{ QColor(101, 63, 176, 255) }; // DEEP PURPLE
  //  QBrush brush{ QColor(62, 84, 174, 255) }; // INDIGO
  //  QBrush brush{ QColor(29, 151, 237, 255) }; // BLUE
  //  QBrush brush{ QColor(0, 170, 238, 255) }; // LIGHT BLUE
  //  QBrush brush{ QColor(0, 188, 208, 255) }; // CYAN
  //  QBrush brush{ QColor(0, 149, 134, 255) }; // TEAL
  //  QBrush brush{ QColor(75, 173, 88, 255) }; // GREEN
  //  QBrush brush{ QColor(137, 193, 86, 255) }; // LIGHT GREEN
  //  QBrush brush{ QColor(203, 217, 81, 255) }; // LIME
  //  QBrush brush{ QColor(254, 231, 86, 255) }; // YELLOW
  //  QBrush brush{ QColor(254, 187, 59, 255) }; // AMBER
  //  QBrush brush{ QColor(254, 144, 50, 255) }; // ORANGE
  //  QBrush brush{ QColor(254, 74, 53, 255) }; // DEEP ORANGE
  //  QBrush brush{ QColor(120, 83, 74, 255) }; // BROWN
  //  QBrush brush{ QColor(156, 156, 156, 255) }; // GREY
  //  QBrush brush{ QColor(95, 124, 136, 255) }; // BLUE GREY
  //  QBrush brush{ QColor(58, 66, 71, 255) }; // DARK GREY
  brush.setStyle(Qt::DiagCrossPattern);
  m_scene->setBackgroundBrush(brush);

  QVBoxLayout *const layout{ new QVBoxLayout{ this } };
  layout->addWidget(m_nodesView);
  setLayout(layout);

  m_package->startDispatchThread();
}

PackageView::~PackageView()
{
  if (m_standalone) {
    qDebug() << "Stopping thread";
    m_package->quitDispatchThread();
    delete m_package;
  }
}

bool PackageView::canClose()
{
  return true;
}

void PackageView::center()
{
  m_nodesView->centerOn(0.0, 0.0);
}
