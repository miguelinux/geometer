/*! \file globjects.h
 *   This is the header file where is defined the class GLObject
 */
#ifndef _GLOBJECTS_H
#define _GLOBJECTS_H

#ifdef WIN32
 #include <windows.h>
#endif
#include <GL/glu.h>
#include <GL/gl.h>

#include "Math/Math3d.h"

#include "glamros.h"
#include "glgeometer.h"

#include "commonvars.h"

#include <cv.h>

#include "rgbd/rgbdimage.h"

#define GL_VIEW_POINTS   0x00010000
#define GL_VIEW_LASER_P  0x00020000
#define GL_VIEW_CAMERA_P 0x00040000
#define GL_VIEW_LANDMARKS 0x00080000

//!  GLObjects class, the OpenGL objects and functions.
/*!
 * This is where all the function relative to OpenGL will be
 * coded.
 */
class GLObjects
{

  private:

   GLuint objAxes;
   GLuint objGrid;

   Vec3dList vV3D;

   // Quadrics are used to draw many different shapes.
   GLUquadricObj *generalQuadric;

   double posX;
   double posY;
   double posZ;
   double posTh;
   float scale;
   float yMax;
   float yMin;
   float zMin;

   unsigned int conf;

   void drawV3D();

   void drawLaserPoints();

   void drawCameraPoints();

   void drawLandmarks();

   void dKinect();
   void dKinect1();

  public:

   //! The constructor of the GLObjects class.
   GLObjects();
   //! The destructor of the GLObjects class.
   ~GLObjects();

   Vec3dList vAllLaserPoints;
   Vec3dList vActualLaserPoints;
   Vec3dList vCameraPoints;

   unsigned short numLandmarks;

   std::vector<CvPoint3D64f> vLandmarks[MAX_LANDMARKS_NUM];

   std::vector< CvPoint3D32f > v3Dkp;

   GlAmros glAmros;

   GlGeometer glGeometer;

   RGBDImage *rgbdI;

   void getCloud(short version = 0);

   void draw();

   void addV3D(Vec3d point);

   void setupAxes();
   void setupGrid();

   void setup(float fScale);

   void setConfig(unsigned int c);

   // Se mueve a la posicion (nX, nY, nZ)
   void moveTo(double nX, double nY, double nZ, double nTh);
   // Se mueve a la posicion relativa (nX, nY, nZ)
   void moveRel(double nX=0, double nY=0, double nZ=0, double nTh=0);
   

};

#endif

