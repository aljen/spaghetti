#include "editor.h"
#include "ui_editor.h"

#include "ui/link_item.h"
#include "ui/socket_item.h"

#include "ui/colors.h"
#include "ui/elements_list.h"

#include <QDebug>
#include <QDockWidget>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QPainterPath>
#include <QPolygonF>
#include <QPushButton>
#include <QToolBox>
#include <cctype>
#include <fstream>
#include <typeinfo>
#include <vector>

#include "core/registry.h"
#include "elements/logic/all.h"
#include "nodes/node.h"
#include "ui/package_view.h"

Editor::Editor(QWidget *a_parent)
  : QMainWindow{ a_parent }
  , m_ui{ new Ui::Editor }
{
  setObjectName("SpaghettiEditor");
  m_ui->setupUi(this);
  m_ui->elementsList->removeItem(0);
  m_ui->tabWidget->removeTab(0);

  connect(m_ui->actionNew, &QAction::triggered, this, &Editor::newPackage);
  connect(m_ui->tabWidget, &QTabWidget::tabCloseRequested, this, &Editor::tabCloseRequested);
  connect(m_ui->tabWidget, &QTabWidget::currentChanged, this, &Editor::tabChanged);

  populateElementsList();
  newPackage();
}

Editor::~Editor()
{
  delete m_ui;
}

void Editor::newPackage()
{
  PackageView *const package{ new PackageView };
  int const index{ m_ui->tabWidget->addTab(package, "New package") };
  m_ui->tabWidget->setCurrentIndex(index);
}

void Editor::tabCloseRequested(int a_index)
{
  QTabWidget *const tab{ m_ui->tabWidget };
  QWidget *const widget{ tab->widget(a_index) };
  PackageView *const packageView{ reinterpret_cast<PackageView *>(widget) };

  if (packageView->canClose()) {
    tab->removeTab(a_index);
    delete packageView;
  }
}

void Editor::tabChanged(int a_index)
{
  if (a_index >= 0)
    m_currentPackageView = qobject_cast<PackageView *>(m_ui->tabWidget->widget(a_index));
  else
    m_currentPackageView = nullptr;
}

void Editor::populateElementsList()
{
  core::Registry const &registry{ core::Registry::get() };
  auto const &elements{ registry.elements() };

  for (auto &&element : elements) {
    auto const &info = element.second;
    std::string const path{ info.type };
    std::string category{ path };

    if (auto const it = path.find_first_of('/'); it != std::string::npos) category = path.substr(0, it);
    category[0] = static_cast<char>(std::toupper(category[0]));

    addElement(QString::fromStdString(category), QString::fromStdString(info.name), QString::fromStdString(info.type),
               QString::fromStdString(info.icon));
  }
}

void Editor::addElement(QString a_category, QString a_name, QString a_type, QString a_icon)
{
  QToolBox *const toolbox{ m_ui->elementsList };

  ElementsList *list{};

  int const count{ toolbox->count() };
  for (int i = 0; i < count; ++i) {
    QString const text{ toolbox->itemText(i) };
    if (text != a_category) continue;

    list = qobject_cast<ElementsList *>(toolbox->widget(i));
    assert(list);
    break;
  }

  if (list == nullptr) {
    list = new ElementsList{ this };
    toolbox->addItem(list, a_category);
  }

  QListWidgetItem *const item{ new QListWidgetItem{ a_name } };
  item->setData(ElementsList::eMetaDataType, a_type);
  item->setData(ElementsList::eMetaDataName, a_name);
  item->setData(ElementsList::eMetaDataIcon, a_icon);
  item->setIcon(QIcon(a_icon));
  list->addItem(item);

  list->sortItems();
}

void Editor::aboutToQuit() {}

void Editor::showEvent(QShowEvent *a_event)
{
  static bool s_firstTime{ true };

  if (s_firstTime) {
    s_firstTime = false;
    auto *const tab = m_ui->tabWidget;
    auto const index = tab->currentIndex();
    auto packageView = qobject_cast<PackageView *const>(tab->widget(index));
    packageView->center();
  }

  QMainWindow::showEvent(a_event);
}
