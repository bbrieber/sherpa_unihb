#ifndef __SHERPA_ROBOT_MANAGER_H__
#define __SHERPA_ROBOT_MANAGER_H__

#include "Singleton.hpp"
#include "SherpaRobot.hpp"
#include "SherpaRobotStateListener.hpp"

#include "sherpa_msgs/SherpaRobot.h"
#include "sherpa_msgs/SherpaAtPosition.h"
#include "sherpa_msgs/SherpaMoveTo.h"
#include <QList>
#include <QHash>



namespace sherpa_control_panel {
class RobotManager : public Singleton<RobotManager> , public SherpaRobotStateListener {
public:
    void addRobot(sherpa_msgs::SherpaRobot);
    void deleteRobot(sherpa_robot);
    QList<sherpa_robot> getRobots();
    void updateRobotPosition(sherpa_msgs::SherpaAtPosition);
    sherpa_msgs::SherpaPosition getRobotPosition(QString *name);
    sherpa_msgs::SherpaPosition getRobotPosition(sherpa_robot robot){return getRobotPosition(robot.name)}
    
    void registerStateListener(SherpaRobotStateListener *listener){listeners << listener;}
    void deregisterStateListener(SherpaRobotStateListener *listener){listeners.removeAll(listener);}
    
private:
    QList<sherpa_robot> robots;
    QList<SherpaRobotStateListener*> listeners;
    QHash<QString,sherpa_msgs::SherpaPosition> positions;
};
}
#endif
