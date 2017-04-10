#include "geometer.h"
#include <string.h>

Geometer::Geometer() {
  int ARGC = 1;
  char *ARGV[1];
  ARGV[0] =  new char[9];
  strcpy(ARGV[0],"geometer");

  Aria::init();

  argParser = new ArArgumentParser(&ARGC, ARGV);

  argParser->loadDefaultArguments();

  // ArRobotConnector connects to the robot, get some initial data from it
  // such as type and name, and then loads parameter files for this robot.
  robotConnector = new ArRobotConnector(argParser, &robot);
  laserConnector = new ArLaserConnector(argParser, &robot, robotConnector);

  // Always try to connect to the first laser:
  argParser->addDefaultArgument("-connectLaser");

  isConnected = false;
  myLaser=0;
  nLaserPoints = 0;

  delete [] ARGV[0];
}

Geometer::Geometer(int argc, char *argv[]) {

  Aria::init();

  argParser = new ArArgumentParser(&argc, argv);

  argParser->loadDefaultArguments();

  // ArRobotConnector connects to the robot, get some initial data from it
  // such as type and name, and then loads parameter files for this robot.
  robotConnector = new ArRobotConnector(argParser, &robot);
  laserConnector = new ArLaserConnector(argParser, &robot, robotConnector);

  // Always try to connect to the first laser:
  argParser->addDefaultArgument("-connectLaser");

  isConnected = false;
  myLaser=0;
  nLaserPoints = 0;
}

Geometer::~Geometer() {
  // Ending robot thread...
  robot.stopRunning();

  // wait for robot task loop to end before exiting the program
  robot.waitForRunExit();

  Aria::shutdown();

  delete laserConnector;
  delete robotConnector;
  delete argParser;
}

void Geometer::connectAndRun() {
  if(!robotConnector->connectRobot()) {
    ArLog::log(ArLog::Terse, "Could not connect to the robot.");
    if(argParser->checkHelpAndWarnUnparsed())
    {
        // -help not given, just exit.
        Aria::logOptions();
        Aria::exit(1);
    }
  }

  // Trigger argument parsing
  if (!Aria::parseArgs() || !argParser->checkHelpAndWarnUnparsed()) {
    Aria::logOptions();
    Aria::exit(1);
  }

  robot.addRangeDevice(&sonar);

  // Start the robot processing cycle running in the background.
  // True parameter means that if the connection is lost, then the 
  // run loop ends.
  robot.runAsync(true);

  // We must "lock" the ArRobot object
  // before calling its methods, and "unlock" when done, to prevent conflicts
  // with the background thread started by the call to robot.runAsync() above.
  // See the section on threading in the manual for more about this.
  // Make sure you unlock before any sleep() call or any other code that will
  // take some time; if the robot remains locked during that time, then
  // ArRobot's background thread will be blocked and unable to communicate with
  // the robot, call tasks, etc.

  isConnected = true;
}

bool Geometer::connectLaser() {
  // Connect to the laser(s) if lasers were configured in this robot's parameter
  // file or on the command line, and run laser processing thread if applicable
  // for that laser class.  (For the purposes of this demo, add all
  // possible lasers to ArRobot's list rather than just the ones that were
  // specified with the connectLaser option (so when you enter laser mode, you
  // can then interactively choose which laser to use from the list which will
  // show both connected and unconnected lasers.)

  // try to connect to laser. if fail, warn but continue, using sonar only
  if(!laserConnector->connectLasers()) {
    ArLog::log(ArLog::Normal, "Warning: unable to connect to requested lasers, will wander using robot sonar only.");
    return false;
  }

  ArUtil::sleep(1500);
  myLaser = robot.findLaser(1);

  if (myLaser==NULL){
    ArUtil::sleep(1500);
    myLaser = robot.findLaser(1);
  }

  return (myLaser != NULL);
}


