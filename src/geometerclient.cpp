#include "geometerclient.h"
#include <cstdlib>
#include <cstring>
#include <cctype>

GeometerClient::GeometerClient()
{
  char tmp[VAL_LEN];
  char *fileData = 0;
  short charPerLine;

  conf = new Configuration(CONFIG_FILE_C, CONFIG_DIR_C);

  if ( ! conf->fileExist() ) {
    charPerLine = 7*80;  // # lines with 80 characters
    fileData = new char[charPerLine];
    STRCPY(fileData,charPerLine,"; Configuration file for GeometerClient.\n\n");
    STRNCAT(fileData,";Server port.\n",charPerLine);
    STRNCAT(fileData,"server_port=2011\n\n",charPerLine);
    STRNCAT(fileData,";Server IP.\n",charPerLine);
    STRNCAT(fileData,"server_ip=127.0.0.1\n\n",charPerLine);
    STRNCAT(fileData,";Logfile.\n",charPerLine);
    STRNCAT(fileData,"logfile=geometerclient.log\n\n",charPerLine);
    conf->setDefaultNewFileData(fileData);
  }

  if (conf->readFile()) {
    // setup configuration
    conf->get(tmp, "server_port");
    server_port = atoi(tmp);

    conf->get(server_ip, "server_ip");
    conf->get(logfile, "logfile");

  } else {
    // use default configuration;
    server_port = 2011;
    *server_ip=0;
    *logfile=0;
  }

  delete conf;
  if (fileData) delete [] fileData;
  conf = 0;
  ds = 0;

  retLen = sizeof(unsigned int);
  robotState = 0;
  connected = false;
}

GeometerClient::~GeometerClient()
{
  if (connected) exit();
  if (conf) delete conf;
  if (ds)   delete ds;
}

void GeometerClient::parseArgs(int argc, char *argv[])
{
  short i;

  for(i=0; i<argc; i++) {
    if (argv[i][0] == '-' || argv[i][0] == '/' ) {

      switch(argv[i][1]) {
        case 'h': case 'H':
          log << "Usage: " << argv[0] << " [options]\n\n";
          log << "\t-p <port>    Port number to bind server.\n";
          log << "\t-i <IP>      Server IP.\n";
          log << "\t-l <file>    Logfile.\n\n";
          return;
        break;
        case 'p': case 'P': // Port number
          if ( i+1  < argc ) {
            server_port = atoi(argv[i+1]);
          }
        break;
        case 'i': case 'I':  // Shared folder
          if ( i+1  < argc ) {
            STRCPY(server_ip,VAL_LEN,argv[i+1]);
          }
        break;
        case 'l': case 'L':  // Logfile
          if ( i+1  < argc ) {
            STRCPY(logfile, VAL_LEN, argv[i+1]);
          }
        break;
        default:
          // bla bla
        break;
      }
    }
  }

  if (*logfile) {
    log.saveToFile(logfile);
  }

  log.getTime();
  log << "Geometer Client running.\n";
  log << "Server IP: " << server_ip << "\n";
  log << "Server port: " << server_port << "\n";
  log << "Logfile: " << logfile << "\n";
  log.flush();

  if (!(*server_ip)) {
    log.getTime();
    log << "Error: No server IP given\n";
    log.flush();
    return;
  }

  ds = new DatagramSocket(server_port, server_ip);
}

bool GeometerClient::connect()
{
  bool robotRet = false;
  if (connected) return connected;
  msg.id = ID_PROTO_CMD | ROBOT_CONNECT;
  //log.getTime();
  //log << "Connect to geometer: ";
  if (ds->send(&msg,retLen)) {
    //log << "msg sent.\n";
    ds->setTimeVal(5L,0L);
    if (ds->receive(&msg, &msgLen)) {
      //log.getTime();
      //log << "Answare received. [";
      if (msg.id == ID_PROTO_OK) {
        //log << "OK]\n";
        robotRet = true;
      //} else {
        //log << "FAIL]\n";
      }
    }
  } else {
    log << "Can not send msg.\n";
  }
  connected = robotRet;
  return robotRet;
}


