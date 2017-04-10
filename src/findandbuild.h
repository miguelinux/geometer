/*! \file findandbuild.h
 */
#ifndef _FIND_AND_BUILD_H
#define _FIND_AND_BUILD_H

#include "Image/ByteImage.h"
#include "Image/PrimitivesDrawer.h"
#include "Image/StereoMatcher.h"
#include "Image/ImageProcessor.h"
#include "Image/ImageProcessorCV.h"
#include "Image/IplImageAdaptor.h"
#include "Structs/Structs.h"

#include "Calibration/Calibration.h"
#include "Calibration/StereoCalibration.h"

#include "landmark.h"
#include "commonvars.h"

#define L_IMG 0
#define R_IMG 1


// Threshold from the area of objets found
#define THRESHOLD_DIFF_AREA 20
// Number of table of disparities
#define NUM_OF_DISP  70
// Initial deep in mm
#define INITIAL_DEEP  300
// Increment on deep in mm
#define INC_DEEP  50

#define NOT_FOUND_MATCH 0

   //!
   /*!
    *
    */
class FindAndBuild {

 private:

   //!
   /*!
    *
    */
  CByteImage *ivtImages[2];

   //!
   /*!
    *
    */
  IplImage *cvImages[2];

   //!
   /*!
    *
    */
  CByteImage *procImg;

   //!
   /*!
    *
    */
  MyRegion objRegion[2];

   //!
   /*!
    *
    */
  Landmark *lndmrk;

   //!
   /*!
    *
    */
  CStereoCalibration *stereoCal;

   //!
   /*!
    *
    */
  short dispProx[NUM_OF_DISP];

   //!
   /*!
    *
    */
  short nWindowSize;
   //!
   /*!
    *
    */
  short dispThreshold;
   //!
   /*!
    *
    */
  float matchThreshold;
   //!
   /*!
    *
    */
  bool  inputImagesAreUndistorted;

   //!
   /*!
    *
    */
  unsigned char cannyLowThreshold;
   //!
   /*!
    *
    */
  unsigned char cannyHighThreshold;

  // Deep of the center between two cameras
  float centerDeep;

   //!
   /*!
    *
    */
  double posX;
   //!
   /*!
    *
    */
  double posY;
   //!
   /*!
    *
    */
  double posZ;
   //!
   /*!
    *
    */
  double posTh;

   //!
   /*!
    *
    */
  void extractPoints(short nImg = L_IMG);

   //!
   /*!
    *
    */
  int matchpoint(const CByteImage *pLeftImage,
	      const CByteImage *pRightImage,
	      int x, int y, int nWindowSize, int d1, int d2,
	      Vec2d &result, Vec3d &result_3d,
	      float fThreshold, bool bInputImagesAreUndistorted = false);

   //!
   /*!
    *
    */
  int SingleZNCC(const CByteImage *pInputImage1,
		   const CByteImage *pInputImage2,
		   int x, int y, int nWindowSize,
		   int d1, int d2, float *values);

  public:

   bool paperMode;
   //!
   /*!
    *
    */
   Vec3dList vP3D;
   //!
   /*!
    *
    */
   Vec2dList vP2D[IMAGES_NUMBER];

   //!
   /*!
    *
    */
   std::vector<unsigned char> vLineValues;

   //!
   /*!
    *
    */
   FindAndBuild();

   //!
   /*!
    *
    */
   FindAndBuild(CByteImage *leftImage, CByteImage *rightImage,
		short winSize = 21,
		short nDispThreshold = 250,
		float fMatchThreshold = 0.1,
		bool bInputImagesAreUndistorted = false);

   //!
   /*!
    *
    */
   ~FindAndBuild();

   //!
   /*!
    *
    */
   void setImg(CByteImage *image = 0, short nImage = L_IMG);

   //!
   /*!
    *
    */
   void setParam(short winSize = 21, short nDispThreshold = 250,
        float fMatchThreshold = 0.1f, bool bInputImagesAreUndistorted = false);

   //!
   /*!
    *
    */
   void setCanny(unsigned char nLowThreshold = 10,
	    unsigned char nHighThreshold = 80);

   //!
   /*!
    *
    */
   void setStereoCalibration(CStereoCalibration *stCal);

   //!
   /*!
    *
    */
   short find(Landmark *lm, short nImg = L_IMG);

   //!
   /*!
    *
    */
   bool build(Landmark *landmark);

   //!
   /*!
    *
    */
   void showPoints(CByteImage *img, short nImg, short r=255, short g=0, short b=255);
   void clearRegion(CByteImage *img, short nImg, short color=255);

   //!
   /*!
    *
    */
   void moveTo(double nX, double nY, double nZ, double nTh);
   //!
   /*!
    *
    */
   void moveRel(double nX, double nY, double nZ, double nTh);
};

#endif

