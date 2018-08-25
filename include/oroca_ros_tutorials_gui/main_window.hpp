/**
 * @file /include/oroca_ros_tutorials_gui/main_window.hpp
 *
 * @brief Qt based gui for oroca_ros_tutorials_gui.
 *
 * @date November 2010
 **/
#ifndef oroca_ros_tutorials_gui_MAIN_WINDOW_H
#define oroca_ros_tutorials_gui_MAIN_WINDOW_H

/*****************************************************************************
** Includes
*****************************************************************************/

#include <QtGui/QMainWindow>
#include "ui_main_window.h"
#include "qnode.hpp"

/*****************************************************************************
** Namespace
*****************************************************************************/

namespace oroca_ros_tutorials_gui {

/*****************************************************************************
** Interface [MainWindow]
*****************************************************************************/
/**
 * @brief Qt central, all operations relating to the view part here.
 */
class MainWindow : public QMainWindow {
Q_OBJECT

public:
	MainWindow(int argc, char** argv, QWidget *parent = 0);
	~MainWindow();

	void ReadSettings(); // Load up qt program settings at startup
	void WriteSettings(); // Save qt program settings when closing

	void closeEvent(QCloseEvent *event); // Overloaded function
	void showNoMasterMessage();
        void updateResultLineEdit(int srv_response);

public Q_SLOTS:
	/******************************************
	** Auto-connections (connectSlotsByName())
	*******************************************/
	void on_actionAbout_triggered();
	void on_button_connect_clicked(bool check );
	void on_checkbox_use_environment_stateChanged(int state);
        void changeOperator();
        void on_send_pushButton_clicked(bool checked);


    /******************************************
    ** Manual connections
    *******************************************/
    void updateLoggingView(); // no idea why this can't connect automatically



private:
	Ui::MainWindowDesign ui;
	QNode qnode;

};

}  // namespace oroca_ros_tutorials_gui

#endif // oroca_ros_tutorials_gui_MAIN_WINDOW_H
