#ifndef __SHERPA_CONNECTOR_DOCK__
#define __SHERPA_CONNECTOR_DOCK__

#include <QDockWidget>
#include <QMainWindow>
#include <QTreeView>
#include <QTreeWidget>
#include <QStandardItem>
#include <QObject>

namespace sherpa_control_panel {
typedef enum {
    SHERPA_CHECKBOX = (1u << 0),
    SHERPA_COLOR = (1u << 1),
    SHERPA_EDITABLE  = (1u << 2),
    SHERPA_COMBO  = (1u << 3),
    SHERPA_POSITION  = (1u << 4)
}Sherpa_Widget_type;

class SherpaRobotDock : public QDockWidget{
Q_OBJECT

public:
    SherpaRobotDock(QWidget* parent = 0, Qt::WindowFlags flags = 0);
    ~SherpaRobotDock();
    
    void updateRobotProperties();
    
private:
  //QTreeView *tree;
  QTreeView *tree;
  void createTree();
  QWidget* parent;
  QStandardItemModel *standardModel;
  QStandardItem *rootNode;
  
  void addRobot(const QString &name);
  QList<QStandardItem *> prepareSubRow(const QString &name,
                                                 const Sherpa_Widget_type type);
  QList<QStandardItem *> prepareMainRow(const QString &name);
  
private Q_SLOTS:
  void updateTree();
  void robotClicked(const QModelIndex& foo );
};
}
#endif