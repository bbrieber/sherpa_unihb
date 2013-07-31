#include <gazebo/common/Plugin.hh>
#include <ros/ros.h>
#include <ros/network.h>
#include <iostream>
#include <image_transport/image_transport.h>
#include "sensor_msgs/CompressedImage.h"
#include <cv_bridge/cv_bridge.h>

//#include <cv_bridge/CvBridge.h>

#include <opencv2/opencv.hpp>
//#include <opencv/cvwimage.h>
//#include <opencv/highgui.h>

#include "sherpa_msgs/ConvertFromMapPosition.h"
#include "sherpa_msgs/ConvertToMapPosition.h"

namespace gazebo
{
class SherpaMapPlugin : public ModelPlugin
{
public:
    void Load(physics::ModelPtr _parent, sdf::ElementPtr /*_sdf*/)
    {
	this->model = _parent;
	
	std::string name = "gazebo_model_plugin";
        int argc = 0;
        //ros::init(argc, NULL, name);
	std::cerr<<"HELLO FOO";
        //
	
	ros::NodeHandle node("/sherpa/map");
	
        //my_rosnode = &node;
	
	/*
	image_transport::ImageTransport it(node);
	image_transport::Publisher pub = it.advertise("image", 0);
	cv_bridge::CvImagePtr cv_ptr;
	cv_ptr.reset (new cv_bridge::CvImage);
	std::string img_name = "/home/bbrieber/catkin_ws/src/sherpa/sherpa_map_description/images/terrain.png";
	
	cv_ptr->image = cv::imread(img_name);
	sensor_msgs::ImagePtr msg = cv_ptr->toImageMsg();
	pub.publish(msg);
	*/
	//sensor_msgs::cv_bridge::cvToImgMsg();
	//sensor_msgs::ImagePtr msg = sensor_msgs::CvBridge::cvToImgMsg(cv_ptr->image, "bgr8");
	//pub.publish(cv_ptr);
	//cv::imread(img_name);
	//sensor_msgs::CompressedImage msg;//= sherpa_msgs::CompressedImage();
	//msg.format = "png";
	//msg.data = 
	//pub.publish(msg);
	
//	sensor_msgs::ImagePtr image = sensor_msgs::Com
    }

    // Called by the world update
private:
    physics::ModelPtr model;
    ros::NodeHandle* my_rosnode;

};

// Register this plugin with the simulator
GZ_REGISTER_MODEL_PLUGIN(SherpaMapPlugin)
}

