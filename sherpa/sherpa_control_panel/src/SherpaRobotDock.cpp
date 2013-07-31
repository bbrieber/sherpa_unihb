#include "../include/sherpa_control_panel/SherpaRobotDock.hpp"
#include "../include/sherpa_control_panel/SherpaRobotTreeModel.hpp"
#include <QLabel>
#include <QStandardItemModel>
#include <QStandardItem>

#include <iostream>

/*
#define SHERPA_CHECKBOX 1
#define SHERPA_COLOR 2
#define SHERPA_EDITABLE 4
#define SHERPA_COMBO 8
*/


namespace sherpa_control_panel {

SherpaRobotDock::SherpaRobotDock(QWidget* parent, Qt::WindowFlags flags): QDockWidget(tr("Robot Settings"),parent, flags),parent(parent)
{

  createTree();
  //QLabel *tmp = new QLabel( this );
  //tmp->setText( "World Settings" );
  setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea | Qt::TopDockWidgetArea );
  setWidget(tree);
}


SherpaRobotDock::~SherpaRobotDock()
{

}


void SherpaRobotDock::updateRobotProperties()
{

}

void SherpaRobotDock::createTree()
{
  tree = new QTreeView(this);
  
  QTreeView view ;
  SherpaRobotTreeModel *model = new SherpaRobotTreeModel(this);
 
  
  view.setModel(model);
  //setWidget(&view);
  standardModel = new QStandardItemModel ;
  rootNode = standardModel->invisibleRootItem();
  
  
  addRobot("Rover_1");
  addRobot("Quadrocopter_1");
  addRobot("Quadrocopter_2");
  addRobot("Helicopter_1");
    /*new QStandardItem("Rover");
    QList<QStandardItem *> roverItems1;
    roverItems1 << new QStandardItem("show current position");
    QStandardItem *roverItem1 = new QStandardItem();
    roverItem1->setCheckable(true);
    roverItems1 << roverItem1;
    
    QStandardItem *roverItem2 = new QStandardItem("show target position");
    roverItem2->setCheckable(true);
    QStandardItem *roverItem3 = new QStandardItem("show Path");
    roverItem3->setCheckable(true);
  
  QStandardItem *quad1Item = new QStandardItem("Quad1");
  QStandardItem *quad2Item= new QStandardItem("Quad2");
  QStandardItem *humanItem = new QStandardItem("Human");
  QStandardItem *heliItem = new QStandardItem("Helicopter");
  */
  
    //roverItem->appendRow(roverItem2);
    //roverItem->appendRow(roverItem3);
  //rootNode->    appendRow(quad1Item);
  //rootNode->    appendRow(quad2Item);
 // rootNode->    appendRow(humanItem);
  //rootNode->    appendRow(heliItem);
  
     //register the model
  tree->setModel(standardModel);
  //tree->expandAll();
  tree->setHeaderHidden(true);
  tree->resizeColumnToContents(0);;
     //selection changes shall trigger a slot
     QItemSelectionModel *selectionModel= tree->selectionModel();
     tree->setAlternatingRowColors(true);
    // connect(selectionModel, SIGNAL(selectionChanged (const QItemSelection &, const QItemSelection &)),
      //       parent, SLOT(robotSelectionChangedSlot(const QItemSelection &, const QItemSelection &)));
     connect(tree, SIGNAL(expanded(QModelIndex)),
             this, SLOT(updateTree()));
     connect(tree, SIGNAL(collapsed(QModelIndex)),
             this, SLOT(updateTree()));
     //connect(tree, SIGNAL(activated (QModelIndex )),
       //      this, SLOT(test()));
     connect(tree, SIGNAL(clicked (QModelIndex )),
             this, SLOT(robotClicked(const QModelIndex& )));

}

QList< QStandardItem* > SherpaRobotDock::prepareSubRow(const QString& name, const Sherpa_Widget_type type)
{
    QList<QStandardItem *> list;
    list << new QStandardItem(name);
    QStandardItem * item;
    item = new QStandardItem("");
    item->setEditable(false);
       
    
    if((type & SHERPA_CHECKBOX) == SHERPA_CHECKBOX){
       item->setCheckable(true);
       QStandardItemModel * model = item->model();
       
//       QStandardItemModel::itemChanged 
//	connect(item, SIGNAL(QStandardItemModel::itemChanged ),           this, SLOT(updateTree()));
    }
    
    if((type & SHERPA_COLOR) == SHERPA_COLOR){
      std::cerr <<"changeing color\n";
      QBrush b;
      b.setColor(Qt::blue);
      b.setColor(QColor(255,0,0));
     // item->setBackground(b);
     // item->setForeground(b);
      //item->setFlags(Qt::BackgroundRole);
      //setData(model->index(row, column), Qt::blue, Qt::BackgroundRole);
       
    }
    list << item;
    return list;

}

QList< QStandardItem* > SherpaRobotDock::prepareMainRow(const QString& name)
{
    QList<QStandardItem *> list;
    list << new QStandardItem(name);
    list << new QStandardItem("");
    return list;
}

void SherpaRobotDock::addRobot(const QString& name)
{

  QList<QStandardItem *> robotItem = prepareMainRow(name);
  
  QList<QStandardItem *> robotItemsP1 = prepareSubRow("Show Path", SHERPA_CHECKBOX);
  QList<QStandardItem *> robotItemsP2 = prepareSubRow("Path Color", SHERPA_COLOR);
  QList<QStandardItem *> robotItems2 = prepareSubRow("Show currentPosition", SHERPA_CHECKBOX);
  QList<QStandardItem *> robotItems3 = prepareSubRow("Show targetPosition", SHERPA_CHECKBOX);
  
  
  rootNode->    appendRow(robotItem);
    robotItem.first()->appendRow(robotItemsP1);
    robotItem.first()->appendRow(robotItemsP2);
    robotItem.first()->appendRow(robotItems2);
    robotItem.first()->appendRow(robotItems3);
}

void SherpaRobotDock::updateTree()
{
  tree->resizeColumnToContents(0);;

}


void SherpaRobotDock::robotClicked(const QModelIndex& foo )
{
  QStandardItem *item = standardModel->itemFromIndex(foo);
  QStandardItem *par = item->parent();
  if(par != 0){
    if(item->column()==1){
      std::cerr << item->accessibleText().toStdString() <<"checked: " << item->checkState()<< "\n";
      std::cerr << item->accessibleText().toStdString() <<"checked: " << item->checkState()<< "\n";
    }
    
  }else{
    
  }
  
}
}
//#include ""
