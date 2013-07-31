#ifndef __SHERPA_ROBOT_H__
#define __SHERPA_ROBOT_H__

#include <QString>
#include <QColor>


namespace sherpa_control_panel {
struct sherpa_waypoint{
  int x;
  int y;
};

struct sherpa_path{
  int length;
  
  sherpa_waypoint waypoints[];
  
};

struct sherpa_robot{
  
  QString *name;
  sherpa_waypoint current_position;
  sherpa_waypoint target_position;
  bool show_current_position;
  QColor *current_position_color;
  bool show_target_position;
  QColor *target_position_color;
  bool show_path;
  QColor *path_color;
  sherpa_path path;  
  
};
}
#endif