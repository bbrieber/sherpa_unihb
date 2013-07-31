
#ifndef __SHERPA_ROS_CONNECTOR_HPP__
#define __SHERPA_ROS_CONNECTOR_HPP__

/*****************************************************************************
** Includes
*****************************************************************************/

#include <ros/ros.h>
#include <string>
#include <QThread>
#include <QStringListModel>
#include <QStandardItemModel>

#include <sherpa_msgs/SherpaLog.h>
#include "Singleton.hpp"
#include "SherpaRobot.hpp"
/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace sherpa_control_panel {

/*****************************************************************************
** Class
*****************************************************************************/

class SherpaRosConnector : public QThread  {
    Q_OBJECT
public:
    SherpaRosConnector(int argc, char** argv );
    virtual ~SherpaRosConnector();
        enum LogLevel {
        Debug,
        Info,
        Warn,
        Error,
        Fatal
    };
    
    bool init();
    bool init(const std::string &master_url, const std::string &host_url);
    void run();
    void logCallback(const sherpa_msgs::SherpaLog::ConstPtr& msg);
    void requestWorldInfo();
    void logmsg(LogLevel level, QString msg, QString tag);
    void spawnRobot(std::string robot, double x, double y, double z);
    std::string getImagePath();
    //void spawnRobot(sherpa_robot robot);
    /*********************
    ** Logging
    **********************/


    QStringListModel* loggingModel() {
        return &logging_model;    //remove this
    }
    void log( const LogLevel &level, const std::string &msg);
    void setLogModel(QStandardItemModel *model) {
        logModel = model;
    }
Q_SIGNALS:
    void loggingUpdated();
    void rosShutdown();
    void robotDescriptionChanged();
    void robotPositionChanged();

private:
    //SherpaRosConnector();
    int init_argc;
    char** init_argv;
    ros::Publisher chatter_publisher;
    ros::Subscriber log_subscriber;
    QStringListModel logging_model;
    QStandardItemModel *logModel;
    ros::NodeHandle *n;

};


}  // namespace qt_test

#endif /* __SHERPA_ROS_CONNECTOR_HPP__ */
