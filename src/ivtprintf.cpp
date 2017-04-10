/*! \file ivtprintf.cpp
 *   This is where the IVTPrintf class is implemented.
 */
#include "ivtprintf.h"

void IVTPrintf::p(Vec2d v, const char *str) {
  printf("%s, x=%5.6f  y=%5.6f\n",str,v.x,v.y);
}

void IVTPrintf::p(Mat2d v, const char *str) {
  printf("%s:\n",str);
  printf("|%5.6f  %5.6f|\n",v.r1, v.r2);
  printf("|%5.6f  %5.6f|\n",v.r3, v.r4);
}

void IVTPrintf::p(Vec3d v, const char *str) {
  printf("%s, x=%5.6f  y=%5.6f  z=%5.6f\n",str,v.x,v.y,v.z);
}

void IVTPrintf::p(Mat3d v, const char *str) {
  printf("%s:\n",str);
  printf("| %5.6f  %5.6f  %5.6f |\n",v.r1, v.r2, v.r3);
  printf("| %5.6f  %5.6f  %5.6f |\n",v.r4, v.r5, v.r6);
  printf("| %5.6f  %5.6f  %5.6f |\n",v.r7, v.r8, v.r9);
}

