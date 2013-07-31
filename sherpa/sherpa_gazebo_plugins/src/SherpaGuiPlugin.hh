#ifndef __SHERPA_GUI_PLUGIN__
#define __SHERPA_GUI_PLUGIN__



#include "gui/Gui.hh"
#include "rendering/rendering.hh"
#include <gazebo.hh>
#include <deque>
#include <string.h>
#include "SherpaGazebo.hpp"
#include "ros/ros.h"
#include "ros/package.h"
#define WALK_ANIM_SPEED 0.002f
#define IDLE_ANIM_SPEED 0.001f
#define POINT_ANIM_SPEED 0.004f


namespace gazebo
{
  class SherpaGUI : public SystemPlugin
  {
    public: 
      void Load(int /*_argc*/, char ** /*_argv*/);
      void OnUpdate();
      void OnPause();
      void OnChangePose(SherpaAnimationPtr &msg);
      void OnChangePosition(SherpaPositionPtr &msg);
    event::ConnectionPtr changePose;
    
    
    transport::NodePtr node;
    transport::SubscriberPtr animationSubscriber;
    transport::SubscriberPtr positionSubscriber;
    
    
    private: 
      Ogre::Camera *cam;
      Ogre::AnimationState *idleAnim;
      Ogre::AnimationState *walkAnim;
      Ogre::AnimationState *pointAnim;
      Ogre::SceneNode * mNode;
      Ogre::AnimationState *mAnimationState;
      
      Ogre::Root * root;
      Ogre::SceneManager *mSceneMgr;
      
      double angle;
      
      bool walking;
      
      void Init();
      std::string resource;
      bool paused;
       rendering::RenderEngine *engine;
      double lastTime;
      double curTimeDif;
    float animation_speed;
      std::deque<Ogre::Vector3> mWalkList;
      rendering::UserCameraPtr userCam;
      std::vector<event::ConnectionPtr> connections;
      event::ConnectionPtr updateConnection;
      event::ConnectionPtr pauseConnection;
  // Register this plugin with the simulator
  };
  
  GZ_REGISTER_SYSTEM_PLUGIN(SherpaGUI)
}

#endif