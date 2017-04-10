//////////////////////////////////////////////////////////////////////////////////////////////
//
//  Copyright Christian W. Frey 2002
//
//  aria_client.cpp
//
//  Version 1.0 / 03.01.2007
//
//  Pioneer Robot Controller 
//   

//#include <math.h>
//#include "Aria.h"
//#include "ariaUtil.h"
//#include "ArNetworking.h"
#include "aria_client.h"
//#include "matrixt.h"

//#define MATZL_MEX
#ifdef MATZL_MEX
    #include "mex.h"
	#include "mxgtway.h"
    //extern void _main();
#endif

RobotController::RobotController(char *IPString) :
	  handleReceivePacketCB(this, &RobotController::handleReceivePacket),
	  handleReceivePathCB(this,&RobotController::handleReceivePath),
	  handleReceivePathPlannerStatusCB(this,&RobotController::handleReceivePathPlannerStatus),
	  handleReceiveLocalisationCB(this,&RobotController::handleReceiveLocalisation),
	  handleReceiveGoalListCB(this,&RobotController::handleReceiveGoalList),
	  handleReceiveBatteryInfoCB(this, &RobotController::handleReceiveBatteryInfo),
	  handleReceiveCameraInfoCB(this, &RobotController::handleReceiveCameraInfo),
	  handleReceiveCameraStatusCB(this, &RobotController::handleReceiveCameraStatus),
	  handleReceiveMapNameCB(this, &RobotController::handleReceiveMapName),
	  handleReceiveMapBinaryCB(this, &RobotController::handleReceiveMapBinary),
	  handleReceiveSafeDriveCB(this, &RobotController::handleReceiveSafeDrive),
	  handleReceiveSensorListCB(this, &RobotController::handleReceiveSensorList),
	  handleReceiveSensorCurrentCB(this, &RobotController::handleReceiveSensorCurrent),
	  handleReceiveUpdateGPSCB(this, &RobotController::handleReceiveUpdateGPS),
	  handleReceiveUpdateCompassCB(this, &RobotController::handleReceiveUpdateCompass),
	  printDeveloperMessage(true),
	  navGoalNumber(0)
{
	Aria::init();
	int argc=0;
	
	robIPAdress = new char[64];
	if(strstr(IPString,"-host "))
		strcpy(robIPAdress,IPString);
	else
	{
		strcpy(robIPAdress,"-host ");
		strcpy(&(robIPAdress[6]),IPString);
	}
	
	robotClient = new ArClientBase;
	robotParser = new ArArgumentParser(&argc,NULL);
	robotParser->addDefaultArgument(robIPAdress);
	robotConnector = new ArClientSimpleConnector(robotParser);
	robotParser->loadDefaultArguments();
	robotConnector->parseArgs();
		
	if (!robotConnector->connectClient(robotClient))
	{
		isConnected=false;
		if (robotClient->wasRejected())
		{
			#ifdef MATZL_MEX
				mexPrintf("***************************************************************\n");
				mexPrintf("** MATLAB - ARIA Gateway\n");
				mexPrintf("** SERVER REJECTED ROBOT CONNECTION TO: ");mexPrintf(robIPAdress);mexPrintf("\n");
				mexPrintf("***************************************************************\n");
			#else
				printf("***************************************************************\n");
				printf("** SERVER REJECTED ROBOT CONNECTION TO: ");printf(robIPAdress);printf("\n");
				printf("***************************************************************\n");
			#endif
		}		  
		else
		{
			#ifdef MATZL_MEX
				mexPrintf("***************************************************************\n");
				mexPrintf("** MATLAB - ARIA Gateway\n");
				mexPrintf("** NO SERVER ROBOT CONNECTION TO: ");mexPrintf(robIPAdress);mexPrintf("\n");
				mexPrintf("***************************************************************\n");
			#else
				printf("***************************************************************\n");
				printf("** NO SERVER ROBOT CONNECTION TO: ");printf(robIPAdress);printf("\n");
				printf("***************************************************************\n");
			#endif
		}
		delete robotConnector;
		delete robotParser;
		//delete robotClient;
		robotIsConnected=0;
		
	}
	else
	{
		//////////////////////////
		// Setup Vars
		robPose.matrixSetsize(1,3,0.0);
		robVel.matrixSetsize(1,2,0.0);
		cameraStatus.matrixSetsize(1,3,-1.0);
		cameraInfo.matrixSetsize(1,7,-1.0);
		batteryInfo.matrixSetsize(1,3,-1.0);
		gpsInfo.matrixSetsize(1,15,-1.0);
		compassInfo.matrixSetsize(1,5,-1.0);
		navGoalNumber=0;
		robSensors.Number=0;
		robMap.isComplete=0;
		
		
	

		robotIsConnected=1;
		robotClient->addHandler("batteryInfo", &handleReceiveBatteryInfoCB);
		
		robotClient->addHandler("update", &handleReceivePacketCB);
		robotClient->addHandler("getLocState",&handleReceiveLocalisationCB);
		robotClient->addHandler("getPath",&handleReceivePathCB);
		robotClient->addHandler("pathPlannerStatus", &handleReceivePathPlannerStatusCB);
		robotClient->addHandler("getGoals", &handleReceiveGoalListCB);
		robotClient->addHandler("cameraUpdate", &handleReceiveCameraStatusCB);
		robotClient->addHandler("cameraInfo", &handleReceiveCameraInfoCB);
		robotClient->addHandler("getMapName", &handleReceiveMapNameCB);
		robotClient->addHandler("getMapBinary", &handleReceiveMapBinaryCB);
		robotClient->addHandler("getSafeDrive", &handleReceiveSafeDriveCB);
		robotClient->addHandler("getSensorList", &handleReceiveSensorListCB);
		robotClient->addHandler("getSensorCurrent", &handleReceiveSensorCurrentCB);
		robotClient->addHandler("updateGPS", &handleReceiveUpdateGPSCB);
		robotClient->addHandler("updateCompass", &handleReceiveUpdateCompassCB);

		robotClient->runAsync();
		robotClient->requestOnce("getSensorList");
		robotClient->requestOnce("getGoals");
		robotClient->requestOnce("batteryInfo");
		robotClient->requestOnce("cameraInfo");
		robotClient->requestOnce("getMapName");
		robotClient->requestOnce("getMapBinary");
		
		robotClient->request("update", 100);
		robotClient->request("getPath", 100);
		robotClient->request("pathPlannerStatus", 100);
		robotClient->request("getLocState", 100);
		robotClient->request("cameraUpdate", 100);
		robotClient->request("getSafeDrive", 100);
		robotClient->request("updateGPS", 1000);
		robotClient->request("updateCompass", 1000);

		
		printf("***************************************************************\n");
		printf("** ARIA Gateway\n");
		printf("** Fraunhofer Institute for Information and Dataprocessing IITB\n");
		printf("** Fraunhofer Str. 1, 76131 Karlsruhe Germany\n");
		printf("** \n");
		printf("** 2008 Christian W. Frey, Thomas Emter\n");
		printf("** \n");
		printf("** ROBOT CONNECTION TO: ");printf(robIPAdress);printf("\n");
		printf("** THIS IS NO FREEWARE / ONLY FOR EVALUATION PURPOSES\n");
		printf("***************************************************************\n");
	
		isConnected=true;

		#ifdef MATZL_MEX
			setupMatlab();
		#endif
		
		
		
	}
	
}

