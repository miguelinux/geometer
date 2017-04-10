/*! \file mainwindow.h
 *   This is the header file where is defined the class MainWindow
 */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <qslider.h>
#include <qlayout.h>
#include <qwidget.h>
#include <qmenubar.h>
#include <qtextedit.h>
#include <qspinbox.h> 
#include <qlabel.h> 

#if QT_VERSION < 0x040000
  #include <qkeycode.h>
  #include <qpopupmenu.h>
#endif

#include "qtaction.h"
#include "qtwimage.h"

#include "glbox.h"
#include "wedimage.h"
#include "commonvars.h"
#include "qt3qt4macros.h"

/*! \def NUM_MENU_ACT
 *  Defined the number of menus in the array variable arrayAct
 *  \sa arrayAct
 */
#define NUM_MENU_ACT 12

//!  MainWindow class, the main window of the program. 
/*!
 * This is the main window where the images of the camera and
 * the 3D virtual world are seen.
 */
class MainWindow : public QWidget
{
    Q_OBJECT

 private:
   //! MenuBar object.
   /*! This object is used to build the menubar into the main window.
    *  \sa menuFile, menuCamera
    */
   QMenuBar *menuBar;
   //! Menu File object.
   /*! This object is used to build the menu File into the menubar.
    *  \sa qt3qt4macros.h, menuBar
    */
   QTX_MENU *menuFile;
   //! Menu Camera object.
   /*! This object is used to build the menu Camera into the menubar.
    *  \sa qt3qt4macros.h, menuBar
    */
   QTX_MENU *menuCamera;

   QTX_MENU *menuImages;

   QTX_MENU *menuFilters;

   QTX_MENU *menuActions;

   QTX_MENU *menuRobot;

   QtAction *exitAct;
   QtAction *openCloseCameraAct;
   QtAction *startStopCaptureAct;
   QtAction *saveImgSeqAct;
   QtAction *loadImgSeqAct;
   QtAction *loadImageAct;
   QtAction *loadCamParamAct;
   QtAction *reloadConfigAct;
   QtAction *harrisAct;

   QtAction *arrayAct[NUM_MENU_ACT];


   //! Layout of the imeges.
   QHBoxLayout *hBoxImages;
   //! A window with the OpenGL tools.
   QWidget wGLTools;
   //! Layout of the OpenGL controls.
   QGridLayout *gLayoutGLCtl;
   //! Main layout of the main window.
   QVBoxLayout *vBoxMain;

   //! Slider for the X rotation axis.
   QSlider *sliderXRot;
   //! Slider for the Y rotation axis.
   QSlider *sliderYRot;
   //! Slider for the Z rotation axis.
   QSlider *sliderZRot;

   QSpinBox *sbXTrans;
   QSpinBox *sbYTrans;
   QSpinBox *sbZTrans;

   QLabel lblX;
   QLabel lblY;
   QLabel lblZ;

   //! Is an output text window.
   /*! Here will be displayed the messages of the program.
    * \sa message(), warning()
    */
   QTextEdit *txtEdit;

   //! Is the configuration state.
   /*! Here es where are placed the flags of configuration of
    *  the main window.
    *  \sa getCfg(), setCfg()
    */
   unsigned int cfg;


   //! Crete the slider.
   /*! Setup the slider with inital values
    *  \return the new slider object
    *  \sa sliderXRot, sliderYRot, sliderZRot
    */
   QSlider *createSlider(QWidget *parent); 

   QSpinBox *createSpinBox(QWidget *parent); 


   void createMenus();


 public:
   //! The constructor of the MainWindow class.
   /*!
    *  Here is where the values of variables are initialized.
    */
   MainWindow( QWidget* parent = 0);
   //! The destructor of the MainWindow class.
   /*!
    *  Here is where all the resources are cleaned.
    */
   ~MainWindow();

   //! The OpenGL widget.
   /*!
    *  This is the object where the 3D virutal enviroment will be displayed.
    */
   GLBox *glBox;

   //! Image from the left camera.
   /*! This is where the image will be displayed.
    *  The image is 320x240 size.
    */
   QtWImage *imgLeft;
   //! Image from the right camera.
   /*! This is where the image will be displayed.
    *  The image is 320x240 size.
    */
   QtWImage *imgRight;

   QtWImage *imgs[2];

   //! Send message to the output window.
   /*! Send the message to the output window in
    *  the main window.
    *  \param msg is the message to be displayed.
    *  \sa txtEdit, warning()
    */
   void message(const char *msg);
   //! Send warning message to the output window.
   /*! Send the warning message to the output window in
    *  the main window. This message is font color red.
    *  \param msg is the message to be displayed.
    *  \sa txtEdit, message()
    */
   void warning(const char *msg);

   //! Get the configuration parameters
   /*! Return the configuration value.
    *  \return the value of the configuration 
    *  \sa cfg
    */
   unsigned int *getCfg();

   void keyPressEvent ( QKeyEvent * e);

   WEDImage wEImg[2];

 public slots:

   //! Hide the main window
   /*! Hide the main window and set the configuration variable to
    *  stop the camera capture.
    */
   void exit();

   void setCfg(unsigned int conf);

 signals:

   void confChanged(unsigned int cnf);

};

#endif // MAINWINDOW_H

