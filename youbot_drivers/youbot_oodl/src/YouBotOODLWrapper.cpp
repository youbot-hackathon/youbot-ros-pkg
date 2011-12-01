/******************************************************************************
* Copyright (c) 2011
* Locomotec
*
* Author:
* Sebastian Blumenthal
*
*
* This software is published under a dual-license: GNU Lesser General Public
* License LGPL 2.1 and BSD license. The dual-license implies that users of this
* code may choose which terms they prefer.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
* * Redistributions of source code must retain the above copyright
* notice, this list of conditions and the following disclaimer.
* * Redistributions in binary form must reproduce the above copyright
* notice, this list of conditions and the following disclaimer in the
* documentation and/or other materials provided with the distribution.
* * Neither the name of Locomotec nor the names of its
* contributors may be used to endorse or promote products derived from
* this software without specific prior written permission.
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License LGPL as
* published by the Free Software Foundation, either version 2.1 of the
* License, or (at your option) any later version or the BSD license.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU Lesser General Public License LGPL and the BSD license for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License LGPL and BSD license along with this program.
*
******************************************************************************/

#include "YouBotOODLWrapper.h"
#include <sstream>

namespace youBot {


YouBotOODLWrapper::YouBotOODLWrapper() {

}

YouBotOODLWrapper::YouBotOODLWrapper(ros::NodeHandle n) :
	node(n)
{

	youBotConfiguration.hasBase = false;
	youBotConfiguration.hasArms = false;

	youBotChildFrameID = "base_link"; //holds true for both: base and arm
	armJointStateMessages.clear();

}

YouBotOODLWrapper::~YouBotOODLWrapper() {
	this->stop();
}

void YouBotOODLWrapper::initializeBase(std::string baseName) {

	try {
		ROS_INFO("Configuration file path: %s", youBotConfiguration.configurationFilePath.c_str());
		youBotConfiguration.baseConfiguration.youBotBase = new youbot::YouBotBase(baseName, youBotConfiguration.configurationFilePath);
		youBotConfiguration.baseConfiguration.youBotBase->doJointCommutation();
//	} catch (youbot::FileNotFoundException& e) {
	} catch (std::exception& e) {
		std::string errorMessage = e.what();
		ROS_FATAL("Cannot open youBot driver: \n %s ", errorMessage.c_str());
		ROS_ERROR("Base \"%s\" could not be initialized.", baseName.c_str());
		youBotConfiguration.hasBase = false;
		return;
	}

	/* setup input/output communication */
	youBotConfiguration.baseConfiguration.baseCommandSubscriber = node.subscribe("cmd_vel", 1000, &YouBotOODLWrapper::baseCommandCallback, this);
	youBotConfiguration.baseConfiguration.baseOdometryPublisher = node.advertise<nav_msgs::Odometry>("odom", 1);
//	youBotConfiguration.baseConfiguration.baseJointStatePublisher = node.advertise<sensor_msgs::JointState>("base_joint_states", 1);
	youBotConfiguration.baseConfiguration.baseJointStatePublisher = node.advertise<sensor_msgs::JointState>("base/joint_states", 1);

	/* setup frame_ids */
	youBotOdometryFrameID = "odom";
	youBotOdometryChildFrameID = "base_footprint";

	ROS_INFO("Base is initialized.");
	youBotConfiguration.hasBase = true;
}


void YouBotOODLWrapper::initializeArm(std::string armName, bool enableStandardGripper) {
	int armIndex;
	youbot::JointName jointNameParameter;
	std::string jointName;
	stringstream topicName;

	try {
		ROS_INFO("Configuration file path: %s", youBotConfiguration.configurationFilePath.c_str());
		YouBotArmConfiguration tmpArmConfig;
		youBotConfiguration.youBotArmConfigurations.push_back(tmpArmConfig);
		armIndex = static_cast<int>(youBotConfiguration.youBotArmConfigurations.size()) - 1;
		youBotConfiguration.youBotArmConfigurations[armIndex].youBotArm = new youbot::YouBotManipulator(armName, youBotConfiguration.configurationFilePath);
		youBotConfiguration.youBotArmConfigurations[armIndex].armID = armName;
		topicName.str("");
		topicName << "arm_" << (armIndex +1) << "/";
		youBotConfiguration.youBotArmConfigurations[armIndex].commandTopicName = topicName.str(); // e.g. arm_1/
		youBotConfiguration.youBotArmConfigurations[armIndex].parentFrameIDName = "base_link";
		youBotConfiguration.armNameToArmIndexMapping.insert(make_pair(armName, static_cast<int>(youBotConfiguration.youBotArmConfigurations.size())));

		/* take joint names form configuration files */
		youBotConfiguration.youBotArmConfigurations[armIndex].jointNames.clear();
		for (int i = 0; i < youBotArmDoF; ++i) {
			youBotConfiguration.youBotArmConfigurations[armIndex].youBotArm->getArmJoint(i+1).getConfigurationParameter(jointNameParameter);
			jointNameParameter.getParameter(jointName);
			youBotConfiguration.youBotArmConfigurations[armIndex].jointNames.push_back(jointName);
			ROS_INFO("Joint %i for arm %s has name: %s", i+1, youBotConfiguration.youBotArmConfigurations[armIndex].armID.c_str(), youBotConfiguration.youBotArmConfigurations[armIndex].jointNames[i].c_str());

		}


		youBotConfiguration.youBotArmConfigurations[armIndex].youBotArm->doJointCommutation();
		youBotConfiguration.youBotArmConfigurations[armIndex].youBotArm->calibrateManipulator();
		if(enableStandardGripper) {
			youBotConfiguration.youBotArmConfigurations[armIndex].youBotArm->calibrateGripper();
		}
	} catch (std::exception& e) {
		youBotConfiguration.youBotArmConfigurations.pop_back();
		std::string errorMessage = e.what();
		ROS_FATAL("Cannot open youBot driver: \n %s ", errorMessage.c_str());
		ROS_ERROR("Arm \"%s\" could not be initialized.", armName.c_str());
		ROS_INFO("System has %i initialized arm(s).", static_cast<int>(youBotConfiguration.youBotArmConfigurations.size()));
		return;
	}

	/* (optional) set all joints into velocity mode -> so the arm can be manually moved */
	youbot::JointVelocitySetpoint jointVelocity;
	for (int i = 0; i < youBotArmDoF; ++i) {
		jointVelocity.angularVelocity = 0.0 * radian_per_second;
		try {
			youBotConfiguration.youBotArmConfigurations[armIndex].youBotArm->getArmJoint(i+1).setData(jointVelocity);
		} catch (std::exception& e) {
			std::string errorMessage = e.what();
			ROS_WARN("Cannot set arm velocity %i: \n %s", i+1, errorMessage.c_str());
		}
	}

	/* setup input/output communication */
	topicName.str("");
	topicName << youBotConfiguration.youBotArmConfigurations[armIndex].commandTopicName << "arm_controller/position_command"; // e.g. arm_1/arm_controller/positionCommand
	youBotConfiguration.youBotArmConfigurations[armIndex].armPositionCommandSubscriber = node.subscribe<brics_actuator::JointPositions>(topicName.str(), 1000, boost::bind(&YouBotOODLWrapper::armPositionsCommandCallback, this, _1, armIndex));

	topicName.str("");
	topicName << youBotConfiguration.youBotArmConfigurations[armIndex].commandTopicName << "arm_controller/velocity_command";
	youBotConfiguration.youBotArmConfigurations[armIndex].armVelocityCommandSubscriber = node.subscribe<brics_actuator::JointVelocities>(topicName.str(), 1000, boost::bind(&YouBotOODLWrapper::armVelocitiesCommandCallback, this, _1, armIndex));

//	youBotConfiguration.youBotArmConfigurations[armIndex].armJointStatePublisher = node.advertise<sensor_msgs::JointState>("joint_states", 1); //TODO different names or one topic?
	topicName.str("");
	topicName << youBotConfiguration.youBotArmConfigurations[armIndex].commandTopicName << "joint_states";
	youBotConfiguration.youBotArmConfigurations[armIndex].armJointStatePublisher = node.advertise<sensor_msgs::JointState>(topicName.str(), 1); //TODO different names or one topic?

	if(enableStandardGripper) {
		topicName.str("");
		topicName << youBotConfiguration.youBotArmConfigurations[armIndex].commandTopicName << "gripper_controller/position_command";
		youBotConfiguration.youBotArmConfigurations[armIndex].gripperPositionCommandSubscriber = node.subscribe<brics_actuator::JointPositions>(topicName.str(), 1000, boost::bind(&YouBotOODLWrapper::gripperPositionsCommandCallback, this, _1, armIndex));
		youBotConfiguration.youBotArmConfigurations[armIndex].lastGripperCommand = 0.0; //This is true if the gripper is calibrated.
	}

	/* initialize message vector for arm joint states */
	sensor_msgs::JointState dummyMessage;
	armJointStateMessages.push_back(dummyMessage);

	/* setup frame_ids */
	youBotArmFrameID = "arm";  //TODO find default topic name

	ROS_INFO("Arm \"%s\" is initialized.", armName.c_str());
	ROS_INFO("System has %i initialized arm(s).", static_cast<int>(youBotConfiguration.youBotArmConfigurations.size()));
	youBotConfiguration.hasArms = true;
}

void YouBotOODLWrapper::stop() {

	if (youBotConfiguration.hasBase) {
		if (youBotConfiguration.baseConfiguration.youBotBase) {
			delete youBotConfiguration.baseConfiguration.youBotBase;
			youBotConfiguration.baseConfiguration.youBotBase = 0;
		}
		youBotConfiguration.hasBase = false;
	}

	if (youBotConfiguration.hasArms) { //delete each arm
		for (int armIndex = 0; armIndex < static_cast<int>(youBotConfiguration.youBotArmConfigurations.size()); armIndex++) {
			if (youBotConfiguration.youBotArmConfigurations[armIndex].youBotArm) {
				delete youBotConfiguration.youBotArmConfigurations[armIndex].youBotArm;
				youBotConfiguration.youBotArmConfigurations[armIndex].youBotArm = 0;
			}
		}
		youBotConfiguration.hasArms = false;
	}
	armJointStateMessages.clear();

}

void YouBotOODLWrapper::baseCommandCallback(const geometry_msgs::Twist& youbotBaseCommand){

	if (youBotConfiguration.hasBase) { // in case stop has been invoked
		quantity<si::velocity> longitudinalVelocity;
		quantity<si::velocity> transversalVelocity;
		quantity<si::angular_velocity> angularVelocity;

		/*
		 * Frame in OODL:
		 *
		 *		 FRONT
		 *
		 *         X
		 *         ^
		 *         |
		 *         |
		 *         |
		 * Y <-----+
		 *
		 *        BACK
		 *
		 * Positive angular velocity means turning counterclockwise
		 *
		 */

		longitudinalVelocity =  youbotBaseCommand.linear.x * meter_per_second;
		transversalVelocity = youbotBaseCommand.linear.y * meter_per_second;
		angularVelocity = youbotBaseCommand.angular.z * radian_per_second;

		try {
			youBotConfiguration.baseConfiguration.youBotBase->setBaseVelocity(longitudinalVelocity, transversalVelocity, angularVelocity);
		} catch (std::exception& e) {
			std::string errorMessage = e.what();
			ROS_WARN("Cannot set base velocities: \n %s", errorMessage.c_str());
		}

	} else {
		ROS_ERROR("No base initialized!");
	}
}


void YouBotOODLWrapper::armPositionsCommandCallback(const brics_actuator::JointPositionsConstPtr& youbotArmCommand, int armIndex) {
	ROS_DEBUG("Command for arm%i received", armIndex+1);
	ROS_ASSERT(0 <= armIndex && armIndex < static_cast<int>(youBotConfiguration.youBotArmConfigurations.size()));

	if (youBotConfiguration.youBotArmConfigurations[armIndex].youBotArm != 0) { // in case stop has been invoked

		ROS_DEBUG("Arm ID is: %s", youBotConfiguration.youBotArmConfigurations[armIndex].armID.c_str());
		if (youbotArmCommand->positions.size() < 1){
			ROS_WARN("youBot driver received an invalid joint positions command.");
			return;
		}

		youbot::JointAngleSetpoint desiredAngle;
		string unit = boost::units::to_string(boost::units::si::radian);

		/* populate mapping between joint names and values  */
		std::map<string, double> jointNameToValueMapping;
		for (int i = 0; i < static_cast<int>(youbotArmCommand->positions.size()); ++i) {
			if (unit == youbotArmCommand->positions[i].unit) {
				jointNameToValueMapping.insert(make_pair(youbotArmCommand->positions[i].joint_uri, youbotArmCommand->positions[i].value));
			} else {
				ROS_WARN("Unit incompatibility. Are you sure you want to command %s instead of %s ?", youbotArmCommand->positions[i].unit.c_str(), unit.c_str());
			}
		}

		/* loop over all youBot arm joints and check if something is in the received message that requires action */
		ROS_ASSERT(youBotConfiguration.youBotArmConfigurations[armIndex].jointNames.size() == static_cast<unsigned int>(youBotArmDoF));
		youbot::EthercatMaster::getInstance().AutomaticSendOn(false); // ensure that all joint values will be send at the same time
		for (int i = 0; i < youBotArmDoF; ++i) {

			/* check what is in map */
			map<string, double>::const_iterator jointIterator = jointNameToValueMapping.find(youBotConfiguration.youBotArmConfigurations[armIndex].jointNames[i]);
			if (jointIterator != jointNameToValueMapping.end()) {

				/* set the desired joint value */
				ROS_DEBUG("Trying to set joint %s to new position value %f", (youBotConfiguration.youBotArmConfigurations[armIndex].jointNames[i]).c_str(), jointIterator->second);
				desiredAngle.angle = jointIterator->second * radian;
				try {
					youBotConfiguration.youBotArmConfigurations[armIndex].youBotArm->getArmJoint(i + 1).setData(desiredAngle); //youBot joints start with 1 not with 0 -> i + 1
				} catch (std::exception& e) {
					std::string errorMessage = e.what();
					ROS_WARN("Cannot set arm joint %i: \n %s", i+1, errorMessage.c_str());
				}
			}
		}
		youbot::EthercatMaster::getInstance().AutomaticSendOn(true); // ensure that all joint values will be send at the same time
	} else {
		ROS_ERROR("Arm%i is not correctly initialized!", armIndex + 1);
	}

}

void YouBotOODLWrapper::armVelocitiesCommandCallback(const brics_actuator::JointVelocitiesConstPtr& youbotArmCommand,  int armIndex) {
	ROS_DEBUG("Command for arm%i received", armIndex + 1);
	ROS_ASSERT(0 <= armIndex && armIndex < static_cast<int>(youBotConfiguration.youBotArmConfigurations.size()));

	if (youBotConfiguration.youBotArmConfigurations[armIndex].youBotArm != 0) { // in case stop has been invoked

		if (youbotArmCommand->velocities.size() < 1){
			ROS_WARN("youBot driver received an invalid joint velocities command.");
			return;
		}

		youbot::JointVelocitySetpoint desiredAngularVelocity;
		string unit = boost::units::to_string(boost::units::si::radian_per_second);

		/* populate mapping between joint names and values  */
		std::map<string, double> jointNameToValueMapping;
		for (int i = 0; i < static_cast<int>(youbotArmCommand->velocities.size()); ++i) {
			if (unit == youbotArmCommand->velocities[i].unit) {
				jointNameToValueMapping.insert(make_pair(youbotArmCommand->velocities[i].joint_uri, youbotArmCommand->velocities[i].value));
			} else {
				ROS_WARN("Unit incompatibility. Are you sure you want to command %s instead of %s ?", youbotArmCommand->velocities[i].unit.c_str(), unit.c_str());
			}

		}

		/* loop over all youBot arm joints and check if something is in the received message that requires action */
		ROS_ASSERT(youBotConfiguration.youBotArmConfigurations[armIndex].jointNames.size() == static_cast<unsigned int>(youBotArmDoF));
		youbot::EthercatMaster::getInstance().AutomaticSendOn(false); // ensure that all joint values will be send at the same time
		for (int i = 0; i < youBotArmDoF; ++i) {

			/* check what is in map */
			map<string, double>::const_iterator jointIterator = jointNameToValueMapping.find(youBotConfiguration.youBotArmConfigurations[armIndex].jointNames[i]);
			if (jointIterator != jointNameToValueMapping.end()) {

				/* set the desired joint value */
				ROS_DEBUG("Trying to set joint %s to new velocity value %f", (youBotConfiguration.youBotArmConfigurations[armIndex].jointNames[i]).c_str(), jointIterator->second);
				desiredAngularVelocity.angularVelocity = jointIterator->second * radian_per_second;
				try {
					youBotConfiguration.youBotArmConfigurations[0].youBotArm->getArmJoint(i + 1).setData(desiredAngularVelocity); //youBot joints start with 1 not with 0 -> i + 1
				} catch (std::exception& e) {
					std::string errorMessage = e.what();
					ROS_WARN("Cannot set arm joint %i: \n %s", i+1, errorMessage.c_str());
				}
			}
		}
		youbot::EthercatMaster::getInstance().AutomaticSendOn(true); // ensure that all joint values will be send at the same time
	} else {
		ROS_ERROR("Arm%i is not correctly initialized!", armIndex + 1);
	}
}

void YouBotOODLWrapper::gripperPositionsCommandCallback(const brics_actuator::JointPositionsConstPtr& youbotGripperCommand, int armIndex){
	ROS_DEBUG("Command for gripper%i received", armIndex + 1);
	ROS_ASSERT(0 <= armIndex && armIndex < static_cast<int>(youBotConfiguration.youBotArmConfigurations.size()));

	if (youBotConfiguration.youBotArmConfigurations[armIndex].youBotArm != 0) { // in case stop has been invoked

		if (youbotGripperCommand->positions.size() < 1){
			ROS_WARN("youBot driver received an invalid gripper positions command.");
			return;
		}
		youbot::GripperBarSpacingSetPoint gripperSlideRailDistance;
		string unit = boost::units::to_string(boost::units::si::meter);
		gripperSlideRailDistance.barSpacing = 0 * meter;
		bool validGripperCommandReceived = false;

		/* populate mapping between joint names and values */
		std::map<string, double> jointNameToValueMapping;
		for (int i = 0; i < static_cast<int>(youbotGripperCommand->positions.size()); ++i) {
			if (unit == youbotGripperCommand->positions[i].unit) {
				jointNameToValueMapping.insert(make_pair(youbotGripperCommand->positions[i].joint_uri, youbotGripperCommand->positions[i].value));
			} else {
				ROS_WARN("Unit incompatibility. Are you sure you want to command %s instead of %s ?", youbotGripperCommand->positions[i].unit.c_str(), unit.c_str());
			}
		}

		/* loop over all youBot gripper joints and check if something is in the received message that requires action */
		ROS_ASSERT(youBotConfiguration.youBotArmConfigurations[armIndex].gripperFingerNames.size() == static_cast<unsigned int>(youBotNumberOfFingers));
		youbot::EthercatMaster::getInstance().AutomaticSendOn(false); // ensure that all joint values will be send at the same time
		for (int i = 0; i < youBotNumberOfFingers; ++i) {

			/* check if there is something in in the message for the gripper */
			map<string, double>::const_iterator gripperIterator = jointNameToValueMapping.find(youBotConfiguration.youBotArmConfigurations[armIndex].gripperFingerNames[i]);
			if (gripperIterator != jointNameToValueMapping.end()) {
				ROS_DEBUG("Trying to set the gripper to new value %f", gripperIterator->second);

				gripperSlideRailDistance.barSpacing += gripperIterator->second * meter; //for now just stack all values, as long youBot oodl API does not suport two fingers
				validGripperCommandReceived = true;
			}
		}
		if (validGripperCommandReceived) { // at least one valid command received that requires action (set accumulated gripper value)
			try {
				youBotConfiguration.youBotArmConfigurations[0].youBotArm->getArmGripper().setData(gripperSlideRailDistance);
				youBotConfiguration.youBotArmConfigurations[armIndex].lastGripperCommand = gripperSlideRailDistance.barSpacing.value();
			} catch (std::exception& e) {
				std::string errorMessage = e.what();
				ROS_WARN("Cannot set the gripper: \n %s", errorMessage.c_str());
			}
		}
		youbot::EthercatMaster::getInstance().AutomaticSendOn(false); // ensure that all joint values will be send at the same time
	} else {
		ROS_ERROR("Arm%i is not correctly initialized!", armIndex + 1);
	}
}

void YouBotOODLWrapper::computeOODLSensorReadings() {

	currentTime = ros::Time::now();
    youbot::JointSensedAngle currentAngle;
    youbot::JointSensedVelocity currentVelocity;

	youbot::EthercatMaster::getInstance().AutomaticReceiveOn(false); // ensure that all joint values will be send at the same time

	if (youBotConfiguration.hasBase == true) {
		double x = 0.0;
		double y = 0.0;
		double theta = 0.0;

		double vx = 0.0;
		double vy = 0.0;
		double vtheta = 0.0;

		quantity<si::length> longitudinalPosition;
	    quantity<si::length> transversalPosition;
	    quantity<plane_angle> orientation;

	    quantity<si::velocity> longitudinalVelocity;
	    quantity<si::velocity> transversalVelocity;
	    quantity<si::angular_velocity> angularVelocity;

	    youBotConfiguration.baseConfiguration.youBotBase->getBasePosition(longitudinalPosition, transversalPosition, orientation);
		x = longitudinalPosition.value();
		y = transversalPosition.value();
		theta = orientation.value();

		youBotConfiguration.baseConfiguration.youBotBase->getBaseVelocity(longitudinalVelocity, transversalVelocity, angularVelocity);
		vx = longitudinalVelocity.value();
		vy = transversalVelocity.value();
		vtheta = angularVelocity.value();
		//ROS_DEBUG("Perceived odometric values (x,y,tetha, vx,vy,vtetha): %f, %f, %f \t %f, %f, %f", x, y, theta, vx, vy, vtheta);


		/* Setup odometry tf frame */
		odometryQuaternion = tf::createQuaternionMsgFromYaw(theta);

		odometryTransform.header.stamp = currentTime;
		odometryTransform.header.frame_id = youBotOdometryFrameID;
		odometryTransform.child_frame_id = youBotOdometryChildFrameID;

		odometryTransform.transform.translation.x = x;
		odometryTransform.transform.translation.y = y;
		odometryTransform.transform.translation.z = 0.0;
		odometryTransform.transform.rotation = odometryQuaternion;

		/* Setup odometry Message */
		odometryMessage.header.stamp = currentTime;
		odometryMessage.header.frame_id = youBotOdometryFrameID;

		odometryMessage.pose.pose.position.x = x;
		odometryMessage.pose.pose.position.y = y;
		odometryMessage.pose.pose.position.z = 0.0;
		odometryMessage.pose.pose.orientation = odometryQuaternion;

		odometryMessage.child_frame_id = youBotOdometryChildFrameID;
//		odometryMessage.child_frame_id = youBotOdometryFrameID;
		odometryMessage.twist.twist.linear.x =  vx;
		odometryMessage.twist.twist.linear.y = vy;
		odometryMessage.twist.twist.angular.z = vtheta;

		for (int i = 0; i < 36; i++) odometryMessage.pose.covariance[i] = 0.0;
		odometryMessage.pose.covariance[0] = 1e-3;
		odometryMessage.pose.covariance[7] = 1e-3;
		odometryMessage.pose.covariance[14] = 1e6;
		odometryMessage.pose.covariance[21] = 1e6;
		odometryMessage.pose.covariance[28] = 1e6;
		odometryMessage.pose.covariance[35] = 1e3;

		for (int i = 0; i < 36; i++) odometryMessage.twist.covariance[i] = 0.0;
		odometryMessage.twist.covariance[0] = 1e-3;
		odometryMessage.twist.covariance[7] = 1e-3;
		odometryMessage.twist.covariance[14] = 1e6;
		odometryMessage.twist.covariance[21] = 1e6;
		odometryMessage.twist.covariance[28] = 1e6;
		odometryMessage.twist.covariance[35] = 1e3;

		/* Set up joint state message for the wheels */
		baseJointStateMessage.header.stamp = currentTime;
		baseJointStateMessage.name.resize(youBotNumberOfWheels * 2); // *2 because of virtual wheel joints in the URDF description
		baseJointStateMessage.position.resize(youBotNumberOfWheels * 2);
		baseJointStateMessage.velocity.resize(youBotNumberOfWheels * 2);

		ROS_ASSERT((youBotConfiguration.baseConfiguration.wheelNames.size() == static_cast<unsigned int>(youBotNumberOfWheels)));
		for (int i = 0; i < youBotNumberOfWheels; ++i) {
			youBotConfiguration.baseConfiguration.youBotBase->getBaseJoint(i + 1).getData(currentAngle); //youBot joints start with 1 not with 0 -> i + 1
			youBotConfiguration.baseConfiguration.youBotBase->getBaseJoint(i + 1).getData(currentVelocity);

			baseJointStateMessage.name[i] = youBotConfiguration.baseConfiguration.wheelNames[i];
			baseJointStateMessage.position[i] = currentAngle.angle.value();
			baseJointStateMessage.velocity[i] = currentVelocity.angularVelocity.value();
		}

		/*
		 * Here we add values for "virtual" rotation joints in URDF - robot_state_publisher can't
		 * handle non-aggregated jointState messages well ...
		 */
		baseJointStateMessage.name[4] = "caster_joint_fl";
		baseJointStateMessage.position[4] = 0.0;

		baseJointStateMessage.name[5] = "caster_joint_fr";
		baseJointStateMessage.position[5] = 0.0;

		baseJointStateMessage.name[6] = "caster_joint_bl";
		baseJointStateMessage.position[6] = 0.0;

		baseJointStateMessage.name[7] = "caster_joint_br";
		baseJointStateMessage.position[7] = 0.0;

		/*
		 * Yet another hack to make the published values compatible with the URDF description.
		 * We actually flipp the directions of the wheel on the right side such that the standard ROS controllers
		 * (e.g. for PR2) can be used for the youBot
		 */
		baseJointStateMessage.position[2] = -baseJointStateMessage.position[2];
		baseJointStateMessage.position[4] = -baseJointStateMessage.position[4];

	}

	if (youBotConfiguration.hasArms == true) {

//		int numberOfArms = static_cast<int>(youBotConfiguration.youBotArmConfigurations.size());
//
//		/* fill joint state message */
//		jointStateMessage.header.stamp = currentTime;
//		jointStateMessage.name.resize(numberOfArms*(youBotArmDoF + youBotNumberOfFingers));
//		jointStateMessage.position.resize(numberOfArms*(youBotArmDoF + youBotNumberOfFingers));
//		jointStateMessage.velocity.resize(numberOfArms*(youBotArmDoF + youBotNumberOfFingers));
//
//		for (int armIndex = 0; armIndex < numberOfArms; armIndex++) {
//			if(youBotConfiguration.youBotArmConfigurations[armIndex].youBotArm == 0) {
//				ROS_ERROR("Arm%i is not correctly initialized! Cannot publish data.", armIndex + 1);
//				continue;
//			}
//
//			ROS_ASSERT(youBotConfiguration.youBotArmConfigurations[armIndex].jointNames.size() == static_cast<unsigned int>(youBotArmDoF));
//			for (int i = 0; i < youBotArmDoF; ++i) {
//				youBotConfiguration.youBotArmConfigurations[armIndex].youBotArm->getArmJoint(i + 1).getData(currentAngle); //youBot joints start with 1 not with 0 -> i + 1
//				youBotConfiguration.youBotArmConfigurations[armIndex].youBotArm->getArmJoint(i + 1).getData(currentVelocity);
//
//				jointStateMessage.name[armIndex*(youBotArmDoF + youBotNumberOfFingers) + i] = youBotConfiguration.youBotArmConfigurations[armIndex].jointNames[i]; // no unique nemes for URDF yet
//				jointStateMessage.position[armIndex*(youBotArmDoF + youBotNumberOfFingers) + i] = currentAngle.angle.value();
//				jointStateMessage.velocity[armIndex*(youBotArmDoF + youBotNumberOfFingers) + i] = currentVelocity.angularVelocity.value();
//			}
//
//			/*
//			 * NOTE: gripper slide rails are always symmetric, but the fingers can be screwed in different
//			 * positions! The published values account for the distance between the gripper slide rails, not the fingers
//			 * themselves. Of course if the finger are screwed to the most inner position (i.e. the can close completely),
//			 * than it is correct.
//			 */
//			youbot::GripperBarSpacingSetPoint gripperSlideRailDistance;
//			//		youBotArm->getArmGripper().getData(gripperSlideRailDistance); // this is not yet implemented in OODL
//			//		double distance = gripperSlideRailDistance.barSpacing.value();
//
//			ROS_ASSERT(gripperFingerNames.size() == static_cast<unsigned int>(youBotNumberOfFingers));
//			for (int i = 0; i < youBotNumberOfFingers; ++i) {
//
//				jointStateMessage.name[armIndex*(youBotArmDoF + youBotNumberOfFingers) + youBotArmDoF + 1] = gripperFingerNames[i];
//				jointStateMessage.position[armIndex*(youBotArmDoF + youBotNumberOfFingers) + youBotArmDoF + 1] = lastGripperCommand / 2; //as the distance is symmetric, each finger travels half of the distance
//			}
//		}


		for (int armIndex = 0; armIndex < static_cast<int>(youBotConfiguration.youBotArmConfigurations.size()); armIndex++) {
			ROS_ASSERT(youBotConfiguration.youBotArmConfigurations.size() == armJointStateMessages.size());

			/* fill joint state message */
			armJointStateMessages[armIndex].header.stamp = currentTime;
			armJointStateMessages[armIndex].name.resize(youBotArmDoF + youBotNumberOfFingers);
			armJointStateMessages[armIndex].position.resize(youBotArmDoF + youBotNumberOfFingers);
			armJointStateMessages[armIndex].velocity.resize(youBotArmDoF + youBotNumberOfFingers);

			if(youBotConfiguration.youBotArmConfigurations[armIndex].youBotArm == 0) {
				ROS_ERROR("Arm%i is not correctly initialized! Cannot publish data.", armIndex + 1);
				continue;
			}

			ROS_ASSERT(youBotConfiguration.youBotArmConfigurations[armIndex].jointNames.size() == static_cast<unsigned int>(youBotArmDoF));
			for (int i = 0; i < youBotArmDoF; ++i) {
				youBotConfiguration.youBotArmConfigurations[armIndex].youBotArm->getArmJoint(i + 1).getData(currentAngle); //youBot joints start with 1 not with 0 -> i + 1 //FIXME might segfault if only 1eout of 2 arms are initialized.
				youBotConfiguration.youBotArmConfigurations[armIndex].youBotArm->getArmJoint(i + 1).getData(currentVelocity);

				armJointStateMessages[armIndex].name[i] = youBotConfiguration.youBotArmConfigurations[armIndex].jointNames[i]; //TODO no unique names for URDF yet
				armJointStateMessages[armIndex].position[i] = currentAngle.angle.value();
				armJointStateMessages[armIndex].velocity[i] = currentVelocity.angularVelocity.value();

			}

			/*
			 * NOTE: gripper slide rails are always symmetric, but the fingers can be screwed in different
			 * positions! The published values account for the distance between the gripper slide rails, not the fingers
			 * themselves. Of course if the finger are screwed to the most inner position (i.e. the can close completely),
			 * than it is correct.
			 */
			youbot::GripperBarSpacingSetPoint gripperSlideRailDistance;
			//		youBotArm->getArmGripper().getData(gripperSlideRailDistance); // this is not yet implemented in OODL
			//		double distance = gripperSlideRailDistance.barSpacing.value();

			ROS_ASSERT(youBotConfiguration.youBotArmConfigurations[armIndex].gripperFingerNames.size() == static_cast<unsigned int>(youBotNumberOfFingers));
			for (int i = 0; i < youBotNumberOfFingers; ++i) {

				armJointStateMessages[armIndex].name[youBotArmDoF + i] = youBotConfiguration.youBotArmConfigurations[armIndex].gripperFingerNames[i];
				armJointStateMessages[armIndex].position[youBotArmDoF + i] = youBotConfiguration.youBotArmConfigurations[armIndex].lastGripperCommand / 2; //as the distance is symmetric, each finger travels half of the distance
			}
		}
	}

	youbot::EthercatMaster::getInstance().AutomaticReceiveOn(true); // ensure that all joint values will be send at the same time

}

void YouBotOODLWrapper::publishOODLSensorReadings() {

	if (youBotConfiguration.hasBase) {
		youBotConfiguration.baseConfiguration.odometryBroadcaster.sendTransform(odometryTransform);
		youBotConfiguration.baseConfiguration.baseOdometryPublisher.publish(odometryMessage);
		youBotConfiguration.baseConfiguration.baseJointStatePublisher.publish(baseJointStateMessage);
	}

	if (youBotConfiguration.hasArms) {
		for (int armIndex = 0; armIndex < static_cast<int>(youBotConfiguration.youBotArmConfigurations.size()); armIndex++) {
			youBotConfiguration.youBotArmConfigurations[armIndex].armJointStatePublisher.publish(armJointStateMessages[armIndex]);
		}
	}


}

}  // namespace youBot

/* EOF */