RobotController::~RobotController(void)
{
	if(isConnected)
	{
		/* Halt the request for data updates */
		robotClient->requestStop("update");
		robotClient->requestStop("getPath");
		robotClient->requestStop("getLocState");
		robotClient->requestStop("pathPlannerStatus");
		robotClient->requestStop("cameraUpdate");
		robotClient->requestStop("getSafeDrive");
		robotClient->requestStop("updateGPS");
		robotClient->requestStop("updateCompass");

		#ifdef MATZL_MEX
			mexPrintf("***************************************************************\n");
			mexPrintf("** MATLAB - ARIA Gateway\n");
			mexPrintf("** CLOSING ROBOT CONNECTION TO: ");mexPrintf(robIPAdress);mexPrintf("\n");
			mexPrintf("***************************************************************\n");
		#else
			printf("***************************************************************\n");
			printf("** CLOSING ROBOT CONNECTION TO: ");printf(robIPAdress);printf("\n");
			printf("***************************************************************\n");
		#endif

		robotClient->stopRunning();
		
		//////////////////////////////////////////////
		// Clean
		int i;
		if(navGoalNumber>0)
			for(i=0;i<navGoalNumber;i++)
				if(navGoalList[i]!=NULL)
					delete [] navGoalList[i];

		if(robSensors.Number>0)
			for(i=0;i<robSensors.Number;i++)
			{
				if(robSensors.Name[i]!=NULL)
					delete [] robSensors.Name[i];
				if(robSensors.Data[i]!=NULL)
					delete robSensors.Data[i];
			}

		if (robMap.numStrings>0)
		{
			for (i=0;i<robMap.numStrings;i++)
			{
				if(robMap.Strings[i]!=NULL)
					delete [] robMap.Strings[i];
			}
		}
		
		delete robotConnector;
		delete robotParser;
		//delete robotClient;
	}
	delete [] robIPAdress;
	
}

