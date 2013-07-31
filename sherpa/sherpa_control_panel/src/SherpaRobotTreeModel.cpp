#include "../include/sherpa_control_panel/SherpaRobotTreeModel.hpp"

namespace sherpa_control_panel {
SherpaRobotTreeModel::SherpaRobotTreeModel( QObject* parent): QAbstractItemModel(parent)
{
     
     rootItem = new SherpaRobotTreeItem();
//     setupModelData(data.split(QString("\n")), rootItem);
}


SherpaRobotTreeModel::~SherpaRobotTreeModel()
{
  delete rootItem;
}


SherpaRobotTreeItem* SherpaRobotTreeModel::getItem(const QModelIndex& index) const
{
  if (index.isValid()) {
         SherpaRobotTreeItem *item = static_cast<SherpaRobotTreeItem*>(index.internalPointer());
         if (item) return item;
     }
     return rootItem;
}

int SherpaRobotTreeModel::rowCount(const QModelIndex &parent) const
 {
     SherpaRobotTreeItem *parentItem = getItem(parent);

     return parentItem->childCount();
 }
 
 int SherpaRobotTreeModel::columnCount(const QModelIndex & /* parent */) const
 {
     return rootItem->columnCount();
 }
 
  Qt::ItemFlags SherpaRobotTreeModel::flags(const QModelIndex &index) const
 {
     if (!index.isValid())
         return 0;

     return Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable;
 }
 
  QModelIndex SherpaRobotTreeModel::index(int row, int column, const QModelIndex &parent) const
 {
     if (parent.isValid() && parent.column() != 0)
         return QModelIndex();
     
    SherpaRobotTreeItem *parentItem = getItem(parent);

     SherpaRobotTreeItem *childItem = parentItem->child(row);
     if (childItem)
         return createIndex(row, column, childItem);
     else
         return QModelIndex();
 }
 
 QModelIndex SherpaRobotTreeModel::parent(const QModelIndex &index) const
 {
     if (!index.isValid())
         return QModelIndex();

     SherpaRobotTreeItem *childItem = getItem(index);
     SherpaRobotTreeItem *parentItem = childItem->parent();

     if (parentItem == rootItem)
         return QModelIndex();

     return createIndex(parentItem->childNumber(), 0, parentItem);
 }
 
void SherpaRobotTreeModel::addRobot(sherpa_robot robot)
{

}

void SherpaRobotTreeModel::removeRobot(sherpa_robot robot)
{

}

sherpa_robot SherpaRobotTreeModel::removeRobot(QString* name)
{

}


QVariant SherpaRobotTreeModel::data(const QModelIndex& index, int role) const
{

}

QVariant SherpaRobotTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    return QAbstractItemModel::headerData(section, orientation, role);
}

bool SherpaRobotTreeModel::insertColumns(int position, int columns, const QModelIndex& parent)
{
    return QAbstractItemModel::insertColumns(position, columns, parent);
}

bool SherpaRobotTreeModel::insertRows(int position, int rows, const QModelIndex& parent)
{
    return QAbstractItemModel::insertRows(position, rows, parent);
}

bool SherpaRobotTreeModel::removeColumns(int position, int columns, const QModelIndex& parent)
{
    return QAbstractItemModel::removeColumns(position, columns, parent);
}

bool SherpaRobotTreeModel::removeRows(int position, int rows, const QModelIndex& parent)
{
    return QAbstractItemModel::removeRows(position, rows, parent);
}

bool SherpaRobotTreeModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    return QAbstractItemModel::setData(index, value, role);
}

bool SherpaRobotTreeModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant& value, int role)
{
    return QAbstractItemModel::setHeaderData(section, orientation, value, role);
}

void SherpaRobotTreeModel::setupModelData(const QStringList& lines, SherpaRobotTreeItem* parent)
{

}
}

//#include "SherpaRobotTreeModel.moc"