#ifndef __SHERPA_ROBOT_TREE_ITEM_H__
#define __SHERPA_ROBOT_TREE_ITEM_H__

#include "SherpaRobot.hpp"
#include <QVariant>

namespace sherpa_control_panel {
 class SherpaRobotTreeItem
 {
 public:
     SherpaRobotTreeItem(const sherpa_robot &data, SherpaRobotTreeItem *parent = 0);
     SherpaRobotTreeItem(SherpaRobotTreeItem *parent = 0);
     ~SherpaRobotTreeItem();

     SherpaRobotTreeItem *child(int number);
     int childCount() ;
     int columnCount() const;
     QVariant data(int column) const;
     bool insertChildren(int position, int count, int columns);
     bool insertColumns(int position, int columns);
     SherpaRobotTreeItem *parent();
     bool removeChildren(int position, int count);
     bool removeColumns(int position, int columns);
     int childNumber();
     bool setData(int column, const QVariant &value);

 private:
     QList<SherpaRobotTreeItem*> childItems;
     sherpa_robot itemData;
     SherpaRobotTreeItem *parentItem;
 };
}
#endif