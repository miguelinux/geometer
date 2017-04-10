#include "mapbuilder.h"

MapBuilder::MapBuilder() {
}

MapBuilder::~MapBuilder() {
}

void MapBuilder::setPoints(Vec3dList v3DL) {
  v3DLaserPoints.clear();
  v3DLaserPoints = v3DL;
}

void MapBuilder::p2l() {
  //int s = v3DLaserPoints.size();
  //printf("X;Y\n");
  //for(int i=0; i<s; i++) {
    //printf("%.2f;%.2f\n",v3DLaserPoints[i].x,v3DLaserPoints[i].y);
  //}
  printf("Fin***;***Fin\n");
  /*
  for(Vec3dList::iterator it =v3DLaserPoints.begin(); it != v3DLaserPoints.end(); it++){
  }
  */
}

