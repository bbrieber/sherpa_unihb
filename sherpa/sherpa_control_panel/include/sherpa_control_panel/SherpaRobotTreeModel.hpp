#ifndef __SHERPA_ROBOT_TREE_MODEL_H__
#define __SHERPA_ROBOT_TREE_MODEL_H__
#include <QAbstractItemModel>

#include "SherpaRobot.hpp"
#include "SherpaRobotTreeItem.hpp"

namespace sherpa_control_panel {
class SherpaRobotTreeModel : public QAbstractItemModel
 {
     Q_OBJECT

 public:
     SherpaRobotTreeModel(
               QObject *parent = 0);
    ~SherpaRobotTreeModel();
   
    
     QVariant data(const QModelIndex &index, int role) const;
     QVariant headerData(int section, Qt::Orientation orientation,
                         int role = Qt::DisplayRole) const;

     QModelIndex index(int row, int column,
                       const QModelIndex &parent = QModelIndex()) const;
     QModelIndex parent(const QModelIndex &index) const;

     int rowCount(const QModelIndex &parent = QModelIndex()) const;
     int columnCount(const QModelIndex &parent = QModelIndex()) const;
     
      Qt::ItemFlags flags(const QModelIndex &index) const;
      
     void addRobot(sherpa_robot robot);
     void removeRobot(sherpa_robot robot);
     sherpa_robot removeRobot(QString *name);
     
     bool setData(const QModelIndex &index, const QVariant &value,
                  int role = Qt::EditRole);
     bool setHeaderData(int section, Qt::Orientation orientation,
                        const QVariant &value, int role = Qt::EditRole);

     bool insertColumns(int position, int columns,
                        const QModelIndex &parent = QModelIndex());
     bool removeColumns(int position, int columns,
                        const QModelIndex &parent = QModelIndex());
     bool insertRows(int position, int rows,
                     const QModelIndex &parent = QModelIndex());
     bool removeRows(int position, int rows,
                     const QModelIndex &parent = QModelIndex());

 private:
     void setupModelData(const QStringList &lines, SherpaRobotTreeItem *parent);
     SherpaRobotTreeItem *getItem(const QModelIndex &index) const;

     SherpaRobotTreeItem *rootItem;
 };

}
#endif