void Geometer::getLaserReadings() {
  const std::list<ArSensorReading *> *readings;
  std::list<ArSensorReading *>::const_iterator it;
  std::list<ArSensorReading *>::const_reverse_iterator rit;
  ArPose encoderPoseTaken;
  ArPose globalPoseTaken;
  ArSensorReading *reading;

  // we take readings in any of the following cases if we haven't
  // taken one yet or if we've been explicitly told to take one or if
  // we've gone further than myDistDiff if we've turned more than
  // myDegDiff if we've switched sign on velocity and gone more than
  // 50 mm (so it doesn't oscilate and cause us to trigger)

  if (robot.isConnected() && myLaser ) {
    myLaser->lockDevice();
    /// use the adjusted raw readings if we can, otherwise just use
    /// the raw readings like before
    if ((readings = myLaser->getAdjustedRawReadings()) != NULL)
    {
      usingAdjustedReadings = true;
    }
    else
    {
      usingAdjustedReadings = false;
      readings = myLaser->getRawReadings();
    }
    if (readings == NULL || (it = readings->begin()) == readings->end() ) {
      myLaser->unlockDevice();
      return;
    }
    /*
    if (usingAdjustedReadings) {
      ArLog::log(ArLog::Normal, 
		 "Taking adjusted readings from the %d laser values", 
		 readings->size());
    } else {
      ArLog::log(ArLog::Normal, 
		 "Taking readings from the %d laser values", 
		 readings->size());
    }
    */
    
    /*
    printf("velocities: %.2f %.2f %.2f\n", 
	    robot.getVel(), robot.getRotVel(), robot.getLatVel());

    encoderPoseTaken = (*readings->begin())->getEncoderPoseTaken();
    printf("robot: %.0f %.0f %.2f\n", 
	  encoderPoseTaken.getX(), 
	  encoderPoseTaken.getY(), 
	  encoderPoseTaken.getTh());

    globalPoseTaken = (*readings->begin())->getPoseTaken();
    printf("robotGlobal: %.0f %.0f %.2f\n", 
	  globalPoseTaken.getX(), 
	  globalPoseTaken.getY(), 
	  globalPoseTaken.getTh());
    */

    /*
    { // reflector
      printf("reflector1: ");
	// make sure that the list is in increasing order
	for (it = readings->begin(); it != readings->end(); it++)
	{
	  reading = (*it);
	  if (!reading->getIgnoreThisReading())
	    printf("%d ", reading->getExtraInt());
	  else
	    printf("0 ");
	}
      printf("\n");
    }
    */

    /**
       Note that the the sick1: or scan1: must be the last thing in
       that timestamp, ie that you should put any other data before
       it.
     **/
    /*
    { //old style
      printf("sick1: ");
	// make sure that the list is in increasing order
	for (it = readings->begin(); it != readings->end(); it++)
	{
	  reading = (*it);
	  printf("%d ", reading->getRange());
	}
      printf("\n");
    }
    */

    nLaserPoints = 0;
    { //new style
      //printf("scan1: ");
      
	// make sure that the list is in increasing order
	for (it = readings->begin(); it != readings->end(); it++)
	{
	  reading = (*it);
	  if (!reading->getIgnoreThisReading()) {
	    //printf("%.0f %.0f  ", 
	    vLaserPoints[nLaserPoints].x =
	      //reading->getLocalX();//- myLaser->getSensorPositionX();
	      reading->getX();//- myLaser->getSensorPositionX();
	    vLaserPoints[nLaserPoints].y =
	      //reading->getLocalY();//- myLaser->getSensorPositionY();
	      reading->getY();//- myLaser->getSensorPositionY();
	    nLaserPoints++;
		    //);
	  }
	  //else printf("0 0  ");
	}
      //printf("\n");
    }
    myLaser->unlockDevice();
  } else {
    ArLog::log(ArLog::Normal, "Robot/Laser not connected");
  }
}