void RobotController::handleReceiveGoalList(ArNetPacket *packet)
{
	int i;
	for(i=0;i<navGoalNumber;i++)
		if(navGoalList[i]!=NULL)
			delete [] navGoalList[i];

	navGoalNumber=0;
	for(i = 0; packet->getReadLength() < packet->getLength(); i++)
	{
		navGoalList[i]=new char[256];
		packet->bufToStr(navGoalList[i], 255);
		if(strlen(navGoalList[i]) == 0)
		{
			delete navGoalList[i];
			return;
		}
		navGoalNumber++;
	}
}

void RobotController::handleReceiveUpdateGPS(ArNetPacket *packet)
{
	gpsInfo.val[0][0]=(double)packet->bufToDouble();			//Latitude
	gpsInfo.val[0][1]=(double)packet->bufToDouble();			//Longitude
	gpsInfo.val[0][2]=(double)packet->bufToDouble();			//Speed
	gpsInfo.val[0][3]=(double)packet->bufToUByte4();			//GPSPostionTimeStamp
	gpsInfo.val[0][4]=(double)packet->bufToUByte2();			//GPSNumSat
	gpsInfo.val[0][5]=(double)packet->bufToUByte2();			//haveDGPSStation
	gpsInfo.val[0][6]=(double)packet->bufToDouble();			//GPSVerticalPositionError
	gpsInfo.val[0][7]=(double)packet->bufToDouble();			//GPSHDOP
	gpsInfo.val[0][8]=(double)packet->bufToDouble();			//GPSVDOP
	gpsInfo.val[0][9]=(double)packet->bufToDouble();			//GPSPDOP
	gpsInfo.val[0][10]=(double)packet->bufToDouble();			//GPSAltitude
	gpsInfo.val[0][11]=(double)packet->bufToUByte2();			//DGPSQuali
	gpsInfo.val[0][12]=(double)packet->bufToUByte2();			//DGPSNumSat
	gpsInfo.val[0][13]=(double)packet->bufToUByte4();			//DGPSTimestamp
	gpsInfo.val[0][14]=(double)packet->bufToUByte2();			//GPSMode 7:unknown; 1:GPS; 2:DGPS; 6:estimated; 0:not valid; 7:simulator 
}