bool GeometerClient::getPose()
{
  robot_pose_t *pRP = 0;
  bool robotRet = false;
  msg.id = ID_PROTO_CMD | ROBOT_GET_POSE;
  //log.getTime();
  //log << "Get pose from geometer: ";
  if (ds->send(&msg,retLen)) {
    //log << "msg sent.\n";
    ds->setTimeVal(5L,0L);
    if (ds->receive(&msg, &msgLen)) {
      //log.getTime();
      //log << "Answare received. [";
      if ((msg.id & ID_PROTO_DATA) && (msg.id & ROBOT_GET_POSE)) {
        //log << "OK]\n";
        pRP = reinterpret_cast<robot_pose_t *>(msg.data);
        rp.x =   pRP->x;
        rp.y =   pRP->y;
        rp.th =  pRP->th;
        rp.vel = pRP->vel;
        rp.volt = pRP->volt;
        robotRet = true;
      //} else {
        //log << "FAIL]\n";
      }
    }
  } else {
    log << "Can not send msg.\n";
  }
  return robotRet;
}

bool GeometerClient::getRawReadings()
{
  bool robotRet = false;
  short dataRecv = 0;
  double *data = laserData;

  msg.id = ID_PROTO_CMD | ROBOT_LASER_DATA;
  laserDataSize = 0;

  if (ds->send(&msg,retLen)) {
    ds->setTimeVal(5L,0L);
    do {
      if (ds->receive(&msg, &msgLen)) {
        if ((msg.id & ID_PROTO_DATA) && (msg.id & ROBOT_LASER_DATA)) {
          dataRecv = (short)msg.id & 0xFF;
          ::memcpy(data, msg.data, sizeof(double)*dataRecv);
          data+=dataRecv;
          laserDataSize+=dataRecv;
          robotRet = true;
        }
      }
    } while(dataRecv == 60);
  } else {
    log << "Can not send msg to get laser data.\n";
  }
  return robotRet;
}

bool GeometerClient::setVel(double d)
{
  double *pD = 0;
  bool robotRet = false;
  
  msg.id = ID_PROTO_CMD | ROBOT_VEL;
  pD = reinterpret_cast<double *>(msg.data);
  *pD = d;

  //log.getTime();
  //log << "Set vel to geometer: ";

  msgLen = retLen + sizeof(double);
  if (ds->send(&msg,msgLen)) {
    //log << "msg sent.\n";
    ds->setTimeVal(5L,0L);
    if (ds->receive(&msg, &msgLen)) {
      //log.getTime();
      //log << "Answare received. [";
      if (msg.id == ID_PROTO_OK) {
        //log << "OK]\n";
        robotRet = true;
      //} else {
        //log << "FAIL]\n";
      }
    }
  } else {
    log << "Can not send msg.\n";
  }
  return robotRet;
}

bool GeometerClient::setRotVel(double d)
{
  double *pD = 0;
  bool robotRet = false;
  
  msg.id = ID_PROTO_CMD | ROBOT_ROTVEL;
  pD = reinterpret_cast<double *>(msg.data);
  *pD = d;

  //log.getTime();
  //log << "Set rot vel to geometer: ";

  msgLen = retLen + sizeof(double);
  if (ds->send(&msg,msgLen)) {
    //log << "msg sent.\n";
    ds->setTimeVal(5L,0L);
    if (ds->receive(&msg, &msgLen)) {
      //log.getTime();
      //log << "Answare received. [";
      if (msg.id == ID_PROTO_OK) {
        //log << "OK]\n";
        robotRet = true;
      //} else {
        //log << "FAIL]\n";
      }
    }
  } else {
    log << "Can not send msg.\n";
  }
  return robotRet;
}

bool GeometerClient::setVel2(double l, double r)
{
  double *pD = 0;
  bool robotRet = false;
  
  msg.id = ID_PROTO_CMD | ROBOT_VEL2;
  pD = reinterpret_cast<double *>(msg.data);
  pD[0] = l;
  pD[1] = r;

  //log.getTime();
  //log << "Set vel2 to geometer: ";

  msgLen = retLen + 2*sizeof(double);
  if (ds->send(&msg,msgLen)) {
    //log << "msg sent.\n";
    ds->setTimeVal(5L,0L);
    if (ds->receive(&msg, &msgLen)) {
      //log.getTime();
      //log << "Answare received. [";
      if (msg.id == ID_PROTO_OK) {
        //log << "OK]\n";
        robotRet = true;
      //} else {
        //log << "FAIL]\n";
      }
    }
  } else {
    log << "Can not send msg.\n";
  }
  return robotRet;
}

