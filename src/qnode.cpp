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
#include <sstream>
#include "../include/oroca_ros_tutorials_gui/qnode.hpp"

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace oroca_ros_tutorials_gui {

/*****************************************************************************
** Implementation
*****************************************************************************/

QNode::QNode(int argc, char** argv ) :
	init_argc(argc),
	init_argv(argv)
	{}

QNode::~QNode() {
    if(ros::isStarted()) {
      ros::shutdown(); // explicitly needed since we use ros::start();
      ros::waitForShutdown();
    }
	wait();
}

bool QNode::init() {
	ros::init(init_argc,init_argv,"oroca_ros_tutorials_gui");
	if ( ! ros::master::check() ) {
		return false;
	}
	ros::start(); // explicitly needed since our nodehandle is going out of scope.
	ros::NodeHandle n;
	// Add your ros communications here.
	chatter_publisher = n.advertise<std_msgs::String>("chatter", 1000);
        ros_tutorial_sub = n.subscribe("ros_tutorial_msg", 100, &QNode::msgCallback, this);
        ros_tutorial_service_client = n.serviceClient<oroca_ros_tutorials_msgs::srvTutorial>("ros_tutorial_srv", 10);
	start();
	return true;
}

bool QNode::init(const std::string &master_url, const std::string &host_url) {
	std::map<std::string,std::string> remappings;
	remappings["__master"] = master_url;
	remappings["__hostname"] = host_url;
	ros::init(remappings,"oroga_ros_tutorials_gui");
	if ( ! ros::master::check() ) {
		return false;
	}
	ros::start(); // explicitly needed since our nodehandle is going out of scope.
	ros::NodeHandle n;
	// Add your ros communications here.
	chatter_publisher = n.advertise<std_msgs::String>("chatter", 1000);
        ros_tutorial_sub = n.subscribe("ros_tutorial_msg", 100, &QNode::msgCallback, this);
        ros_tutorial_service_client = n.serviceClient<oroca_ros_tutorials_msgs::srvTutorial>("ros_tutorial_srv", 10);
	return true;
}

void QNode::sendSetOperator(std::string index)
{
    ros::NodeHandle n;

    if(!strncmp(index.c_str(), "PLUS", 4))
        n.setParam("calculation_method", 1);

    if(!strncmp(index.c_str(), "MINUS", 4))
        n.setParam("calculation_method", 2);

    if(!strncmp(index.c_str(), "MULTIPLICATION", 4))
        n.setParam("calculation_method", 3);

    if(!strncmp(index.c_str(), "DIVISION", 4))
        n.setParam("calculation_method", 4);
}

void QNode::sendSetNumber(int res_a, int res_b)
{
    oroca_ros_tutorials_msgs::srvTutorial srv;
    srv.request.a = res_a;
    srv.request.b = res_b;

    std::string receiveMsg;
    std::stringstream ss;
    ss << "here we come !!!!!!!!!!!!!!!!!!!!!: ";
    receiveMsg = ss.str();
    log(Info, receiveMsg);

    if (ros_tutorial_service_client.call(srv))
        Q_EMIT updateServiceResult(srv.response.result);
    else
    {
        ss << "Error!!!!!!!!!!!!!!!!!!!!!!!!";
        receiveMsg = ss.str();
        log(Info, receiveMsg);
    }

}





void QNode::msgCallback(const oroca_ros_tutorials_msgs::msgTutorial::ConstPtr &msg)
{
	std::string receiveMsg;
	std::stringstream ss;
	ss << "receive msg : " << msg->data;
	receiveMsg = ss.str();
	log(Info, receiveMsg);
}

void QNode::run() {
	ros::Rate loop_rate(1);
	int count = 0;
	while ( ros::ok() ) {

		std_msgs::String msg;
		std::stringstream ss;
		ss << "hello world " << count;
		msg.data = ss.str();
		chatter_publisher.publish(msg);
		log(Info,std::string("I sent: ")+msg.data);
		ros::spinOnce();
		loop_rate.sleep();
		++count;
	}
	std::cout << "Ros shutdown, proceeding to close the gui." << std::endl;
	Q_EMIT rosShutdown(); // used to signal the gui for a shutdown (useful to roslaunch)
}


void QNode::log( const LogLevel &level, const std::string &msg) {
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

}  // namespace oroga_ros_tutorials_gui