void RobotController::handleReceiveUpdateCompass(ArNetPacket *packet)
{
	compassInfo.val[0][0]=(double)packet->bufToDouble(); //yaw
	compassInfo.val[0][1]=(double)packet->bufToDouble(); //pitch
	compassInfo.val[0][2]=(double)packet->bufToDouble(); //roll
	compassInfo.val[0][3]=(double)packet->bufToDouble(); //temperature
	compassInfo.val[0][4]=(double)packet->bufToDouble(); //error
}

void RobotController::handleReceiveSensorCurrent(ArNetPacket *packet)
{
	//printf("sensor\n");
	int i;
	memset(robSensors.NameRead, 0, sizeof(robSensors.NameRead));

	int numData=(int)packet->bufToByte2();
	int numSensor=-1;
	packet->bufToStr(robSensors.NameRead, 255);
	for(i=0;i<robSensors.Number;i++)
		if(strstr(robSensors.Name[i],robSensors.NameRead))
		{
			numSensor=i;
		}

	if((numSensor<robSensors.Number)&&(numSensor>-1))
	{
		if ((numData>0)&&(numData<512))
		{
			robSensors.Data[numSensor]->matrixSetsize(2,numData);
			for(i=0;i<numData;i++)
			{
				robSensors.Data[numSensor]->val[0][i]=(double)packet->bufToByte4();
				robSensors.Data[numSensor]->val[1][i]=(double)packet->bufToByte4();
			}
		}
		else
		{
			robSensors.Data[numSensor]->matrixSetsize(0,0);
		}
	}
}

int RobotController::robot_GetSensorCurrent(int sensorNumber)
{
	if ((sensorNumber>-1)&&(sensorNumber<robSensors.Number))
	{
		robotClient->requestOnceWithString("getSensorCurrent",(robSensors.Name[sensorNumber]));
		return robotIsConnected;
	}
	else
		return -1;
}

void RobotController::handleReceiveSensorList(ArNetPacket *packet)
{
	int i;
	
	if(robSensors.Number>0)
		for(i=0;i<robSensors.Number;i++)
		{
			if(robSensors.Name[i]!=NULL)
				delete [] robSensors.Name[i];
			if(robSensors.Data[i]!=NULL)
				delete robSensors.Data[i];
		}

	robSensors.Number=0;
	robSensors.Number=(int)packet->bufToByte2();
	for(i = 0; i<robSensors.Number; i++)
	{
		robSensors.Name[i]=new char[256];
		packet->bufToStr(robSensors.Name[i], 255);
		if(strlen(robSensors.Name[i]) == 0)
		{
			delete [] robSensors.Name[i];
			return;
		}
		robSensors.Data[i] = new CMatrixT<double>;
	}
}