bool GeometerClient::move(double d)
{
  double *pD = 0;
  bool robotRet = false;
  
  msg.id = ID_PROTO_CMD | ROBOT_MOVE;
  pD = reinterpret_cast<double *>(msg.data);
  *pD = d;

  //log.getTime();
  //log << "Move geometer: ";

  msgLen = retLen + sizeof(double);
  if (ds->send(&msg,msgLen)) {
    //log << "msg sent.\n";
    ds->setTimeVal(5L,0L);
    if (ds->receive(&msg, &msgLen)) {
      //log.getTime();
      //log << "Answare received. [";
      if (msg.id == ID_PROTO_OK) {
        //log << "OK]\n";
        robotRet = true;
      //} else {
        //log << "FAIL]\n";
      }
    }
  } else {
    log << "Can not send msg.\n";
  }
  return robotRet;
}

bool GeometerClient::stop()
{
  bool robotRet = false;
  msg.id = ID_PROTO_CMD | ROBOT_STOP;
  //log.getTime();
  //log << "Stop geometer: ";
  if (ds->send(&msg,retLen)) {
    //log << "msg sent.\n";
    ds->setTimeVal(5L,0L);
    if (ds->receive(&msg, &msgLen)) {
      //log.getTime();
      //log << "Answare received. [";
      if (msg.id == ID_PROTO_OK) {
        //log << "OK]\n";
        robotRet = true;
      //} else {
        //log << "FAIL]\n";
      }
    }
  } else {
    log << "Can not send msg.\n";
  }
  return robotRet;
}

bool GeometerClient::initLaser()
{
  bool robotRet = false;
  msg.id = ID_PROTO_CMD | ROBOT_LASER;
  //log.getTime();
  //log << "Connect laser to geometer: ";
  if (ds->send(&msg,retLen)) {
    //log << "msg sent.\n";
    ds->setTimeVal(5L,0L);
    if (ds->receive(&msg, &msgLen)) {
      //log.getTime();
      //log << "Answare received. [";
      if (msg.id == ID_PROTO_OK) {
        //log << "OK]\n";
        robotRet = true;
      //} else {
        //log << "FAIL]\n";
      }
    }
  } else {
    log << "Can not send msg.\n";
  }
  return robotRet;
}

bool GeometerClient::exit()
{
  bool robotRet = false;
  msg.id = ID_PROTO_CMD | ROBOT_EXIT;
  //log.getTime();
  //log << "Exit geometer: ";
  if (ds->send(&msg,retLen)) {
    //log << "msg sent.\n";
    ds->setTimeVal(5L,0L);
    if (ds->receive(&msg, &msgLen)) {
      //log.getTime();
      //log << "Answare received. [";
      if (msg.id == ID_PROTO_OK) {
        //log << "OK]\n";
        robotRet = true;
      //} else {
        //log << "FAIL]\n";
      }
    }
  } else {
    log << "Can not send msg.\n";
  }
  return robotRet;
}

bool GeometerClient::setHeading(double d)
{
  double *pD = 0;
  bool robotRet = false;
  
  msg.id = ID_PROTO_CMD | ROBOT_HEAD;
  pD = reinterpret_cast<double *>(msg.data);
  *pD = d;

  //log.getTime();
  //log << "Set heading to geometer: ";

  msgLen = retLen + sizeof(double);
  if (ds->send(&msg,msgLen)) {
    //log << "msg sent.\n";
    ds->setTimeVal(5L,0L);
    if (ds->receive(&msg, &msgLen)) {
      //log.getTime();
      //log << "Answare received. [";
      if (msg.id == ID_PROTO_OK) {
        //log << "OK]\n";
        robotRet = true;
      //} else {
        //log << "FAIL]\n";
      }
    }
  } else {
    log << "Can not send msg.\n";
  }
  return robotRet;
}

bool GeometerClient::setDeltaHeading(double d)
{
  double *pD = 0;
  bool robotRet = false;
  
  msg.id = ID_PROTO_CMD | ROBOT_D_HEAD;
  pD = reinterpret_cast<double *>(msg.data);
  *pD = d;

  //log.getTime();
  //log << "Set delta heading to geometer: ";

  msgLen = retLen + sizeof(double);
  if (ds->send(&msg,msgLen)) {
    //log << "msg sent.\n";
    ds->setTimeVal(5L,0L);
    if (ds->receive(&msg, &msgLen)) {
      //log.getTime();
      //log << "Answare received. [";
      if (msg.id == ID_PROTO_OK) {
        //log << "OK]\n";
        robotRet = true;
      //} else {
        //log << "FAIL]\n";
      }
    }
  } else {
    log << "Can not send msg.\n";
  }
  return robotRet;
}

void GeometerClient::flush()
{
  log.flush();
};

