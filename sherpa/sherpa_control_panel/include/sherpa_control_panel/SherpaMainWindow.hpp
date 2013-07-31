#ifndef __SHERPA_MAIN_WINDOW_H__
#define __SHERPA_MAIN_WINDOW_H__

/*****************************************************************************
** Includes
*****************************************************************************/

#include <QtGui/QMainWindow>

#include "ui_SherpaMain.h"
#include "ui_WorldDock.h"
#include "ui_RosDock.h"
#include "ui_LogDock.h"

#include "SherpaRobotDock.hpp"
#include "SherpaRobotStateListener.hpp"
#include "RosConnector.hpp"

/*****************************************************************************
** Namespace
*****************************************************************************/

namespace sherpa_control_panel {

/*****************************************************************************
** Interface [MainWindow]
*****************************************************************************/
/**
 * @brief Qt central, all operations relating to the view part here.
 */
class SherpaMainWindow : public QMainWindow, SherpaRobotStateListener{
    Q_OBJECT

public:
    SherpaMainWindow(int argc, char** argv, QWidget *parent = 0);
    ~SherpaMainWindow();

    void ReadSettings(); // Load up qt program settings at startup
    void WriteSettings(); // Save qt program settings when closing

    void closeEvent(QCloseEvent *event); // Overloaded function
    void showNoMasterMessage();

public Q_SLOTS:
    /******************************************
    ** Auto-connections (connectSlotsByName())
    *******************************************/
    void on_actionAbout_triggered();
    void on_button_connect_clicked(bool check );
    void on_checkbox_use_environment_stateChanged(int state);

    /******************************************
    ** Manual connections
    *******************************************/
    void updateLoggingView(); // no idea why this can't connect automatically

private:
    QDockWidget *rosDock;
    QDockWidget *worldDock;
    QDockWidget *logDock;
    SherpaRobotDock *robotDock;
    //QDockWidget rosDock;
    Ui::SherpaMainWindow mainUi;
    Ui::WorldSettingsDock worldUi;
    Ui::LogDock logUi;
    Ui::RosDock rosUi;
    SherpaRosConnector rosNode;
};

}  // namespace qt_test

#endif // qt_test_MAIN_WINDOW_H
