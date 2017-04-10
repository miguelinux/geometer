/*! \file streamsocket.h
 *  This is the header file where is defined the class StreamSocket
 */
#if defined(_MSC_VER) || __GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ > 3)
#pragma once
#endif

#ifndef __GEOMETER_H
#define __GEOMETER_H

#include "Aria.h"
#include "il/point2.h"

class Geometer {
  private:
 /*
    CSVSCapture *capture;
    CByteImage **ppImages;

    int width;
    int height;
    CByteImage::ImageType;
    int nCameras;

    int options;
 */
  bool usingAdjustedReadings;

  protected:
    ArArgumentParser *argParser;
    ArRobotConnector *robotConnector;
    ArLaserConnector *laserConnector;

    ArLaser *myLaser;

  public:
    ArRobot robot;
    ArSonarDevice sonar;
    //ArLaserConnector::LaserData ld;

    bool isConnected;

    il::Point2d vLaserPoints[181];
    short nLaserPoints;

    Geometer();
    Geometer(int argc, char *argv[]);
    ~Geometer();


    void connectAndRun();
    bool connectLaser();
    void getLaserReadings();
};

#endif