void RobotController::handleReceiveMapBinary(ArNetPacket *packet)
{
	
	int i;
	switch(robMap.isComplete)
	{
		case 0:
			{
				if (robMap.numStrings>0)
				{
					for (i=0;i<robMap.numStrings;i++)
					{
						if(robMap.Strings[i]!=NULL)
							delete [] robMap.Strings[i];
					}
				}
				robMap.numLines=0;
				robMap.numPoints=0;
				robMap.numStrings=0;
				robMap.numLinesRead=0;
				robMap.numPointsRead=0;

				
				robMap.isComplete=1;
				
			}
		case 1:
			{
				robMap.Strings[robMap.numStrings]=new char[256];
				packet->bufToStr(robMap.Strings[robMap.numStrings], 255);
				if (strstr(robMap.Strings[robMap.numStrings],"NumPoints:"))
				{
					robMap.numPoints=atoi(&((robMap.Strings[robMap.numStrings])[10]));
					if ((robMap.numPoints>0))
						robMap.Points.matrixSetsize(2,robMap.numPoints,0.0);
					else
						robMap.numPoints=-1;

				}
				if (strstr(robMap.Strings[robMap.numStrings],"NumLines:"))
				{
					robMap.numLines=atoi(&((robMap.Strings[robMap.numStrings])[9]));
					if ((robMap.numLines>0))
						robMap.Lines.matrixSetsize(4,robMap.numLines,0.0);
					else
						robMap.numLines=-1;
				}

				if (strcasecmp(robMap.Strings[robMap.numStrings],"LINES")==0)
				{
					robMap.isComplete=2;
				}
				if (strcasecmp(robMap.Strings[robMap.numStrings],"DATA")==0)
				{
					robMap.isComplete=3;
				}
				robMap.numStrings++;
				break;
							
			}
		case 2:
			{ 
				if (robMap.numLines>0)
				{
					int linesInPacket=packet->bufToByte4();
					for(i=robMap.numLinesRead;i<robMap.numLinesRead+linesInPacket;i++)
					{
						robMap.Lines.val[0][i]=packet->bufToByte4();
						robMap.Lines.val[1][i]=packet->bufToByte4();
						robMap.Lines.val[2][i]=packet->bufToByte4();
						robMap.Lines.val[3][i]=packet->bufToByte4();
					}
					if (robMap.numLinesRead+linesInPacket<robMap.numLines)
						robMap.numLinesRead=robMap.numLinesRead+linesInPacket;
					else
					{
						robMap.numLinesRead=robMap.numLinesRead+linesInPacket;
						robMap.isComplete=1;
					}
				}
				else
					robMap.isComplete=1;

				break;
			}
		case 3:
			{ 
				if (robMap.numPoints>0)
				{
					int pointsInPacket=packet->bufToByte4();
					for(i=robMap.numPointsRead;i<robMap.numPointsRead+pointsInPacket;i++)
					{
						robMap.Points.val[0][i]=packet->bufToByte4();
						robMap.Points.val[1][i]=packet->bufToByte4();
					}
					if (robMap.numPointsRead+pointsInPacket<robMap.numPoints)
						robMap.numPointsRead=robMap.numPointsRead+pointsInPacket;
					else
					{
						robMap.numPointsRead=robMap.numPointsRead+pointsInPacket;
						robMap.isComplete=4;
					}
				}
				else
					robMap.isComplete=4;

				break;
			}
		case 4:
			{
				robMap.isComplete=4;
				break;
			}
	}
		
}

void RobotController::handleReceiveMapName(ArNetPacket *packet)
{
	memset(robMap.Name, 0, sizeof(robMap.Name));
	packet->bufToStr(robMap.Name,255);
	
	
}
void RobotController::handleReceiveSafeDrive(ArNetPacket *packet)
{
	safeDrive=(int)packet->bufToByte();
}

void RobotController::handleReceiveCameraStatus(ArNetPacket *packet)
{
	cameraStatus.val[0][0]=(double)packet->bufToByte2();
	cameraStatus.val[0][1]=(double)packet->bufToByte2();
	cameraStatus.val[0][2]=(double)packet->bufToByte2();

}

void RobotController::handleReceiveCameraInfo(ArNetPacket *packet)
{
	cameraInfo.val[0][0]=(double)packet->bufToByte2();
	cameraInfo.val[0][1]=(double)packet->bufToByte2();
	cameraInfo.val[0][2]=(double)packet->bufToByte2();
	cameraInfo.val[0][3]=(double)packet->bufToByte2();
	cameraInfo.val[0][4]=(double)packet->bufToByte2();
	cameraInfo.val[0][5]=(double)packet->bufToByte2();
	cameraInfo.val[0][6]=(double)packet->bufToByte();

}

void RobotController::handleReceiveLocalisation(ArNetPacket *packet)
{
	locState=(int)packet->bufToByte();
	locScore=((double)packet->bufToByte2())/1000.0;
	

}
void RobotController::handleReceivePath(ArNetPacket *packet)
{
	navPathPoints=(int)packet->bufToByte2();
	navPath.matrixResize(2,navPathPoints,0.0);
	for(int i=0;i<navPathPoints;i++)
		for(int j=0;j<2;j++)
			navPath.val[j][i]=(double)packet->bufToByte4();

}

