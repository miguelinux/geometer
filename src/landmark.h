/*! \file landmark.h
 */
#ifndef __LANDMARK_H
#define __LANDMARK_H

#define IMAGES_NUMBER 2
#define OBJ_NOT_FOUND 0
#define MAX_DISTANCE  300

#include <cv.h>
#include <vector>
#include <cmath>

class Landmark {
  private:

   std::vector<CvPoint3D64f> vPos3D; 

  public:

   Landmark(const char *sCascadeClassifier);

   ~Landmark();

   bool ok;

   // Region of the landmark in the images 0,1
   CvRect region[IMAGES_NUMBER];

   // found flag 
   short objFound[IMAGES_NUMBER];

   CvMemStorage *storage;
   CvHaarClassifierCascade *cascade;

   // Centrod's disparity
   short cenDisp;

   // Number of time that the landmark was found
   short numFound;

   // Id of landmark
   unsigned short id;

   CvPoint3D64f pos3D; 

   // reset de objFound=0
   void reset();

   std::vector<CvPoint3D64f> getPose(); 

   unsigned short addPose(double x, double y, double z);

   double dist3D(CvPoint3D64f p1, CvPoint3D64f p2);
};

#endif

