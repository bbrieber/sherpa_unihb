/**
 * @file /src/qnode.cpp
 *
 * @brief Ros communication central!
 *
 * @date February 2011
 **/

/*****************************************************************************
** Includes
*****************************************************************************/

#include <ros/ros.h>
#include <ros/network.h>
#include <string>
#include <std_msgs/String.h>


#include <iostream>
#include <algorithm>
#include <sstream>
#include <QDate>
#include "../include/sherpa_control_panel/RosConnector.hpp"



#include <gazebo_msgs/GetWorldProperties.h>
#include <gazebo_msgs/GetModelProperties.h>
#include <gazebo_msgs/GetModelState.h>
#include <gazebo_msgs/SpawnModel.h>
//#include <gazebo_msgs/GetWorldPropertiesRequest.h>
//#include <gazebo_msgs/GetWorldPropertiesResponse.h>

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace sherpa_control_panel {

/*****************************************************************************
** Implementation
*****************************************************************************/

SherpaRosConnector::SherpaRosConnector(int argc, char** argv ) :
	init_argc(argc),
	init_argv(argv)
	{
  
}

SherpaRosConnector::~SherpaRosConnector() {
    if(ros::isStarted()) {
      ros::shutdown(); // explicitly needed since we use ros::start();
      ros::waitForShutdown();
    }
	wait();
}

bool SherpaRosConnector::init() {
	ros::init(init_argc,init_argv,"sherpa_control_panel");
	if ( ! ros::master::check() ) {
		return false;
	}
	ros::start(); // explicitly needed since our nodehandle is going out of scope.
	
	n = new ros::NodeHandle();
	// Add your ros communications here.
	//chatter_publisher = n.advertise<std_msgs::String>("/sherpa_send_test", 1000);
	log_subscriber = n->subscribe<sherpa_msgs::SherpaLog>("/sherpa/log", 1000, &SherpaRosConnector::logCallback,this);
	ROS_INFO_STREAM("INITIALIZED SUBSCRIBER");
	start();
	return true;
}



void SherpaRosConnector::logCallback(const sherpa_msgs::SherpaLog::ConstPtr& msg)
{
  ROS_INFO("I heard: [%s]", msg->log_message.c_str());
  QList<QStandardItem*> list;//=new QList<*QStandardItem>;
  list<<new QStandardItem(QTime().currentTime().toString());
  
  switch ( msg->log_level ) {
		case(1) : {
				ROS_DEBUG_STREAM(msg);
				list << new QStandardItem(QString("DEBUG"));
				break;
		}
		case(2) : {
				ROS_INFO_STREAM(msg);
				list << new QStandardItem(QString("INFO"));
				break;
		}
		case(3) : {
				ROS_WARN_STREAM(msg);
				list << new QStandardItem(QString("INFO"));
				break;
		}
		case(4) : {
				ROS_ERROR_STREAM(msg);
				list << new QStandardItem(QString("ERROR"));
				break;
		}
		case(5) : {
				ROS_FATAL_STREAM(msg);
				list << new QStandardItem(QString("FATAL"));
				break;
		}
		default:{
			  list << new QStandardItem(QString("UNKNOWN"));
				break;
		  
		}
	}
  list<< new QStandardItem(QString(msg->tag.c_str()));
  list<< new QStandardItem(QString(msg->log_message.c_str()));
  logModel->appendRow(list);
  Q_EMIT loggingUpdated();
  
}

bool SherpaRosConnector::init(const std::string &master_url, const std::string &host_url) {
	std::map<std::string,std::string> remappings;
	remappings["__master"] = master_url;
	remappings["__hostname"] = host_url;
	ros::init(remappings,"sherpa_control_panel");
	if ( ! ros::master::check() ) {
		return false;
	}
	ros::start(); // explicitly needed since our nodehandle is going out of scope.
	//ros::NodeHandle n;
	// Add your ros communications here.
	//chatter_publisher = n.advertise<std_msgs::String>("chatter", 1000);
	
	start();
	return true;
}

void SherpaRosConnector::run() {
	ros::Rate loop_rate(1);
	int count = 0;
	while ( ros::ok() ) {

		std_msgs::String msg;
		std::stringstream ss;
		ss << "hello world " << count;
		msg.data = ss.str();
		//chatter_publisher.publish(msg);
		//log(Info,std::string("HEY HO: ")+msg.data);
		ros::spinOnce();
		loop_rate.sleep();
		++count;
	}
	std::cout << "Ros shutdown, proceeding to close the gui." << std::endl;
	Q_EMIT rosShutdown(); // used to signal the gui for a shutdown (useful to roslaunch)
}


void SherpaRosConnector::log( const LogLevel &level, const std::string &msg) {
	logging_model.insertRows(logging_model.rowCount(),1);
	std::stringstream logging_model_msg;
	switch ( level ) {
		case(Debug) : {
				ROS_DEBUG_STREAM(msg);
				logging_model_msg << "[DEBUG] [" << ros::Time::now() << "]: " << msg;
				break;
		}
		case(Info) : {
				ROS_INFO_STREAM(msg);
				logging_model_msg << "[INFO] [" << ros::Time::now() << "]: " << msg;
				break;
		}
		case(Warn) : {
				ROS_WARN_STREAM(msg);
				logging_model_msg << "[INFO] [" << ros::Time::now() << "]: " << msg;
				break;
		}
		case(Error) : {
				ROS_ERROR_STREAM(msg);
				logging_model_msg << "[ERROR] [" << ros::Time::now() << "]: " << msg;
				break;
		}
		case(Fatal) : {
				ROS_FATAL_STREAM(msg);
				logging_model_msg << "[FATAL] [" << ros::Time::now() << "]: " << msg;
				break;
		}
	}
	QVariant new_row(QString(logging_model_msg.str().c_str()));
	logging_model.setData(logging_model.index(logging_model.rowCount()-1),new_row);
	Q_EMIT loggingUpdated(); // used to readjust the scrollbar
}


void SherpaRosConnector::requestWorldInfo(){
    ros::ServiceClient client = n->serviceClient<gazebo_msgs::GetWorldProperties>("/gazebo/get_world_properties");
    gazebo_msgs::GetWorldProperties srv;
    
    //logmsg(sherpa_control_panel::SherpaRosConnector::Warn, QString("Heyho"), QString("BAR"));
    if (client.call(srv))
    {
      //gazebo_msgs::GetWorldPropertiesResponse::_status_message_type s =srv.response.status_message;
      //gazebo_msgs::GetWorldPropertiesResponse::_model_names_type names = srv.response.model_names;
      //ROS_INFO("Modes %s",srv.response.model_names[0].c_str());
      
      for(int i=0;i<srv.response.model_names.size();i++){
	gazebo_msgs::GetModelProperties mp_srv;
	ROS_INFO("Model %s",srv.response.model_names[i].c_str());
	/*
	QString name(srv.response.model_names[i].c_str());
	//name.contains(new QString)
	mp_srv.request.model_name =srv.response.model_names[i].c_str(); 
	if (client.call(mp_srv))
	{
	  //mp_srv.response.
	}
	
	gazebo_msgs::GetModelState ms_srv;
	ms_srv.request.model_name =srv.response.model_names[i].c_str(); 
	if (client.call(ms_srv))
	{
	  //ms_srv.response.
	}
	*/
      }
    
    }
    else
    {
      ROS_ERROR("Failed /gazebo/get_world_properties");
      return;
    }
}


std::string SherpaRosConnector::getImagePath(){
  ros::NodeHandle rosNode;
  std::string description;
    if(!rosNode.getParam("/sherpa/map_image_path",description)){
	ROS_ERROR("NO IMAGE PARAM SET");
    }
    return description;
}



void SherpaRosConnector::spawnRobot(std::string robot, double x, double y, double z){
  std::string description;
  std::string robot_name = robot.c_str();
  QString tmp = QString("/");
  char str[80];
  str[0] = '/';
  strcat(str , robot.c_str());
  
  std::string robot_namespace = std::string(str);
  
  
  ros::NodeHandle tmpNode(robot_namespace);
    if(!tmpNode.getParam("robot_description",description)){
	ROS_ERROR("NO ROBOT DESCRIPTION [%s] FOUND IN NAMESPACE [%s]","robot_description", robot_namespace.c_str());
	return;
    }
    
  ros::ServiceClient client = n->serviceClient<gazebo_msgs::SpawnModel>("/gazebo/spawn_urdf_model");
    gazebo_msgs::SpawnModel srv;
    srv.request.model_name = robot_name;
    srv.request.robot_namespace = robot_name;
    srv.request.model_xml = description;
    /*
    "\
    <?xml version='1.0'?>\
    <sdf version='1.3'>\
    <model name=\"my_robot\">\
    <static>true</static>\
      <link name='chassis'>\
        <pose>0 0 .1 0 0 0</pose>\
        <collision name='collision'>\
          <geometry>\
            <box>\
              <size>.4 .2 .1</size>\
            </box>\
          </geometry>\
        </collision>\
        <visual name='visual'>\
          <geometry>\
            <box>\
              <size>.4 .2 .1</size>\
            </box>\
          </geometry>\
        </visual>\
      </link>\
   </model>\
</sdf>";
    */
    //logmsg(sherpa_control_panel::SherpaRosConnector::Warn, QString("Heyho"), QString("BAR"));
    if (client.call(srv))
    {
      
    }
}

void SherpaRosConnector::logmsg(LogLevel level, QString msg, QString tag)
{
  QList<QStandardItem*> list;//=new QList<*QStandardItem>;
  list<<new QStandardItem(QTime().currentTime().toString());
  
  switch ( level ) {
		case(Debug) : {
				list << new QStandardItem(QString("DEBUG"));
				break;
		}
		case(Info) : {
				list << new QStandardItem(QString("INFO"));
				break;
		}
		case(Warn) : {
				list << new QStandardItem(QString("Warn"));
				break;
		}
		case(Error) : {
				list << new QStandardItem(QString("ERROR"));
				break;
		}
		case(Fatal) : {
				list << new QStandardItem(QString("FATAL"));
				break;
		}
	}
  list<< new QStandardItem(msg);
  list<< new QStandardItem(tag);
  logModel->appendRow(list);
  Q_EMIT loggingUpdated();
  
}


}  // namespace qt_test