void RobotController::handleReceivePathPlannerStatus(ArNetPacket *packet)
{
	memset(navPathPlannerStatus, 0, sizeof(navPathPlannerStatus));
	packet->bufToStr(navPathPlannerStatus, 63);


}

void RobotController::handleReceivePacket(ArNetPacket *packet)
{
	//printf("pose\n");
    memset(robStatus, 0, sizeof(robStatus));
	memset(robMode, 0, sizeof(robMode));
	packet->bufToStr(robStatus, 255);
	packet->bufToStr(robMode, 31);
	batteryInfo.val[0][0] = ( (double) packet->bufToByte2() )/10.0;
	robPose.val[0][0]=packet->bufToByte4();
	robPose.val[0][1]=packet->bufToByte4();
	robPose.val[0][2]=packet->bufToByte2();
	robVel.val[0][0] = (double) packet->bufToByte2();
	robVel.val[0][1] = (double) packet->bufToByte2();
	robotIsConnected=1;
}

void RobotController::handleReceiveBatteryInfo(ArNetPacket *packet)
{
	batteryInfo.val[0][1]=packet->bufToDouble();
	batteryInfo.val[0][2]=packet->bufToDouble();
}
int RobotController::robot_GotoGoal(int goal_number)
{
	if ((navGoalNumber < goal_number)|(goal_number<1))
		return -1;
	else
	{
		robotClient->requestOnceWithString("gotoGoal", navGoalList[goal_number-1]);
		return robotIsConnected;
	}

}

int RobotController::robot_GotoHome(void)
{
	robotClient->requestOnce("home");
	return robotIsConnected;
}

int RobotController::robot_Wander(void)
{
	robotClient->requestOnce("wander");
	return robotIsConnected;
}

int RobotController::robot_TourGoals(void)
{
	robotClient->requestOnce("tourGoals");
	return robotIsConnected;
}

int RobotController::robot_SetSafeDrive(int safedrive)
{
	ArNetPacket packet;
	packet.byteToBuf(safedrive);
  	robotClient->requestOnce("setSafeDrive",&packet);
	return robotIsConnected;
	
}

int RobotController::robot_useGyro(int gyro)
{
	if (gyro == 1)
	{
  		robotClient->requestOnce("GyroEnable");
	}
	else if (gyro == 0)
	{
		robotClient->requestOnce("GyroDisable");
	}
	return robotIsConnected;
	
}

int RobotController::robot_MicroControllerCommand(char *commandString)
{
	robotClient->requestOnceWithString("MicroControllerCommand",commandString);
	return robotIsConnected;
	
}

int RobotController::robot_Stop(void)
{
	robotClient->requestOnce("stop");
	return robotIsConnected;
}



int RobotController::robot_GotoPose(const CMatrixT<double> &pose)
{
	ArNetPacket packet;
	if ((pose.matrixCols()!=3)|(pose.matrixRows()!=1))
		return -1;
	else
	{
		packet.byte4ToBuf(pose.val[0][0]);
		packet.byte4ToBuf(pose.val[0][1]);
		packet.byte4ToBuf(pose.val[0][2]);
		robotClient->requestOnce("gotoPose",&packet);	
		return robotIsConnected;
	}
}


int RobotController::robot_LocalizeToPose(const CMatrixT<double> &pose)
{
	ArNetPacket packet;
	if ((pose.matrixCols()!=3)|(pose.matrixRows()!=1))
		return -1;
	else
	{
		packet.byte4ToBuf(pose.val[0][0]);
		packet.byte4ToBuf(pose.val[0][1]);
		packet.byte4ToBuf(pose.val[0][2]);
		robotClient->requestOnce("localizeToPose",&packet);	
		return robotIsConnected;
	}
}

