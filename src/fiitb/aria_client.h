//////////////////////////////////////////////////////////////////////////////////////////////
//
//  Copyright Christian W. Frey 2002
//
//  aria_client.h
//
//  Version 1.0 / 03.01.2007
//
//  Pioneer Robot Controller 
//   

#ifndef ARIA_CLIENT_DEFINED
#define ARIA_CLIENT_DEFINED ARIA_CLIENT_DEFINED


//#define MATZL_MEX

#include <math.h>
#include "Aria.h"
#include "ariaUtil.h"
#include "ArNetworking.h"
#include "matrixt.h"

#ifdef MATZL_MEX
	#include "mex.h"
	#include "mxgtway.h"
#endif

struct robSensorsStruct
		{
			char *Name[256];
			char NameRead[256];
			CMatrixT<double> *Data[256];
			int Number;
		};


class RobotController 
{
    public:
		RobotController(char *IPString);
		virtual ~RobotController(void);
		int robotIsConnected;
		int robot_GotoGoal(int goal_number);
		int robot_GotoHome(void);
		int robot_Wander(void);
		int robot_TourGoals(void);
		int robot_GotoPose(const CMatrixT<double> &pose);
		int robot_LocalizeToPose(const CMatrixT<double> &pose);
		int robot_SetCameraAbs(const CMatrixT<double> &pose);
		int robot_SetCameraRel(const CMatrixT<double> &pose);
		int robot_SetSafeDrive(int safedrive);
		int robot_GetSensorCurrent(int sensorNumber);
		int robot_Stop(void);
		int robot_useGyro(int gyro);
		int robot_MicroControllerCommand(char *commandString);
		
		#ifdef MATZL_MEX
			mxArray* robot_Data2Matlab();
			mxArray* robot_Map2Matlab();
			mxArray* robot_Sensors2Matlab();
			void setupMatlab();
		#endif

		CMatrixT<double> getRobPose(void){return robPose;};
		robSensorsStruct getSensors(void){return robSensors;};
		double getRobVoltage(void){return batteryInfo.val[0][0];};

	protected:		
		struct
		{
			char Name[256];
			char *Strings[1024];
			int numLines;
			int numPoints;
			int numStrings;
			CMatrixT<double> Points;
			CMatrixT<double> Lines;
			int numLinesRead;
			int numPointsRead;
			int isComplete;
		} robMap;
		

		struct robSensorsStruct robSensors;

		char *robIPAdress;
		char robMode[32];
		char robStatus[256];
		char *navGoalList[256];
		
		char navPathPlannerStatus[64];
		CMatrixT<double> robPose;
		CMatrixT<double> robVel;
		double robVoltage;
		double locScore;
		int locState;
		int safeDrive;
		int navPathPoints;
		int navGoalNumber;
		CMatrixT<double> navPath;
		CMatrixT<double> cameraStatus;
		CMatrixT<double> cameraInfo;
		CMatrixT<double> batteryInfo;
		CMatrixT<double> gpsInfo;
		CMatrixT<double> compassInfo;
				
		ArClientBase *robotClient;
		ArArgumentParser *robotParser;
		ArClientSimpleConnector *robotConnector;
		
		void handleReceivePacket(ArNetPacket *packet);
		void handleReceivePath(ArNetPacket *packet);
		void handleReceivePathPlannerStatus(ArNetPacket *packet);
		void handleReceiveLocalisation(ArNetPacket *packet);
		void handleReceiveGoalList(ArNetPacket *packet);
		void handleReceiveBatteryInfo(ArNetPacket *packet);
		void handleReceiveCameraStatus(ArNetPacket *packet);
		void handleReceiveCameraInfo(ArNetPacket *packet);
		void handleReceiveMapName(ArNetPacket *packet);
		void handleReceiveMapBinary(ArNetPacket *packet);
		void handleReceiveSafeDrive(ArNetPacket *packet);
		void handleReceiveSensorList(ArNetPacket *packet);
		void handleReceiveSensorCurrent(ArNetPacket *packet);
		void handleReceiveUpdateGPS(ArNetPacket *packet);
		void handleReceiveUpdateCompass(ArNetPacket *packet);

		ArFunctor1C<RobotController, ArNetPacket *> handleReceivePacketCB;
		ArFunctor1C<RobotController, ArNetPacket *> handleReceivePathCB;
		ArFunctor1C<RobotController, ArNetPacket *> handleReceivePathPlannerStatusCB;
		ArFunctor1C<RobotController, ArNetPacket *> handleReceiveLocalisationCB;
		ArFunctor1C<RobotController, ArNetPacket *> handleReceiveGoalListCB;
		ArFunctor1C<RobotController, ArNetPacket *> handleReceiveBatteryInfoCB;
		ArFunctor1C<RobotController, ArNetPacket *> handleReceiveCameraInfoCB;
		ArFunctor1C<RobotController, ArNetPacket *> handleReceiveCameraStatusCB;
		ArFunctor1C<RobotController, ArNetPacket *> handleReceiveMapNameCB;
		ArFunctor1C<RobotController, ArNetPacket *> handleReceiveMapBinaryCB;
		ArFunctor1C<RobotController, ArNetPacket *> handleReceiveSafeDriveCB;
		ArFunctor1C<RobotController, ArNetPacket *> handleReceiveSensorListCB;
		ArFunctor1C<RobotController, ArNetPacket *> handleReceiveSensorCurrentCB;
		ArFunctor1C<RobotController, ArNetPacket *> handleReceiveUpdateGPSCB;
		ArFunctor1C<RobotController, ArNetPacket *> handleReceiveUpdateCompassCB;
		bool printDeveloperMessage;
		bool isConnected;

	    
};

#endif

