#if defined(_MSC_VER) || __GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ > 3)
#pragma once
#endif

#ifndef _GEOMETERCLIENT_H_
#define _GEOMETERCLIENT_H_

#include "common/datagramsocket.h"
#include "common/configuration.h"
#include "common/mylogger.h"
#include "myprotocol.h"

#ifdef WIN32
 #define CONFIG_DIR_C  "cinvestav"
#else
 #define CONFIG_DIR_C  ".cinvestav"
#endif
#define CONFIG_FILE_C "geometerclient.ini"

class GeometerClient 
{
  private:
    
    Configuration *conf;
    unsigned short server_port;

    int robotState;
    bool connected;
    short msgLen;
    short retLen;

    char server_ip[VAL_LEN];
    char logfile[VAL_LEN];
    
    my_protocol_t msg;
    robot_pose_t rp;

    DatagramSocket *ds;
    MyLogger log;

    double laserData[181*2];
    short  laserDataSize;

  public:

    GeometerClient();

    ~GeometerClient();

    void parseArgs(int argc, char *argv[]);

    bool connect();
    inline bool isConnected() { return connected; };
    
    bool stop();
    bool exit();
    bool initLaser();

    bool setVel(double d);
    bool setRotVel(double d);
    bool setVel2(double l, double r);
    bool move(double d);
    bool setHeading(double d);
    bool setDeltaHeading(double d);

    bool getPose();
    bool getRawReadings();

    inline double *getLaserData()     { return laserData; };
    inline short   getLaserDataSize() { return laserDataSize; };

    inline double getX()   { return rp.x; };
    inline double getY()   { return rp.y; };
    inline double getTh()  { return rp.th; };
    inline double getVel() { return rp.vel; };
    inline double getBatteryVoltage() { return rp.volt; };

    void flush();
};

#endif