int RobotController::robot_SetCameraRel(const CMatrixT<double> &pose)
{
	ArNetPacket packet;
	if ((pose.matrixCols()!=3)|(pose.matrixRows()!=1))
		return -1;
	else
	{
		packet.byteToBuf(ArTypes::Byte(pose.val[0][0]));
		packet.byteToBuf(ArTypes::Byte(pose.val[0][1]));
		packet.byteToBuf(ArTypes::Byte(pose.val[0][2]));
		robotClient->requestOnce("camera",&packet);	
		return robotIsConnected;
	}
}

int RobotController::robot_SetCameraAbs(const CMatrixT<double> &pose)
{
	ArNetPacket packet;
	if ((pose.matrixCols()!=3)|(pose.matrixRows()!=1))
		return -1;
	else
	{
		packet.byteToBuf(ArTypes::Byte(pose.val[0][0]));
		packet.byteToBuf(ArTypes::Byte(pose.val[0][1]));
		packet.byteToBuf(ArTypes::Byte(pose.val[0][2]));
		robotClient->requestOnce("cameraAbs",&packet);	
		return robotIsConnected;
	}
}

#ifdef MATZL_MEX
mxArray* RobotController::robot_Map2Matlab(void)
{
	
	int i;
	
	/////////////////////////////////////////
	//struct
	//	{
	//		char Name[256];
	//		char *Strings[1024];
	//		int numLines;
	//		int numPoints;
	//		int numStrings;
	//		int isComplete;
	//		CMatrixT<double> Points;
	//		CMatrixT<double> Lines;
	//		int numLinesRead;
	//		int numPointsRead;
	//		int isComplete;
	//	} robMap;


	mxArray* robotMap;
	const char *names_robot_map[] = {
		"Name",
		"Strings",
		"numLines",
		"numPoints",
		"Points",
		"Lines",
		"isComplete"};
	mwSize dim_robot_map[2] = {1,1};
	robotMap = mxCreateStructArray(2, dim_robot_map, 7, names_robot_map);
	mxSetField(robotMap,0,"Name",mxCreateString(robMap.Name));
	dim_robot_map[1] = robMap.numStrings;
	mxSetField(robotMap,0,"Strings",mxCreateCellArray(2,dim_robot_map));
    for (i=0;i<robMap.numStrings;i++)
        mxSetCell(mxGetField(robotMap,0,"Strings"),i,mxCreateString(robMap.Strings[i]));
	mxSetField(robotMap,0,"numLines",scalar2mxArray(robMap.numLines));
	mxSetField(robotMap,0,"numPoints",scalar2mxArray(robMap.numPoints));
	mxSetField(robotMap,0,"isComplete",scalar2mxArray(robMap.isComplete));
	mxSetField(robotMap,0,"Points",matrix2mxArray(robMap.Points));
	mxSetField(robotMap,0,"Lines",matrix2mxArray(robMap.Lines));
    return robotMap;
}


mxArray* RobotController::robot_Sensors2Matlab(void)
{
	
	int i;
	
	/////////////////////////////////////////
	//struct
	//	{
	//		char *Name[256];
	//		char NameRead[256];
	//		CMatrixT<double> *Data[256];
	//		int Number;
	//	} robSensors;

	mxArray* robotSensors;
	const char *names_robot_sensors[] = {
		"Name",
		"NameRead",
		"Data",
		"Number"};
	mwSize dim_robot_sensors[2] = {1,1};
	robotSensors = mxCreateStructArray(2, dim_robot_sensors, 4, names_robot_sensors);
	dim_robot_sensors[1] = robSensors.Number;
	mxSetField(robotSensors,0,"Name",mxCreateCellArray(2,dim_robot_sensors));
	mxSetField(robotSensors,0,"Data",mxCreateCellArray(2,dim_robot_sensors));
	for (i=0;i<robSensors.Number;i++)
	{
		mxSetCell(mxGetField(robotSensors,0,"Name"),i,mxCreateString(robSensors.Name[i]));
		mxSetCell(mxGetField(robotSensors,0,"Data"),i,matrix2mxArray(*(robSensors.Data[i])));
	}
	
	mxSetField(robotSensors,0,"Number",scalar2mxArray(robSensors.Number));
	mxSetField(robotSensors,0,"NameRead",mxCreateString(robSensors.NameRead));
	
	return robotSensors;
}



