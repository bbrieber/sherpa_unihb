/**
 * @file /src/main_window.cpp
 *
 * @brief Implementation for the qt gui.
 *
 * @date February 2011
 **/
/*****************************************************************************
** Includes
*****************************************************************************/

#include <QtGui>
#include <QMessageBox>
#include <iostream>
#include "../include/sherpa_control_panel/SherpaMainWindow.hpp"

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace sherpa_control_panel {

using namespace Qt;

/*****************************************************************************
** Implementation [MainWindow]
*****************************************************************************/

SherpaMainWindow::SherpaMainWindow(int argc, char** argv, QWidget *parent)
    : QMainWindow(parent),
    rosNode(argc,argv)
{
    
    rosDock = new QDockWidget(this);
    worldDock = new QDockWidget(this);
    logDock = new QDockWidget(this);
    robotDock = new SherpaRobotDock(this);
    mainUi.setupUi(this); // Calling this incidentally connects all ui's triggers to on_...() callbacks in this class.
    rosUi.setupUi(rosDock);
    worldUi.setupUi(worldDock);
    logUi.setupUi(logDock);

    QStandardItemModel *model = new QStandardItemModel(2,3,this); //2 Rows and 3 Columns
    model->setHorizontalHeaderItem(0, new QStandardItem(QString("Time")));
    model->setHorizontalHeaderItem(1, new QStandardItem(QString("Log Level")));
    model->setHorizontalHeaderItem(2, new QStandardItem(QString("Tag")));
    model->setHorizontalHeaderItem(3, new QStandardItem(QString("Data")));
    model->setColumnCount(4);	
    logUi.logTable->horizontalHeader()->setStretchLastSection(true);
    //logUi.logTable->horizontalHeader()->setResizeMode(3,QHeaderView::Stretch);

    logUi.logTable->setModel(model);
    
    rosNode.setLogModel(model);
    addDockWidget(Qt::LeftDockWidgetArea, rosDock);
    addDockWidget(Qt::RightDockWidgetArea, worldDock);
    addDockWidget(Qt::RightDockWidgetArea, robotDock);
    addDockWidget(Qt::BottomDockWidgetArea, logDock);
    rosDock->setAllowedAreas(Qt::AllDockWidgetAreas);
    worldDock->setAllowedAreas(Qt::AllDockWidgetAreas);
    robotDock->setAllowedAreas(Qt::AllDockWidgetAreas);
    logDock->setAllowedAreas(Qt::AllDockWidgetAreas);
    //QObject::connect(ui.actionAbout_Qt, SIGNAL(triggered(bool)), qApp, SLOT(aboutQt())); // qApp is a global variable for the application

   // ReadSettings();
    setWindowIcon(QIcon(":/images/icon.png"));
    //mainUi.tab_manager->setCurrentIndex(0); // ensure the first tab is showing - qt-designer should have this already hardwired, but often loses it (settings?).
    QObject::connect(&rosNode, SIGNAL(rosShutdown()), this, SLOT(close()));

    /*********************
    ** Logging
    **********************/
    //mainUi.view_logging->setModel(rosNode.loggingModel());
    QObject::connect(&rosNode, SIGNAL(loggingUpdated()), this, SLOT(updateLoggingView()));
    rosNode.init();
    QObject::connect(&rosNode, SIGNAL(loggingUpdated()), this, SLOT(updateLoggingView()));
    rosNode.requestWorldInfo();
    sherpa_robot sr;
    sr.name = new QString("sherpa_rover");
    //rosNode.spawnRobot(sr);
}

SherpaMainWindow::~SherpaMainWindow() {}

/*****************************************************************************
** Implementation [Slots]
*****************************************************************************/

void SherpaMainWindow::showNoMasterMessage() {
    QMessageBox msgBox;
    msgBox.setText("Couldn't find the ros master.");
    msgBox.exec();
    close();
}

/*
 * These triggers whenever the button is clicked, regardless of whether it
 * is already checked or not.
 */

void SherpaMainWindow::on_button_connect_clicked(bool check ) {

}


void SherpaMainWindow::on_checkbox_use_environment_stateChanged(int state) {

}

/*****************************************************************************
** Implemenation [Slots][manually connected]
*****************************************************************************/

/**
 * This function is signalled by the underlying model. When the model changes,
 * this will drop the cursor down to the last line in the QListview to ensure
 * the user can always see the latest log message.
 */
void SherpaMainWindow::updateLoggingView() {
        logUi.logTable->scrollToBottom();
        logUi.logTable->update();
 
}

/*****************************************************************************
** Implementation [Menu]
*****************************************************************************/

void SherpaMainWindow::on_actionAbout_triggered() {
    //QMessageBox::about(this, tr("About ..."),tr("<h2>PACKAGE_NAME Test Program 0.10</h2><p>Copyright Yujin Robot</p><p>This package needs an about description.</p>"));
}

/*****************************************************************************
** Implementation [Configuration]
*****************************************************************************/

void SherpaMainWindow::ReadSettings() {
    QSettings settings("Qt-Ros Package", "qt_test");
    restoreGeometry(settings.value("geometry").toByteArray());
    restoreState(settings.value("windowState").toByteArray());

}

void SherpaMainWindow::WriteSettings() {
    QSettings settings("Qt-Ros Package", "qt_test");
    //settings.setValue("topic_name",ui.line_edit_topic->text());
    settings.setValue("geometry", saveGeometry());
    settings.setValue("windowState", saveState());
}

void SherpaMainWindow::closeEvent(QCloseEvent *event)
{
    WriteSettings();
    QMainWindow::closeEvent(event);
}

}  // namespace qt_test

