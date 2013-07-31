#ifndef __SHERPA_GAZEBO_HPP__
#define __SHERPA_GAZEBO_HPP__

#include "sherpa_animation_pose.pb.h"
#include "sherpa_position.pb.h"
#include "msgs/msgs.hh"

#define SHERPA_GAZEBO_NODE_NAME "sherpa"
#define SHERPA_GAZEBO_ANIMATION_TOPIC "~/animation"
#define SHERPA_GAZEBO_POSITION_TOPIC "~/position"

namespace gazebo
{
  
  typedef const boost::shared_ptr<const sherpa_gazebo::msgs::sherpa_animation_pose> SherpaAnimationPtr;
  typedef const boost::shared_ptr<const sherpa_gazebo::msgs::sherpa_position> SherpaPositionPtr;
}

#endif