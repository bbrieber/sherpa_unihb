#include "../include/sherpa_control_panel/SherpaRobotTreeItem.hpp"
#include <iostream>

namespace sherpa_control_panel {
SherpaRobotTreeItem::SherpaRobotTreeItem(const sherpa_robot& data, SherpaRobotTreeItem* parent)
{
  itemData = data;
  parentItem = parent;
}

SherpaRobotTreeItem::SherpaRobotTreeItem(SherpaRobotTreeItem* parent)
{
  parentItem = parent;

}



SherpaRobotTreeItem::~SherpaRobotTreeItem()
{
  qDeleteAll(childItems);
}

SherpaRobotTreeItem* SherpaRobotTreeItem::parent()
{
  return parentItem;
}

SherpaRobotTreeItem* SherpaRobotTreeItem::child(int number)
{

     return childItems.value(number);
}

int SherpaRobotTreeItem::childCount()
{
  return childItems.count();
}

int SherpaRobotTreeItem::childNumber()
{
  if (parentItem)
         return parentItem->childItems.indexOf(const_cast<SherpaRobotTreeItem*>(this));

     return 0;
}

int SherpaRobotTreeItem::columnCount() const
{
  return 2;
}

QVariant SherpaRobotTreeItem::data(int column) const
{
  
  return QVariant("Foo");
}


bool SherpaRobotTreeItem::insertChildren(int position, int count, int columns)
{
  std::cerr << "can't add children to robot \n";
  return false;
}


bool SherpaRobotTreeItem::insertColumns(int position, int columns)
{
  std::cerr << "can't add Colum to robot \n";
return false;
}

bool SherpaRobotTreeItem::removeChildren(int position, int count)
{

  std::cerr << "can't reomve Child to robot \n";
return false;
}

bool SherpaRobotTreeItem::removeColumns(int position, int columns)
{

  std::cerr << "can't remove Column to robot \n";
return false;
}


bool SherpaRobotTreeItem::setData(int column, const QVariant& value)
{
  std::cerr << "update Data not Set \n";
return false;

}
}




