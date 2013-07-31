#ifndef __SHERPA_HUMAN_PLUGIN_HPP__
#define __SHERPA_HUMAN_PLUGIN_HPP__


#include "gazebo.hh"
#include "physics/World.hh"
#include "physics/Model.hh"
#include "physics/HeightmapShape.hh"
#include "physics/Physics.hh"
#include <stdio.h>
#include "ros/ros.h"
#include <geometry_msgs/Twist.h>
#include <std_msgs/String.h>
#include <map>
#include "SherpaGazebo.hpp"


namespace gazebo
{



class SherpaHumanPlugin : public ModelPlugin
{


private:
    std::string name;
    std::string current_pose;
    
    event::ConnectionPtr updateConnection;
    event::ConnectionPtr pauseConnection;

    physics::ModelPtr model;
    physics::WorldPtr world;
    physics::ModelPtr heightmap_model;
    physics::HeightmapShapePtr heightmap;
    
    
    float rotation;
    
    ros::NodeHandle *ros_node;
    ros::Publisher pose_pub;
    ros::Publisher position_pub;
    ros::Subscriber teleopSub;
    ros::Subscriber poseSub;
       
    
    geometry_msgs::Twist twist;
    geometry_msgs::Twist movement_twist;
    
    transport::PublisherPtr gazAnimationPub;
    transport::PublisherPtr gazPositionPub;
    transport::NodePtr gazNode;
    void show_info();
    void init_ros();
    
public:
    SherpaHumanPlugin();
    ~SherpaHumanPlugin();

    void Load(physics::ModelPtr _parent, sdf::ElementPtr /*_sdf*/);
    void OnUpdate();
    void OnRosUpdateMovement(const geometry_msgs::Twist::ConstPtr& msg);
    void OnRosUpdatePose(const std_msgs::String::ConstPtr& msg);
    void OnPause();
 
};

  // Register this plugin with the simulator
  GZ_REGISTER_MODEL_PLUGIN(SherpaHumanPlugin);

}
#endif