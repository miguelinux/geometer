#include "laserpoint.h"

LaserPoint::LaserPoint() {
  p[0]=0.0;
  p[1]=0.0;
}

LaserPoint::LaserPoint(double x, double y) {
  p[0]=x;
  p[1]=y;
}

LaserPoint::~LaserPoint() {
}

double LaserPoint::x() {
  return p[0];
}

double LaserPoint::y() {
  return p[1];
}

void LaserPoint::setX(double x) {
  p[0]=x;
}

void LaserPoint::setY(double y) {
  p[1]=y;
}


