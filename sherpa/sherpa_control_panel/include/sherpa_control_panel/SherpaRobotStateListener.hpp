#ifndef __SHERPA_ROBOT_STATE_LISTENER__
#define __SHERPA_ROBOT_STATE_LISTENER__

#include <QString>
#include "SherpaRobot.hpp"

namespace sherpa_control_panel {

class SherpaRobotStateListener {
public:
    void robotPositionChanged(QString *name, double x, double y);
    void robotRemoved(QString name);
    void robotAdded(sherpa_robot robot);


};

}

#endif
