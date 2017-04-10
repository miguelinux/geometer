/*! \file cameramacros.h
 *   This is the header file where is defined the macros used to
 *   define a special camera.
 *   <h1> Camera macros </h1>
 *   <p>Declare one of this macros to use the camera you want.
 *   <lu>
 *     <li><b>USE_OPENCV</b>: to use OpenCV
 *     <li><b>USE_SVS</b>: to use SVS
 *     <li><b>USE_DFLY</b>: to use DragonFly
 *     <li><b>USE_BITMAP</b>: to use a Bitmap Image
 *   </lu><hr>
 *   The macros are:
 *   <lu>
 *     <li><b>CAPTURE_CLASS</b>: Is the class to be used in
 *        the application.
 *     <li><b>CAPTURE_PARAMETERS</b>: Are the parameters of
 *        the constructor of the camera object.
 *   </lu>
 *   </p>
 */
#ifndef _CAMERA_MACROS_H_
#define _CAMERA_MACROS_H_

/*! \def CAPTURE_CLASS
 *  Is the class of the camerea object.
 */
//#define CAPTURE_CLASS

/*! \def CAPTURE_PARAMETERS
 *  Are the parameters of the camera object.
 */
//#define CAPTURE_PARAMETERS

#ifdef WIN32

 #ifdef USE_OPENCV
  #include "VideoCapture/OpenCVCapture.h"
  #define CAPTURE_CLASS COpenCVCapture
  #define CAPTURE_PARAMETERS -1
 #endif

 #ifdef USE_SVS
  #include "VideoCapture/SVSCapture.h"
  #define CAPTURE_CLASS CSVSCapture 
  #define CAPTURE_PARAMETERS CVideoCaptureInterface::e640x480
 #endif

 #ifdef USE_DFLY
  #include "VideoCapture/DragonFlyCapture.h"
  #define CAPTURE_CLASS CDragonFlyCapture
  #define CAPTURE_PARAMETERS CVideoCaptureInterface::e640x480,CVideoCaptureInterface::eRGB24,ImageProcessor::eBayerGR
 #endif

 #ifdef USE_BITMAP
  #include "VideoCapture/BitmapCapture.h"
  #define CAPTURE_CLASS CBitmapCapture
  #define CAPTURE_PARAMETERS "noImage"
 #endif

 #ifdef USE_KINECT
  #include "rgbd/KinectCapture.h"
  #define CAPTURE_CLASS CKinectCapture
  //#define CAPTURE_PARAMETERS "C:\\kinect\\oni\\rampa.oni"
  #define CAPTURE_PARAMETERS "C:\\kinect\\oni\\Captured1.oni"
  //#define CAPTURE_PARAMETERS "C:\\kinect\\OpenNI\\Data\\amros.xml"
 #endif

#else /* WIN32 */

 #ifdef USE_SVS
  #include "VideoCapture/SVSCapture.h"
  #define CAPTURE_CLASS CSVSCapture 
  #define CAPTURE_PARAMETERS CVideoCaptureInterface::e640x480
  //#define CAPTURE_PARAMETERS CVideoCaptureInterface::e320x240
 #else
  #include "VideoCapture/Linux1394Capture2.h"
  #define CAPTURE_CLASS CLinux1394Capture2
  //#define CAPTURE_PARAMETERS -1,CVideoCaptureInterface::e640x480,CVideoCaptureInterface::eBayerPatternToRGB24
  //#define CAPTURE_PARAMETERS -1,CVideoCaptureInterface::e640x480,CByteImage::eRGB24
  //#define CAPTURE_PARAMETERS 2,CVideoCaptureInterface::e640x480,30.0f,192,144
  #define CAPTURE_PARAMETERS 2,CVideoCaptureInterface::e640x480,30.0f,0,0,640,480,CVideoCaptureInterface::eBayerPatternToRGB24,ImageProcessor::eBayerGR

 #endif /* USE_SVS */

#endif /* WIN32 */

#endif /* _CAMERA_MACROS_H_ */