mxArray* RobotController::robot_Data2Matlab(void)
{
    
	int i;
	
	mxArray* robot_data;
	const char *names_robot_data[] = {
		"Status",
		"isConnected",
		"Mode",
		"safeDrive",
		"battery",
		"Pose",
		"Vel",
		"locState",
		"locScore",
		"navPathPlannerStatus",
		"navPathPoints",
		"navPath",
		"navGoalNumber",
		"navGoalList",
		"camera",
		"cameraInfo",
		"gpsInfo",
		"compassInfo",
		};
	mwSize dim_robot_data[2] = {1,1};
	robot_data = mxCreateStructArray(2, dim_robot_data, 18, names_robot_data);
	mxSetField(robot_data,0,"Status",mxCreateString(robStatus));
	mxSetField(robot_data,0,"Mode",mxCreateString(robMode));
	mxSetField(robot_data,0,"safeDrive",scalar2mxArray(safeDrive));
    mxSetField(robot_data,0,"battery",matrix2mxArray(batteryInfo));
	mxSetField(robot_data,0,"Pose",matrix2mxArray(robPose));
	mxSetField(robot_data,0,"Vel",matrix2mxArray(robVel));
	mxSetField(robot_data,0,"isConnected",scalar2mxArray(int(robotClient->isConnected())));
	mxSetField(robot_data,0,"locScore",scalar2mxArray(locScore));
	mxSetField(robot_data,0,"locState",scalar2mxArray(locState));
	mxSetField(robot_data,0,"navPath",matrix2mxArray(navPath));
	mxSetField(robot_data,0,"navPathPlannerStatus",mxCreateString(navPathPlannerStatus));
	mxSetField(robot_data,0,"navPathPoints",scalar2mxArray(navPathPoints));
	mxSetField(robot_data,0,"navGoalNumber",scalar2mxArray(navGoalNumber));
	dim_robot_data[1] = navGoalNumber;
	mxSetField(robot_data,0,"navGoalList",mxCreateCellArray(2,dim_robot_data));
    for (i=0;i<navGoalNumber;i++)
        mxSetCell(mxGetField(robot_data,0,"navGoalList"),i,mxCreateString(navGoalList[i]));
	mxSetField(robot_data,0,"camera",matrix2mxArray(cameraStatus));
	mxSetField(robot_data,0,"cameraInfo",matrix2mxArray(cameraInfo));
	mxSetField(robot_data,0,"gpsInfo",matrix2mxArray(gpsInfo));
	mxSetField(robot_data,0,"compassInfo",matrix2mxArray(compassInfo));
	robotIsConnected=0;
	return robot_data;
}
void RobotController::setupMatlab()
{
	
	if (printDeveloperMessage)
	{
		mexPrintf("***************************************************************\n");
		mexPrintf("** ARIA Gateway\n");
		mexPrintf("** Fraunhofer Institute for Information and Dataprocessing IITB\n");
		mexPrintf("** Fraunhofer Str. 1, 76131 Karlsruhe Germany\n");
		mexPrintf("** \n");
		mexPrintf("** 2008 Christian W. Frey, Thomas Emter\n");
		mexPrintf("** \n");
		mexPrintf("** ROBOT CONNECTION TO: ");mexPrintf(robIPAdress);mexPrintf("\n");
		mexPrintf("** THIS IS NO FREEWARE / ONLY FOR EVALUATION PURPOSES\n");
		mexPrintf("***************************************************************\n");
	}
}

#endif
