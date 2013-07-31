#ifndef __SHERPA_ACTOR_FACTORY__
#define __SHERPA_ACTOR_FACTORY__



//#include "common/CommonTypes.hh"
#include "common/Animation.hh"
#include "common/KeyFrame.hh"
#include "common/Skeleton.hh"
#include "common/Mesh.hh"
#include "common/SkeletonAnimation.hh"
#include "physics/Model.hh"
#include "physics/Actor.hh"
#include "gazebo.hh"
#include <geometry_msgs/Twist.h>
#include "physics/World.hh"
#include "physics/Joint.hh"
#include "physics/Link.hh"
#include "physics/Model.hh"
#include "physics/Actor.hh"
#include "physics/PhysicsEngine.hh"
#include "physics/Actor.hh"
#include "physics/Physics.hh"
#include <stdio.h>
#include <boost/bind.hpp>
#include <physics/Physics.hh>
#include <common/common.hh>
#include <stdio.h>
#include "ros/ros.h"
#include <tf/transform_broadcaster.h>

///#include "sherpa/model_state.h"

#include "std_msgs/Float64.h"
#include <map>
namespace gazebo
{
class ActorFactoryPlugin : public WorldPlugin
{
private:
    const common::Mesh *mesh;
    common::Skeleton *skeleton;
    bool active;
    std::string current_animation;

    common::Time playStartTime;
    double lastAnimTime;
    common::Time prevFrameTime;
    physics::LinkPtr mainLink;
    unsigned int lastTraj;
    math::Vector3 lastPos;
    std::string visualName;
    event::ConnectionPtr updateConnection;

    physics::WorldPtr world;
    transport::PublisherPtr bonePosePub;

    std::string modelName;

    std::map<unsigned int, common::PoseAnimation*> trajectories;

    std::vector<physics::TrajectoryInfo> trajInfo;


    std::map<std::string, common::SkeletonAnimation*> skelAnimation;
    std::map<std::string, bool> interpolateX;
    std::map<std::string, std::map<std::string, std::string> > skelNodesMap;
    //bool isActive();

    //void LoadAnimation(std::string anim_filename,std::string anim_name, bool use_traj);
    void LoadSkin(std::string skin_filename,sdf::SDF _sdf);
    void LoadAnimation(std::string anim_filename, std::string anim_name, bool use_traj) ;

    void LoadBaseElements(std::string skin_filename, sdf::SDF _sdf);

    void AddSphereVisual(sdf::ElementPtr _linkSdf, const std::string &_name,
                         const math::Pose &_pose, double _radius,
                         const std::string &_material, const common::Color &_ambient);

    void AddBoxVisual(sdf::ElementPtr _linkSdf, const std::string &_name,
                      const math::Pose &_pose, const math::Vector3 &_size,
                      const std::string &_material, const common::Color &_ambient);

    void AddActorVisual(sdf::ElementPtr _linkSdf, const std::string &_name,
                        const math::Pose &_pose);

    void AddSphereCollision(sdf::ElementPtr _linkSdf,
                            const std::string &_name,
                            const math::Pose &_pose,
                            double _radius);

    void AddSphereInertia(sdf::ElementPtr _linkSdf,
                          const math::Pose &_pose,
                          double _mass, double _radius);

    void updateAnim();



    void startAnimation(std::string anim_name);

    void stopAnim();

    void SetPose(std::map<std::string, math::Matrix4> _frame,
                 std::map<std::string, std::string> _skelMap, double _time);
    void AddPlugin(sdf::ElementPtr _sdf,
                   std::string name, std::string filemame);

    
    void AddJoint(sdf::ElementPtr _sdf , std::string _name, 
		  std::string _parent, std::string _child,
		  const math::Pose &_pose, math::Vector3 _rot_axis, 
		  double _limit_lower = -10000000000000000, 
		  double _limit_upper = 10000000000000000  );

public:
    void Load(physics::WorldPtr _parent, sdf::ElementPtr /*_sdf*/);
    ActorFactoryPlugin();
    ~ActorFactoryPlugin();



    bool isActive();

    void OnUpdate();


};
GZ_REGISTER_WORLD_PLUGIN(ActorFactoryPlugin)
}

#endif
