#include "ActorFactoryPlugin.hh"
#include "SherpaColladaLoader.hh"

#define SKIN_SCALE 1.0f
namespace gazebo {

void ActorFactoryPlugin::Load(physics::WorldPtr _parent, sdf::ElementPtr /*_sdf*/) {
    world = _parent;
    sdf::SDF actorSDF;
    actorSDF.SetFromString(
     "<?xml version='1.0'?>\
      <sdf version='1.4'>\
	<model name='sherpa_human'>\
	  <pose>0 0 0 0 0 0 </pose>\
	  <static>false</static>\
	  <link name='human_body'>\
	    <visual name='human_mesh'>\
	      <geometry>\
		<mesh>\
		  <uri>model://sherpa_human/meshes/walk.dae</uri>\
		</mesh>\
	      </geometry>\
	    </visual>\
	  </link>\
	</model>\
      </sdf>"
    );
    /*

    physics::ActorSdf actor;
    actor->
    
    
    
    */

    this->modelName = "sherpa_human";
    //std::cerr << "Loading Skin" <<"\n";
    std::cerr << "Loading Base Links" <<"\n";
    //LoadSkin("/home/bbrieber/.gazebo/models/sherpa_human/meshes/walk.dae", actorSDF);
    SherpaColladaLoader loader;
    loader.Load("/home/bbrieber/Sherpa/graphix/raw/rover_skel_pos.dae", actorSDF,"/home/bbrieber/tmp.sdf");
    //LoadBaseElements("/home/bbrieber/Sherpa/graphix/raw/rover_skel_pos.dae", actorSDF);
    //std::cerr << "loading Animations\n";
    
    //LoadAnimation("/home/bbrieber/.gazebo/models/sherpa_human/meshes/walk.dae","walk",true);
    //LoadAnimation("/home/bbrieber/.gazebo/models/sherpa_human/meshes/walk.dae","walk",true);
    //LoadAnimation("/home/bbrieber/.gazebo/models/sherpa_human/meshes/talk.dae","talk",true);
    //std::cerr << actorSDF.ToString() <<"\n";
    //actorSDF.PrintDescription();

    //AddPlugin(actorSDF.root->GetElement("model"),"SherpaHumanModel","libsherpa_actor.so");

    std::cerr << "Writing to file" <<"\n";
    actorSDF.Write("/home/bbrieber/tmp.sdf");
    //std::cerr << actorSDF.ToString() <<"\n";
    //_parent->InsertModelSDF(actorSDF);
    
    //startAnimation("walk");
    //this->updateConnection = event::Events::ConnectWorldUpdateBegin(boost::bind(&ActorFactoryPlugin::OnUpdate, this));
    /*
    gazebo::transport::NodePtr node(new gazebo::transport::Node());
    node->Init();
    this->bonePosePub = node->Advertise<msgs::PoseAnimation>(
                            "~/skeleton_pose/info", 10);
                            */
    
}




void ActorFactoryPlugin::LoadAnimation(std::string anim_filename, std::string anim_name, bool use_traj) {
    common::Skeleton *skel = NULL;
    common::MeshManager::Instance()->Load(anim_filename);
    const common::Mesh *animMesh = NULL;
    if (common::MeshManager::Instance()->HasMesh(anim_filename))
        animMesh = common::MeshManager::Instance()->GetMesh(anim_filename);
    if (animMesh && animMesh->HasSkeleton())
    {
        skel = animMesh->GetSkeleton();
        skel->Scale(SKIN_SCALE);//FIXME
        std::cerr << "num of joints" <<skel->GetNumJoints()<< "\n";
    }

    if (!skel || skel->GetNumAnimations() == 0)
        gzerr << "Failed to load animation.";
    else
    {
        bool compatible = true;
        std::map<std::string, std::string> skelMap;

        //if (this->skeleton->GetNumNodes() != skel->GetNumNodes())
        //  compatible = false;
        //else
        for (unsigned int i = 0; i < this->skeleton->GetNumNodes(); i++)
        {
            common::SkeletonNode *skinNode = this->skeleton->GetNodeByHandle(i);
            common::SkeletonNode *animNode = skel->GetNodeByHandle(i);
            if (animNode->GetChildCount() != skinNode->GetChildCount())
            {
                compatible = false;
                break;
            }
            else {
                skelMap[skinNode->GetName()] = animNode->GetName();
                std::cerr <<animNode->GetName()<<"\n";
            }
        }

        if (!compatible)
        {
            gzerr << "Skin and animation " << anim_name <<
                  " skeletons are not compatible.\n";
        }
        else
        {
            this->skelAnimation[anim_name] =
                skel->GetAnimation(0);
            //FIXME we will need this
            this->interpolateX[anim_name] = use_traj;
            this->skelNodesMap[anim_name] = skelMap;
            ROS_INFO("Loaded Animation %s ", anim_name.c_str());
        }

    }
}

ActorFactoryPlugin::ActorFactoryPlugin()
{
    puts("starting actor Factory");
    this->mesh = NULL;
    this->skeleton = NULL;
    std::string name = "sherpa_human_model";
    int argc = 0;
    //ros::init(argc, NULL, name);
    //ROS_INFO("Initialized Sherpa Human");
}

ActorFactoryPlugin::~ActorFactoryPlugin()
{

}

void ActorFactoryPlugin::LoadBaseElements(std::string skin_filename, sdf::SDF _sdf)
{
  common::MeshManager::Instance()->Load(skin_filename);

	std::cerr<< "Mesh Loaded";
    if (common::MeshManager::Instance()->HasMesh(skin_filename))
    {
        this->mesh = common::MeshManager::Instance()->GetMesh(skin_filename);
	std::cerr<< "check for Skeleton";
        if (!this->mesh->HasSkeleton())
            gzthrow("Collada file does not contain skeletal animation.");
        std::cerr<< "laoding Skeleton";
	this->skeleton = mesh->GetSkeleton();
        this->skeleton->Scale(SKIN_SCALE);

        common::NodeMap nodes = this->skeleton->GetNodes();

        sdf::ElementPtr linkSdf;

        linkSdf = _sdf.root->GetFirstElement()->GetElement("link");
        linkSdf->GetAttribute("name")->Set(modelName + "_pose");
        linkSdf->GetElement("gravity")->Set(false);//FIXME ????
        sdf::ElementPtr linkPose = linkSdf->GetElement("pose");
    }
}

void ActorFactoryPlugin::LoadSkin(std::string skin_filename, sdf::SDF _sdf)
{
    common::MeshManager::Instance()->Load(skin_filename);

    if (common::MeshManager::Instance()->HasMesh(skin_filename))
    {
        this->mesh = common::MeshManager::Instance()->GetMesh(skin_filename);
        if (!this->mesh->HasSkeleton())
            gzthrow("Collada file does not contain skeletal animation.");
        this->skeleton = mesh->GetSkeleton();
        this->skeleton->Scale(SKIN_SCALE);

        common::NodeMap nodes = this->skeleton->GetNodes();

        sdf::ElementPtr linkSdf;

        linkSdf = _sdf.root->GetFirstElement()->GetElement("link");
        linkSdf->GetAttribute("name")->Set(modelName + "_pose");
        linkSdf->GetElement("gravity")->Set(false);//FIXME ????
        sdf::ElementPtr linkPose = linkSdf->GetElement("pose");
        //model->UpdateParameters(linkSdf);


        this->AddBoxVisual(linkSdf, modelName + "_pose_vis", math::Pose(),
                           math::Vector3(0.05, 0.05, 0.05), "Gazebo/White",
                           common::Color::White);
        this->AddActorVisual(linkSdf, modelName + "_visual", math::Pose());

        this->visualName = modelName + "::" + modelName + "_pose::"
                           + modelName + "_visual";



        std::cerr<<"Creating sub Links";
        for (common::NodeMapIter iter = nodes.begin(); iter != nodes.end(); ++iter)
        {
            common::SkeletonNode* bone = iter->second;

            linkSdf = _sdf.root->GetFirstElement()->AddElement("link");

            linkSdf->GetAttribute("name")->Set(bone->GetName());
            linkSdf->GetElement("gravity")->Set(false);
            linkPose = linkSdf->GetElement("pose");
            math::Pose pose(bone->GetModelTransform().GetTranslation(),
                            bone->GetModelTransform().GetRotation());
            if (bone->IsRootNode())
                pose = math::Pose();
            linkPose->Set(pose);

            /// FIXME hardcoded inertia of a sphere with mass 1.0 and radius 0.01
            this->AddSphereInertia(linkSdf, math::Pose(), 1.0, 0.01);

            /// FIXME hardcoded visual to red sphere with radius 0.02
            if (bone->IsRootNode())
            {
                this->AddSphereVisual(linkSdf, bone->GetName() 
		//+ "__SKELETON_VISUAL__"
		,math::Pose(), 0.02, "Gazebo/Blue", common::Color::Blue);
            }
            else if (bone->GetChildCount() == 0)
            {
                this->AddSphereVisual(linkSdf, bone->GetName()
                                     //+ "__SKELETON_VISUAL__"
				      , math::Pose(), 0.02,
                                      "Gazebo/Yellow", common::Color::Yellow);
            }
            else
                this->AddSphereVisual(linkSdf, bone->GetName()
				      //+ "__SKELETON_VISUAL__"
				      , math::Pose(), 0.02,
                                      "Gazebo/Red", common::Color::Red);

            /// FIXME hardcoded collision to sphere with radius 0.02
            this->AddSphereCollision(linkSdf, bone->GetName() + "_collision",
                                     math::Pose(), 0.02);

/**
            for (unsigned int i = 0; i < bone->GetChildCount(); i++)
            {
                common::SkeletonNode *curChild = bone->GetChild(i);

                math::Vector3 dir = curChild->GetModelTransform().GetTranslation() -
                                    bone->GetModelTransform().GetTranslation();
                double length = dir.GetLength();

                if (!math::equal(length, 0.0))
                {
                    math::Vector3 r = curChild->GetTransform().GetTranslation();
                    math::Vector3 linkPos = math::Vector3(r.x / 2.0,
                                                          r.y / 2.0, r.z / 2.0);
                    double theta = atan2(dir.y, dir.x);
                    double phi = acos(dir.z / length);

                    math::Pose bonePose(linkPos, math::Quaternion(0.0, phi, theta));
                    bonePose.rot = pose.rot.GetInverse() * bonePose.rot;

                    this->AddBoxVisual(linkSdf, bone->GetName() + "_" +
                                       curChild->GetName() + "__SKELETON_VISUAL__", bonePose,
                                       math::Vector3(0.02, 0.02, length), "Gazebo/Green", common::Color::Green);
                }
            }
           
*/
        }
	    sdf::ElementPtr jointSdf;
	    sdf::ElementPtr jointPoseSdf;
	    sdf::ElementPtr jointParentSdf;
	    sdf::ElementPtr jointChildSdf;
	     for (common::NodeMapIter iter = nodes.begin(); iter != nodes.end(); ++iter)
	    {
	      common::SkeletonNode* bone = iter->second;

	      for (unsigned int i = 0; i < bone->GetChildCount(); i++)
	      {
		  common::SkeletonNode *curChild = bone->GetChild(i);
		  jointSdf = _sdf.root->GetFirstElement()->AddElement("joint");
		  jointSdf->GetAttribute("name")->Set(bone->GetName()+"_"+curChild->GetName()+"_joint");
		  jointSdf->GetAttribute("type")->Set("revolute");
		  //jointSdf->GetAttribute("type")->Set("ball");//TODO Set to Ball
		  jointPoseSdf = jointSdf->GetElement("pose");
		  math::Pose po = math::Pose();
		  jointPoseSdf->Set(po);
		  jointParentSdf = jointSdf->GetElement("parent");
		  jointParentSdf->Set(bone->GetName());
		  jointChildSdf = jointSdf->GetElement("child");
		  jointChildSdf->Set(curChild->GetName());

		
	      }
	    }
	  //  */
    }
}





bool ActorFactoryPlugin::isActive() {
    return active;
}

void ActorFactoryPlugin::OnUpdate()
{
    /*
    ros::spinOnce();
    */
    updateAnim();

}


void ActorFactoryPlugin::startAnimation(std::string anim_name)
{
    this->active = true;
    this->playStartTime = this->world->GetSimTime();
    this->lastAnimTime = std::numeric_limits<double>::max();
    this->current_animation = anim_name;
}

void ActorFactoryPlugin::stopAnim() {
    this->active = false;
}


void ActorFactoryPlugin::updateAnim()
{
    if (!this->active) {
        std::cerr << "no animation is active";
        return;
    }


    common::Time currentTime = this->world->GetSimTime();

    /// do not refresh animation more faster the 30 Hz sim time
    if ((currentTime - this->prevFrameTime).Double() < (1.0 / 30.0))
        return;


    double animationTime = currentTime.Double()  -
                           this->playStartTime.Double();



    /// at this point we are certain that a new frame will be animated
    this->prevFrameTime = currentTime;

    physics::TrajectoryInfo tinfo;



    //REMOVED TRAJ STUFF HERE FIXME
    for (unsigned int i = 0; i < this->trajInfo.size(); i++)
        if (this->trajInfo[i].startTime <= lastAnimTime &&
                this->trajInfo[i].endTime >= lastAnimTime)
        {
            tinfo = this->trajInfo[i];
            break;
        }

    lastAnimTime = lastAnimTime - tinfo.startTime;


//  common::SkeletonAnimation *skelAnim = this->skelAnimation[tinfo.type];
    common::SkeletonAnimation *skelAnim = this->skelAnimation[this->current_animation];
    if(skelAnim==NULL) {
        std::cerr<< "no Animation loaded: "<<this->current_animation <<"\n";
        return;
    }

    /*
        if(animationTime > skelAnim->GetLength()) {
    	this->playStartTime -= skelAnim->GetLength();
            animationTime = animationTime - skelAnim->GetLength();
        }
    */
    std::map<std::string, std::string> skelMap = this->skelNodesMap[tinfo.type];

    math::Pose modelPose;
    std::map<std::string, math::Matrix4> frame;
    if (this->trajectories.find(tinfo.id) != this->trajectories.end())
    {

        std::cerr << "2-1";

        common::PoseKeyFrame posFrame(0.0);
	
        this->trajectories[tinfo.id]->SetTime(lastAnimTime);
        this->trajectories[tinfo.id]->GetInterpolatedKeyFrame(posFrame);
        modelPose.pos = posFrame.GetTranslation();
        modelPose.rot = posFrame.GetRotation();

        this->lastPos = modelPose.pos;
    }
    if (this->interpolateX[tinfo.type] &&
            this->trajectories.find(tinfo.id) != this->trajectories.end())
    {
        std::cerr << "2-2";
//    frame = skelAnim->GetPoseAtX(this->pathLength,
        frame = skelAnim->GetPoseAtX(500,
                                     skelMap[this->skeleton->GetRootNode()->GetName()]);
    }
    else {
        //std::cerr <<"animation Time "<< animationTime << "skelTime:" << skelAnim->GetLength()<<"\n";
        frame = skelAnim->GetPoseAt(animationTime);
	
	
    }
    
    
    
    //frame.

//    
//    frame.
    
    /*
    this->lastTraj = tinfo.id;


    math::Matrix4 rootTrans =
        frame[skelMap[this->skeleton->GetRootNode()->GetName()]];

    math::Vector3 rootPos = rootTrans.GetTranslation();
    math::Quaternion rootRot = rootTrans.GetRotation();

    if (tinfo.translated)
        rootPos.x = 0.0;
    math::Pose actorPose;
    actorPose.pos = modelPose.pos + modelPose.rot.RotateVector(rootPos);
    actorPose.rot = modelPose.rot *rootRot;

    math::Matrix4 rootM(actorPose.rot.GetAsMatrix4());
    rootM.SetTranslate(actorPose.pos);


    frame[skelMap[this->skeleton->GetRootNode()->GetName()]] = rootM;
    */
    //this->SetPose(frame, skelMap, currentTime.Double());

    //this->lastScriptTime = scriptTime;
}

void ActorFactoryPlugin::SetPose(std::map<std::string, math::Matrix4> _frame,
                                 std::map<std::string, std::string> _skelMap, double _time)
{
    msgs::PoseAnimation msg;
    msg.set_model_name(this->visualName);

    math::Matrix4 modelTrans(math::Matrix4::IDENTITY);
    math::Pose mainLinkPose;
    physics::ModelPtr model = world->GetModel(modelName);

    for (unsigned int i = 0; i < this->skeleton->GetNumNodes(); i++)
    {
        common::SkeletonNode *bone = this->skeleton->GetNodeByHandle(i);
        common::SkeletonNode *parentBone = bone->GetParent();
        math::Matrix4 transform(math::Matrix4::IDENTITY);
        if (_frame.find(_skelMap[bone->GetName()]) != _frame.end())
            transform = _frame[_skelMap[bone->GetName()]];
        else
            transform = bone->GetTransform();
	
        physics::LinkPtr currentLink = model->GetChildLink(bone->GetName());
        math::Pose bonePose = transform.GetAsPose();

        if (!bonePose.IsFinite())
        {
            //std::cerr << "ACTOR: " << _time << " " << bone->GetName() << " " << bonePose << "\n";
            bonePose.Correct();
        }

        msgs::Pose *bone_pose = msg.add_pose();
 //       bone_pose->set_name(bone->GetName()); FIXME
       bone_pose->set_name(bone->GetName());

        if (!parentBone)
        {
            bone_pose->mutable_position()->CopyFrom(msgs::Convert(math::Vector3()));
            bone_pose->mutable_orientation()->CopyFrom(msgs::Convert(
                        math::Quaternion()));
            mainLinkPose = bonePose;
        }
        else
        {
            bone_pose->mutable_position()->CopyFrom(msgs::Convert(bonePose.pos));
            bone_pose->mutable_orientation()->CopyFrom(msgs::Convert(bonePose.rot));
            physics::LinkPtr parentLink = model->GetChildLink(parentBone->GetName());
            //physics::LinkPtr parentLink = world->GetByName(parentBone->GetName());
            math::Pose parentPose = parentLink->GetWorldPose();
            math::Matrix4 parentTrans(parentPose.rot.GetAsMatrix4());
            parentTrans.SetTranslate(parentPose.pos);
            transform = parentTrans * transform;
        }
/////////////////////////
        msgs::Pose *link_pose = msg.add_pose();
        link_pose->set_name(currentLink->GetScopedName());
        math::Pose linkPose = transform.GetAsPose() - mainLinkPose;
        link_pose->mutable_position()->CopyFrom(msgs::Convert(linkPose.pos));
        link_pose->mutable_orientation()->CopyFrom(msgs::Convert(linkPose.rot));
	/////////////////////////////////////////
    }

    msgs::Time *stamp = msg.add_time();
    stamp->CopyFrom(msgs::Convert(_time));

    msgs::Pose *model_pose = msg.add_pose();
    model_pose->set_name(model->GetScopedName());
    model_pose->mutable_position()->CopyFrom(msgs::Convert(mainLinkPose.pos));
    model_pose->mutable_orientation()->CopyFrom(msgs::Convert(mainLinkPose.rot));

    std::cerr << msg.DebugString();
    if (this->bonePosePub && this->bonePosePub->HasConnections())
        this->bonePosePub->Publish(msg); //FIXME defintily cool to have this
    model->SetWorldPose(mainLinkPose, true, false);

}


//////////////////////////////////////////////////
void ActorFactoryPlugin::AddSphereVisual(sdf::ElementPtr _linkSdf, const std::string &_name,
        const math::Pose &_pose, double _radius,
        const std::string &_material, const common::Color &_ambient)
{
    sdf::ElementPtr visualSdf = _linkSdf->GetElement("visual");
    visualSdf->GetAttribute("name")->Set(_name);
    sdf::ElementPtr visualPoseSdf = visualSdf->GetElement("pose");
    visualPoseSdf->Set(_pose);
    sdf::ElementPtr geomVisSdf = visualSdf->GetElement("geometry");
    sdf::ElementPtr sphereVisSdf = geomVisSdf->GetElement("sphere");
    sphereVisSdf->GetElement("radius")->Set(_radius);
    sdf::ElementPtr matSdf = visualSdf->GetElement("material");
    matSdf->GetElement("script")->Set(_material);
    sdf::ElementPtr colorSdf = matSdf->GetElement("ambient");
    colorSdf->Set(_ambient);
}


void ActorFactoryPlugin::AddSphereInertia(sdf::ElementPtr _linkSdf,
        const math::Pose &_pose,
        double _mass, double _radius)
{
    double ixx = 2.0 * _mass * _radius * _radius / 5.0;
    sdf::ElementPtr inertialSdf = _linkSdf->GetElement("inertial");
    sdf::ElementPtr inertialPoseSdf = inertialSdf->GetElement("pose");
    inertialPoseSdf->Set(_pose);
    inertialSdf->GetElement("mass")->Set(_mass);
    sdf::ElementPtr tensorSdf = inertialSdf->GetElement("inertia");
    tensorSdf->GetElement("ixx")->Set(ixx);
    tensorSdf->GetElement("ixy")->Set(0.00);
    tensorSdf->GetElement("ixz")->Set(0.00);
    tensorSdf->GetElement("iyy")->Set(ixx);
    tensorSdf->GetElement("iyz")->Set(0.00);
    tensorSdf->GetElement("izz")->Set(ixx);
}

//////////////////////////////////////////////////
void ActorFactoryPlugin::AddSphereCollision(sdf::ElementPtr _linkSdf,
        const std::string &_name,
        const math::Pose &_pose,
        double _radius)
{
    sdf::ElementPtr collisionSdf = _linkSdf->GetElement("collision");
    collisionSdf->GetAttribute("name")->Set(_name);
    sdf::ElementPtr collPoseSdf = collisionSdf->GetElement("pose");
    collPoseSdf->Set(_pose);
    sdf::ElementPtr geomColSdf = collisionSdf->GetElement("geometry");
    sdf::ElementPtr sphereColSdf = geomColSdf->GetElement("sphere");
    sphereColSdf->GetElement("radius")->Set(_radius);
}

//////////////////////////////////////////////////
void ActorFactoryPlugin::AddBoxVisual(sdf::ElementPtr _linkSdf, const std::string &_name,
                                      const math::Pose &_pose, const math::Vector3 &_size,
                                      const std::string &_material, const common::Color &_ambient)
{
    sdf::ElementPtr visualSdf = _linkSdf->AddElement("visual");
    visualSdf->GetAttribute("name")->Set(_name);
    sdf::ElementPtr visualPoseSdf = visualSdf->GetElement("pose");
    visualPoseSdf->Set(_pose);
    sdf::ElementPtr geomVisSdf = visualSdf->GetElement("geometry");
    sdf::ElementPtr boxSdf = geomVisSdf->GetElement("box");
    boxSdf->GetElement("size")->Set(_size);
    sdf::ElementPtr matSdf = visualSdf->GetElement("material");
    matSdf->GetElement("script")->Set(_material);
    sdf::ElementPtr colorSdf = matSdf->GetElement("ambient");
    colorSdf->Set(_ambient);
}


//TODO Guess this is hard Coded in my model SDF
//////////////////////////////////////////////////
void ActorFactoryPlugin::AddActorVisual(sdf::ElementPtr _linkSdf, const std::string &_name,
                                        const math::Pose &_pose)
{
    /*
    sdf::ElementPtr visualSdf = _linkSdf->AddElement("visual");
    visualSdf->GetAttribute("name")->Set(_name);
    sdf::ElementPtr visualPoseSdf = visualSdf->GetElement("pose");
    visualPoseSdf->Set(_pose);
    sdf::ElementPtr geomVisSdf = visualSdf->GetElement("geometry");

    sdf::ElementPtr meshSdf = geomVisSdf->GetElement("mesh");
    meshSdf->GetElement("filename")->Set(1.0);
    meshSdf->GetElement("scale")->Set(math::Vector3(10.0,10.0,10.0));
    */
}


void ActorFactoryPlugin::AddPlugin(sdf::ElementPtr _sdf,
                                   std::string name, std::string filename)
{
    sdf::ElementPtr pluginSDF = _sdf->AddElement("plugin");
    pluginSDF->GetAttribute("name")->Set(name);
    pluginSDF->GetAttribute("filename")->Set(filename);
}

}
