#include "SherpaGuiPlugin.hh"
#include <stdio.h>
#include "rendering/RenderEngine.hh"
#include "gui/GuiEvents.hh"

#include <ros/package.h>
namespace gazebo{

void SherpaGUI::Init(){
  
  this->engine =  rendering::RenderEngine::Instance();
  this->cam = gui::get_active_camera()->GetOgreCamera();
  
  this->angle = 0.;
  
  if(!engine || engine == NULL){
    std::cerr << "could not access RenderEngine";
  }
  
  this->root = engine->root;
  
  
  if(!root || root == NULL){
    std::cerr << "could not access Ogre Root element";
  }
  
  root->addResourceLocation(resource ,"FileSystem","SHERPA");
  Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
  
  
  paused=false;
  
  this->node = transport::NodePtr(new transport::Node());
  this->node->Init(SHERPA_GAZEBO_NODE_NAME);
  animationSubscriber = node->Subscribe(SHERPA_GAZEBO_ANIMATION_TOPIC,&SherpaGUI::OnChangePose,this);
  positionSubscriber = node->Subscribe(SHERPA_GAZEBO_POSITION_TOPIC,&SherpaGUI::OnChangePosition,this);
  
  
  
  Ogre::SceneManagerEnumerator::SceneManagerIterator si =root->getSceneManagerIterator();
  
  Ogre::SceneManager * mSceneMgr = si.getNext();
  
   
  walking = false;
  
  Ogre::Entity * humanEntity = mSceneMgr->createEntity("human", "Sherpa_human.mesh");
  
  
  mNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("HumanNode", Ogre::Vector3(0.0f, 0.0f, 0.0f));//,Ogre::Quaternion(1.0f,0.0f,1.0f,1.0f));
  mNode->scale(25,25,25);
  mNode->pitch(Ogre::Degree(90));
  mNode->yaw(Ogre::Degree(-90));
  mNode->attachObject(humanEntity);
  mWalkList.push_back(Ogre::Vector3(550.0f,  0.0f,  50.0f ));  
  mWalkList.push_back(Ogre::Vector3(-100.0f,  0.0f, -200.0f));
  
  walkAnim = humanEntity->getAnimationState("walk");
  
        walkAnim->setLoop(true);
        walkAnim->setEnabled(false);
	
	
  idleAnim = humanEntity->getAnimationState("idle");
        idleAnim->setLoop(true);
        idleAnim->setEnabled(true);
	
  pointAnim = humanEntity->getAnimationState("point");
        pointAnim->setLoop(false);
	
        pointAnim->setEnabled(false);
	
  mAnimationState = idleAnim;
  
  this->updateConnection = event::Events::ConnectRender(boost::bind(&SherpaGUI::OnUpdate, this));
  animation_speed = IDLE_ANIM_SPEED;
  this->pauseConnection = event::Events::ConnectStop(boost::bind(&SherpaGUI::OnPause, this));
  
  lastTime = engine->root->getTimer()->getMilliseconds();
}

void SherpaGUI::Load(int argc, char ** argv){
  
  
  std::string path = ros::package::getPath("sherpa_gazebo");
  resource = path+"/media";
//  std::cerr<< path;
}

void SherpaGUI::OnUpdate(){
  curTimeDif = root->getTimer()->getMillisecondsCPU() - lastTime;
  lastTime = root->getTimer()->getMillisecondsCPU();
  if(!paused){
    mAnimationState->addTime(curTimeDif*this->animation_speed);
  }
}


void SherpaGUI::OnPause()
{
  
  paused=!paused;
  if(paused)
    std::cerr<<"pause activated";
  else
    std::cerr<<"pause deactivated";
}

void SherpaGUI::OnChangePose(SherpaAnimationPtr &msg)
{
    mAnimationState->setEnabled(false);
    if(strcmp (msg->name().c_str(),"walk")== 0){
      animation_speed = WALK_ANIM_SPEED;
      mAnimationState= walkAnim;
    }else if(strcmp (msg->name().c_str(),"point")== 0){
      mAnimationState= pointAnim;
      animation_speed = POINT_ANIM_SPEED;
    }else{
      mAnimationState = idleAnim;
      
      animation_speed = IDLE_ANIM_SPEED;
    }
    mAnimationState->setTimePosition(0.0);
    mAnimationState->setEnabled(true);
}

void SherpaGUI::OnChangePosition(SherpaPositionPtr& msg)
{
  mNode->setPosition(msg->x(),msg->y(),msg->z());
  mNode->yaw(Ogre::Radian(-(this->angle-msg->angle())));
  angle = msg->angle();
}

}