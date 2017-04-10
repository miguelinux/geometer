#if defined(_MSC_VER) || __GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ > 3)
#pragma once
#endif

#ifndef _MY_PROTOCOL_H_
#define _MY_PROTOCOL_H_

enum {
    ID_PROTO_DATA = 0x80000000,
#define  ID_PROTO_DATA  ID_PROTO_DATA
    ID_PROTO_CMD  = 0x40000000,
#define  ID_PROTO_CMD   ID_PROTO_CMD
    ID_PROTO_FAIL = 0x20000000,
#define  ID_PROTO_FAIL  ID_PROTO_FAIL
    ID_PROTO_OK   = 0x10000000,
#define  ID_PROTO_OK    ID_PROTO_OK
    ID_PROTO_FLAG = 0xF0000000
#define  ID_PROTO_FLAG  ID_PROTO_FLAG
};

enum {
    ROBOT_CONNECT   =  0x00010000,
#define ROBOT_CONNECT   ROBOT_CONNECT
//#define ROBOT_RUN       0x0002
    ROBOT_GET_POSE  =  0x00030000,
#define ROBOT_GET_POSE  ROBOT_GET_POSE
#define ROBOT_EXIT      0x00040000
#define ROBOT_VEL       0x00050000
#define ROBOT_ROTVEL    0x00060000
#define ROBOT_VEL2      0x00070000
#define ROBOT_LASER     0x00080000
#define ROBOT_MOVE      0x00090000
#define ROBOT_STOP      0x000A0000
#define ROBOT_HEAD      0x000B0000
#define ROBOT_D_HEAD    0x000C0000
    ROBOT_LASER_DATA =  0x000D0000
#define ROBOT_LASER_DATA ROBOT_LASER_DATA
};

struct my_protocol_t {
  unsigned int id;
  char data[480];
};

struct robot_pose_t {
  double x;
  double y;
  double th;
  double vel;
  double volt;
};

#endif

