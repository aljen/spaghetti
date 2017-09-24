#include "editor.h"
#include "ui_editor.h"

#include "ui/socket_item.h"
#include "ui/link_item.h"

#include "ui/config.h"
#include "ui/elements_list.h"

//#include <json/json.hpp>

#include <QPainterPath>
#include <QGraphicsItem>
#include <QDebug>
#include <QDockWidget>
#include <QGraphicsScene>
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

Editor::Editor(QWidget* a_parent)
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

  Config::get();

  core::register_internal_elements();

  populateElementsList();
  newPackage();

#if 0
  m_ui->graphicsView->setScene(scene);

  QListWidgetItem *item{};

  item = new QListWidgetItem{ "Inputs" };
  item->setData(Qt::UserRole, Elements::eInputs);
  item->setIcon(QIcon(":/inputs.png"));
  m_ui->listWidget->addItem(item);

  item = new QListWidgetItem{ "Outputs" };
  item->setData(Qt::UserRole, Elements::eOutputs);
  item->setIcon(QIcon(":/outputs.png"));
  m_ui->listWidget->addItem(item);

  item = new QListWidgetItem{ "And Gate" };
  item->setData(Qt::UserRole, Elements::eAndGate);
  item->setIcon(QIcon(":/and_gate.png"));
  m_ui->listWidget->addItem(item);

  item = new QListWidgetItem{ "Or Gate" };
  item->setData(Qt::UserRole, Elements::eOrGate);
  item->setIcon(QIcon(":/or_gate.png"));
  m_ui->listWidget->addItem(item);

  item = new QListWidgetItem{ "Not Gate" };
  item->setData(Qt::UserRole, Elements::eNotGate);
  item->setIcon(QIcon(":/not_gate.png"));
  m_ui->listWidget->addItem(item);

  m_ui->listWidget->setDragEnabled(true);
#endif

#if 0
  using json = nlohmann::json;
  std::ifstream config_file{ "config.json" };
  json const config{ json::parse(config_file) };
  assert(config_file.is_open());
  config_file.close();

  json const elements{ config["elements"] };
  json const connections{ config["connections"] };

  std::cerr << "config: " << config << "\n";
  std::cerr << "elements: " << elements << "\n";
  std::cerr << "connections: " << connections << "\n";

  for (auto it = elements.begin(); it != elements.end(); ++it) {
    json const element{ *it };

    AbstractNode *node{};
    int const type{ element["type"] };
    switch (type) {
      case Elements::eInputs: {
        node = new InputsGroup{ QString::fromStdString(it.key()), QString::fromStdString(element.dump()) };
        break;
      }
      case Elements::eOutputs: {
        node = new OutputsGroup{ QString::fromStdString(it.key()), QString::fromStdString(element.dump()) };
        break;
      }
      case Elements::eAndGate: {
        node = new AndGate{ QString::fromStdString(it.key()), QString::fromStdString(element.dump()) };
        break;
      }
      case Elements::eOrGate: {
        node = new OrGate{ QString::fromStdString(it.key()), QString::fromStdString(element.dump()) };
        break;
      }
      case Elements::eNotGate: {
        node = new NotGate{ QString::fromStdString(it.key()), QString::fromStdString(element.dump()) };
        break;
      }
    }
    assert(node);
    m_ui->graphicsView->scene()->addItem(node);
  }

  QList<QGraphicsItem *> items{ scene->items() };

  for (auto it = connections.begin(); it != connections.end(); ++it) {
    json const element{ *it };
    json const connect{ element["connect"] };
    json const to{ element["to"] };

    int const fromId{ connect["id"] };
    int const fromSocketId{ connect["socket"] };
    int const toId{ to["id"] };
    int const toSocketId{ to["socket"] };

    auto findItemById = [&items](int aId) -> AbstractNode* {
      auto found = std::find_if(items.begin(), items.end(), [&aId](QGraphicsItem *aItem){
        if (aItem->type() == NODE_TYPE) {
          AbstractNode *node{ static_cast<AbstractNode*>(aItem) };
          if (node->id() == aId)
            return true;
        }
        return false;
      });

      assert(found != items.end());
      return static_cast<AbstractNode*>(*found);
    };

    AbstractNode *const fromItem{ findItemById(fromId) };
    AbstractNode *const toItem{ findItemById(toId) };
    fromItem->connectTo(fromSocketId, toItem, toSocketId);
  }
#endif
}

Editor::~Editor()
{
  delete m_ui;
}

void Editor::newPackage(bool a_checked)
{
  (void)a_checked;

  PackageView *const package{ new PackageView };
  int const index{ m_ui->tabWidget->addTab(package, "New package") };
  m_ui->tabWidget->setCurrentIndex(index);
}

void Editor::tabCloseRequested(int a_index)
{
  QTabWidget *const tab{ m_ui->tabWidget };
  QWidget *const widget{ tab->widget(a_index) };
  PackageView *const packageView{ reinterpret_cast<PackageView*>(widget) };

  if (packageView->canClose()) {
    tab->removeTab(a_index);
    delete packageView;
  }
}

void Editor::tabChanged(int a_index)
{
  if (a_index >= 0)
    m_currentPackageView = qobject_cast<PackageView*>(m_ui->tabWidget->widget(a_index));
  else
    m_currentPackageView = nullptr;
}

void Editor::populateElementsList()
{
  core::Registry const &registry{ core::Registry::get() };
  auto const &elements{ registry.elements() };

  for (auto &&element : elements) {
    auto const &info = element.second;
    std::string const path{ info.data->path };
    std::string category{ path };

    if (auto const it = path.find_first_of('/'); it != std::string::npos)
      category = path.substr(0, it);
    category[0] = static_cast<char>(std::toupper(category[0]));

    addElement(QString::fromStdString(category), *info.data);
  }
}

void Editor::addElement(QString a_category, core::MetaData a_metaData)
{
  QToolBox *const toolbox{ m_ui->elementsList };

  ElementsList *list{};

  int const count{ toolbox->count() };
  for (int i = 0; i < count; ++i) {
    QString const text{ toolbox->itemText(i) };
    if (text != a_category) continue;

    list = qobject_cast<ElementsList*>(toolbox->widget(i));
    assert(list);
    break;
  }

  if (list == nullptr) {
    list = new ElementsList{ this };
    toolbox->addItem(list, a_category);
  }

  QListWidgetItem *const item{ new QListWidgetItem{ QString::fromLatin1(a_metaData.name) } };
  item->setData(ElementsList::eMetaDataPath, QString::fromLatin1(a_metaData.path));
  item->setData(ElementsList::eMetaDataName, QString::fromLatin1(a_metaData.name));
  item->setData(ElementsList::eMetaDataIcon, QString::fromLatin1(a_metaData.icon));
  item->setIcon(QIcon(QString::fromLatin1(a_metaData.icon)));
  list->addItem(item);

  list->sortItems();
}

void Editor::aboutToQuit()
{
//  m_package.quitDispatchThread();
}

void Editor::itemMoved(QGraphicsItem * const aItem)
{
//  m_pointManager->setValue(m_idToProperty["pos"], aItem->pos());
}
