#include "SherpaHuman.hpp"

#include <physics/PhysicsEngine.hh>
#include <math.h>
#define __PI_ 3.1415

namespace gazebo{

SherpaHumanPlugin::SherpaHumanPlugin() 
{

    this->name = "sherpa_human_model";
    int argc = 0;
    ros::init(argc, NULL, name);
    //ROS_INFO("Initialized Sherpa Human");
    current_pose="not_initialized";
    
    
  twist.angular.x = 0.;
  twist.angular.y = 0.;
  twist.angular.z = 0;
  twist.linear.x = 0;
  twist.linear.y = 0;
  twist.linear.z = -9.867;
  rotation=0.f;
}
  
SherpaHumanPlugin::~SherpaHumanPlugin()
{
  
    //ROS_INFO("Shuttong DOWN Sherpa Human");
    delete this->ros_node;
}




void SherpaHumanPlugin::show_info()
{

  
}  

void SherpaHumanPlugin::Load(physics::ModelPtr _parent, sdf::ElementPtr /*_sdf*/)
{
  
    this->model = _parent;
    this->world = model->GetWorld();
    
    //this->heightmap = boost::static_pointer_cast<physics::HeightmapShape>(world->GetByName("map"));
    this->heightmap_model = boost::static_pointer_cast<physics::Model>(world->GetByName("map"));
  
    
    this->updateConnection = event::Events::ConnectWorldUpdateBegin(
                                 boost::bind(&SherpaHumanPlugin::OnUpdate, this));
    
    heightmap_model->GetName();
    
    this->gazNode = transport::NodePtr(new transport::Node());
        // Initialize the node with the world name
    //this->gazNode->Init(world->GetName());
    this->gazNode->Init(SHERPA_GAZEBO_NODE_NAME);
    gazAnimationPub = this->gazNode->Advertise<sherpa_gazebo::msgs::sherpa_animation_pose>(SHERPA_GAZEBO_ANIMATION_TOPIC); 
    gazPositionPub = this->gazNode->Advertise<sherpa_gazebo::msgs::sherpa_position>(SHERPA_GAZEBO_POSITION_TOPIC);   
    
    //std::cerr<<"Map Name: "<<heightmap_model->GetName()<<"\n";
    
//     std::cerr << heightmap->GetHeight(0,0);
    
//    math::Pose pose = math::Pose(0,0,h,0,0,0);
//    model->SetWorldPose(pose);
    
    init_ros();
    
    
}

void SherpaHumanPlugin::init_ros(){
  ros_node = new ros::NodeHandle("~");
  teleopSub = ros_node->subscribe<geometry_msgs::Twist>("/sherpa_move_command", 1, &SherpaHumanPlugin::OnRosUpdateMovement,this);
  poseSub = ros_node->subscribe<std_msgs::String>("/sherpa_pose_command", 1, &SherpaHumanPlugin::OnRosUpdatePose,this);
}

void SherpaHumanPlugin::OnRosUpdateMovement(const geometry_msgs::Twist::ConstPtr& msg)
{
//  ROS_INFO("received movement");

  twist.angular.x = msg->angular.x;
  twist.angular.y = msg->angular.y;
  twist.angular.z = msg->angular.z;
  twist.linear.x = msg->linear.x;
  twist.linear.y = msg->linear.y;
  twist.linear.z = -9.867;
  /*
  std::cerr << "(" <<
    twist.linear.x << "," <<
    twist.linear.y << "," <<
    twist.linear.z << ")(" <<
    twist.angular.x << "," <<
    twist.angular.y << "," <<
    twist.angular.z << ")\n";  
    */
  //this->twist = ;
}

void SherpaHumanPlugin::OnRosUpdatePose(const std_msgs::String::ConstPtr& msg)
{
  //ROS_INFO("received new pose");
  this->current_pose = msg->data;
  sherpa_gazebo::msgs::sherpa_animation_pose pose;
  pose.set_name(this->current_pose);
  gazAnimationPub->Publish(pose);
}

void SherpaHumanPlugin::OnPause()
{
  
}



void SherpaHumanPlugin::OnUpdate()
{
      ros::spinOnce();
      double stepTime = this->model->GetWorld()->GetPhysicsEngine()->GetMaxStepSize();
      this->rotation += twist.angular.z*stepTime;
      
      
      
      float sn = sin(rotation);
      float cs = cos(rotation);
      
      
    //ROS_INFO("[STEP_TIME:] %f",stepTime);
      //std::cerr<<"[STEP_TIME:] "<<stepTime;
/*
      math::Vector3 lin = this->model->GetWorld()->GetPhysicsEngine()->GetGravity();
      lin.x = twist.linear.x*stepTime;
      lin.x = twist.linear.y*stepTime;
      model->SetLinearVel(lin);
      model->SetLinearVel(math::Vector3(twist.linear.x*stepTime,twist.linear.y*stepTime,this->model->GetWorld()->GetPhysicsEngine()->GetGravity()));
      model->SetAngularVel(math::Vector3(twist.angular.x*stepTime,twist.angular.y*stepTime,twist.angular.z*stepTime));
      */
//      model->SetLinearVel(math::Vector3(twist.linear.x*10,twist.linear.y*10,twist.linear.z));
      model->SetAngularVel(math::Vector3(twist.angular.x,twist.angular.y,twist.angular.z));
      math::Pose p;
      p = model->GetWorldPose();
      sherpa_gazebo::msgs::sherpa_position msg;
      msg.set_x(p.pos.x);
      msg.set_y(p.pos.y);
      msg.set_z(p.pos.z-1.f);
      msg.set_angle(rotation);
      gazPositionPub->Publish(msg);
      //model->GetWorldPose().rot.z;
      //std::cerr << p.rot.x <<","<<p.rot.y <<","<<p.rot.z <<","<< "\n";
      
      model->SetLinearVel(math::Vector3((twist.linear.x*cs - twist.linear.y *sn)  * 10,(twist.linear.x*sn + twist.linear.y *cs)  * 10,twist.linear.z));
      
      //math::Pose p;
      
      
      //model->SetRelativePose(p);
      //model->SetWorldTwist(math::Vector3(twist.linear.x*10,twist.linear.y*10,twist.linear.z),math::Vector3(twist.angular.x,twist.angular.y,twist.angular.z));
      //if(current)
      
}
}