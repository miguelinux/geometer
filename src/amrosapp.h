/*! \file amrosapp.h
 *   This is the header file where is defined the class AmrosApp.
 */
#ifndef _AMROS_APP_IITB_H
#define _AMROS_APP_IITB_H

/** Linux problem: include this library as first **/
#include <fstream>
#include <complex>

#include "geometerclient.h"
#include "geometer.h"

/** Windows problem: include this library as second **/
#include "fiitb/aria_client.h"


#include <qmessagebox.h>
#include <fstream>
#include "il/point3.h"

#include "VideoCapture/BitmapSequenceCapture.h"

#include "Calibration/Calibration.h"
#include "Calibration/StereoCalibration.h"

#include "gui/QTApplicationHandler.h"

#include "Image/PrimitivesDrawer.h"
#include "Image/StereoMatcher.h"
#include "Image/StereoVision.h"
#include "Image/ImageProcessorCV.h"

#include "Image/ImageProcessor.h"

#include "mainwindow.h"

#include "configprog.h"

#include "cameramacros.h"

#include "findandbuild.h"

#include "ivtprintf.h"

#include "mapbuilder.h"

#include "laserlog.h"

//! AmrosApp, is the main object of the project.
/*! In this class are declared all the objects necesary by the project.
 */
class AmrosApp : public QObject
 {

  Q_OBJECT

  private:

    il::Point3d il_p3d;

    //! The Qt application handler.
    /*! Here is where the event of the Qt widget are handler.
     *  This is an IVT Object.
     */
    CQTApplicationHandler qtApplicationHandler;
    //! The main window of the project.
    /*! Here is where the images and the virtual world will be
     *  seen.
     */
    MainWindow *mainWindow;

    //!
    /*!
     *
     */
    CStereoCalibration stereoCalib;

    //! The stream FILE output.
    /*! This will help us to redirect the printf function to a log file.
     * \sa streamIn, ConfigProg
     */
    FILE *streamOut;

    //! The stream FILE input.
    /*! This will help us to read the data from the log file.
     * \sa streamOut, ConfigProg
     */
    FILE *streamIn;

    //!
    /*!
     *
     */
    unsigned int *conf;
    unsigned int confK;

    //!
    /*!
     *
     */
    short nCameras;
    //!
    /*!
     *
     */
    int imgWidth;
    //!
    /*!
     *
     */
    int imgHeight;
    //!
    /*!
     *
     */
    CByteImage::ImageType imgType;

    //!
    /*!
     *
     */
    unsigned int imgSaveCount;

    //!
    /*!
     *
     */
    CByteImage **imagesCamera;

    //!
    /*!
     *
     */
    CByteImage **imagesDisplay;

    //!
    /*!
     *
     */
    CByteImage **imagesProcess;

    //!
    /*!
     *
     */
    QtWImage **wOImgs;

    //!
    /*!
     *
     */
    WEDImage wEOImg[2];

    //!
    /*!
     *
     */
    std::string  sSavePath[2];

    //!
    /*!
     *
     */
    unsigned int displayRate;

    //!
    /*!
     *
     */
    ConfigProg cfgPgr;

    //!
    /*!
     *
     */
    GLObjects glObjs;

    //!
    /*!
     *
     */
    FindAndBuild fab;

    //!
    /*!
     *
     */
    Landmark *landmarks[MAX_LANDMARKS_NUM];
    //!
    /*!
     *
     */
    unsigned short numLandmarks;

    //!
    /*!
     *
     */
    Vec2d vHarrisPoints[MAX_HARRIS_POINTS];
    //!
    /*!
     *
     */
    int harrisPFound[2];

    //!
    /*!
     *
     */
    Vec2d imgPoints[2];

    //! 3D point
    /*! Used to store the 3D point of a pair point in stereo images
     */
    Vec3d worldPoint;

    //!
    /*!
     *
     */
    MapBuilder  mb;

    //! Send the strOut content to display.
    /*! Display de content of strOut in the output window
     */
    void sendMsg();

    //! Setup the images
    /*! Setup the images to start the application.
     *  \param cap the capture image interface
     */
    void setupImages(CVideoCaptureInterface *cap);

    //! Perform one action to the images
    /*! Perform a proces to the actual image in the memory.
     */
    void processImages();

    void processKinectImg();

    //! Draw the small images in the main window
    /*! Draw the small
     *  \sa drawOrigImages()
     */
    void drawImages();

    //! Draw the images from the camera to a window
    /*! Draw the images (full size) to a window
     *  \sa drawImages()
     */
    void drawOrigImages();

    //! Save images sequence
    /*! Procedure that save the actual image in the memory.
     *  \sa imgSaveCount
     */
    void saveImages(bool bothImg = false);

    //! Perform an action from a event image
    /*! Take an action when a event in the images occurs
     *  \param evFlag Event on the image
     */
    void eventImages(unsigned int evFlag);

    //! Free the images memory
    void  freeImages();

    //! Setup the Original captured images 
    /*! Setup the Original captured images to show in its own 
     *  window
     *  \sa drawOrigImages()
     */
    void setupOImg();

    //! Draw an X in a image
    /*! Draw a small X in the image.
     *  \param pImage image to draw
     *  \param point to draw the X
     *  \param r intencity of the red color 0-255
     *  \param g intencity of the green color 0-255
     *  \param b intencity of the blue color 0-255
     */
    void DrawAnX(CByteImage *pImage, const Vec2d &point,
		short r, short g, short b);

    //! Match point in stereo camera
    /*! Match a point using  Single Zero Normalized mean Cross-Correlation
     *  \param pLeftImage left image
     *  \param pRightImage right image
     *  \param x coordiante point in left image
     *  \param y coordiante point in left image
     *  \param nWindowSize size of search window
     *  \param d1 minimal disparity threshold to start searching
     *  \param d2 maximal disparity threshold to stop searching
     *  \param result returned value of the point in the left image
     *  \param result_3d returned value of the point in 3D
     *  \param fThreshold for the minimal value of the maximum correlation value
     *  \param bInputImagesAreUndistorted true if the images are rectificated
     *  \sa SingleZNCC()
     *  \return the maximum correlation value or 0 in fail
     */
    int matchpoint(const CByteImage *pLeftImage,
		    const CByteImage *pRightImage,
	      int x, int y, int nWindowSize, int d1, int d2,
	      Vec2d &result, Vec3d &result_3d,
	      float fThreshold, bool bInputImagesAreUndistorted = false);

    //! Single Zero Normalized mean Cross-Correlation
    /*! Used to find the correspondence point in stereo images
     *  \param pInputImage1 left image
     *  \param pInputImage2 right image
     *  \param x coordiante point in left image
     *  \param y coordiante point in left image
     *  \param nWindowSize size of search window
     *  \param d1 minimal disparity threshold to start searching
     *  \param d2 maximal disparity threshold to stop searching
     *  \param values from the correlation between d1 and d2
     *  \sa matchpoint()
     *  \return the maximum correlation value or -1 in fail
     */
    int SingleZNCC(const CByteImage *pInputImage1,
		   const CByteImage *pInputImage2,
		   int x, int y, int nWindowSize,
		   int d1, int d2, float *values);


    Geometer *geometer;

    GeometerClient *gc;

    //! Robot controller
    /*! Object that control the robot
     */
    RobotController *rob;

    //! Robot's sensors
    /*! Struct that contain the robot's sensor
     */
    robSensorsStruct theSensors;

    //! Pose of robot
    /*! Store the pose of the robot
     */
    CMatrixT<double> theRobPose;

    //! Data from laser
    /*! Data from the laser rangefinder
     *  \sa laserDataP
     */
    CMatrixT<double> laserData;

    //! Pointer to the laser data
    /*! Point to the returned laser data when is called by
     * rob->robot_GetSensorCurrent(sensor);
     * laserDataP=theSensors.Data[sensor];
     * \sa laserData
     */
    CMatrixT<double> *laserDataP;

    //! Velocity of the robot.
    /*! Stores the velocity of the robot
     *  \sa MainWindow::keyPressEvent()
     */
    short robVel;

    //! 2D point + height
    /*! Used to store the 2D point of the laser + the hieght of
     *  laser rangefinder to be display in OpenGL
     */
    Vec3d laserP3d;

    //! Stores and load laser data in a file.
    /*! Record and load to a memory the values of the laser rangefinder.
     *  \sa run()
     */
    LaserLog lg;

    void runKinect();

    void runGeometerClient();

    void ktmp();

  public:

    //! The constructor of AmrosApp.
    /*! Here is where initialize the variables and setup 
     *  the project.
     *  \param argc is the arguments counter from the command line.
     *  \param argv is the arguments vector from the command line.
     */
    AmrosApp(int argc, char *argv[]);

    //! The destructor of AmrosApp.
    ~AmrosApp();

    //! The main loop of the project.
    /*! Here is implement the main loop of the project.
     *  \return an exit value code.
     */
    int run();

    //! The image capture object.
    /*! This is a macro of the class of VideoCapture object, previously
     *  defined by USE_XXX where XXX are the object to use.
     *  \sa cameramacros.h
     */
    CAPTURE_CLASS  *capture;

    //! The image sequence object.
    /*! This is used to load a images sequence.
     */
    CBitmapSequenceCapture  *imgCapture;

    std::ofstream myfile;


 public slots:
    
    //! Slot used to communicate to the GUI
    /*! This slot recives the signal that the GUI send and do an especific
     *  action with the parameter cfg
     *  \param cfg is the action to perform
     */
    void action(unsigned int cfg);

};

#endif

