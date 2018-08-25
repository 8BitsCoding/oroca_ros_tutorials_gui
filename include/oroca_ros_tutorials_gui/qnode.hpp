/**
 * @file /include/oroca_ros_tutorials_gui/qnode.hpp
 *
 * @brief Communications central!
 *
 * @date February 2011
 **/
/*****************************************************************************
** Ifdefs
*****************************************************************************/

#ifndef oroca_ros_tutorials_gui_QNODE_HPP_
#define oroca_ros_tutorials_gui_QNODE_HPP_

/*****************************************************************************
** Includes
*****************************************************************************/

// To workaround boost/qt4 problems that won't be bugfixed. Refer to
//    https://bugreports.qt.io/browse/QTBUG-22829
#ifndef Q_MOC_RUN
#include <ros/ros.h>
//#include <oroca_ros_tutorials_msgs/msgTutorial.h>
#endif
#include <string>
#include <QThread>
#include <QStringListModel>
#include "../../../../devel/include/oroca_ros_tutorials_msgs/msgTutorial.h"
#include "../../../../devel/include/oroca_ros_tutorials_msgs/srvTutorial.h"


/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace oroca_ros_tutorials_gui {

/*****************************************************************************
** Class
*****************************************************************************/

class QNode : public QThread {
    Q_OBJECT
public:
	QNode(int argc, char** argv );
	virtual ~QNode();
	bool init();
	bool init(const std::string &master_url, const std::string &host_url);
	void run();

	/*********************
	** Logging
	**********************/
	enum LogLevel {
	         Debug,
	         Info,
	         Warn,
	         Error,
	         Fatal
	 };

	QStringListModel* loggingModel() { return &logging_model; }
	void log( const LogLevel &level, const std::string &msg);
	void msgCallback(const oroca_ros_tutorials_msgs::msgTutorial::ConstPtr &msg);
public:
        void sendSetOperator(std::string index);
        void sendSetNumber(int res_a, int res_b);

Q_SIGNALS:
	void loggingUpdated();
    void rosShutdown();
    void updateServiceResult(int);


private:
	int init_argc;
	char** init_argv;
	ros::Publisher chatter_publisher;
	ros::Subscriber ros_tutorial_sub;
        ros::ServiceClient ros_tutorial_service_client;
    QStringListModel logging_model;
};

}  // namespace oroca_ros_tutorials_gui

#endif /* oroca_ros_tutorials_gui_QNODE_HPP_ */
