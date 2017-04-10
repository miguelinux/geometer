/*! \file mainwindow.cpp
 *   This is where the MainWindow class is implemented.
 */

#include <qdatetime.h>

#include "mainwindow.h"

MainWindow::MainWindow( QWidget* parent )
   : QWidget( parent ),
     lblX(tr("X"),&wGLTools), lblY(tr("Y"),&wGLTools), lblZ(tr("Z"),&wGLTools)
{

  // Create the Menu bar and its menus
  createMenus();

  // Create the images to display
  imgLeft  = new QtWImage(DSP_IMG_WIDTH,DSP_IMG_HEIGHT,&wEImg[0],this);
  imgRight = new QtWImage(DSP_IMG_WIDTH,DSP_IMG_HEIGHT,&wEImg[1],this);

  // An alias to the images
  imgs[0] = imgLeft;
  imgs[1] = imgRight;

  // Asigning the Window Event to the images
  wEImg[0].setImage(DSP_IMG_LEFT);
  wEImg[1].setImage(DSP_IMG_RIGHT);

  // Create the OpenGL widget
  glBox = new GLBox(this);

  // Create the output text window
  txtEdit = new QTextEdit(this);
  txtEdit->setMaximumHeight(75);
  txtEdit->setReadOnly(true);

  // Create the sliders
  sliderXRot = createSlider(&wGLTools);
  sliderYRot = createSlider(&wGLTools);
  sliderZRot = createSlider(&wGLTools);

  // Create the spinboxes
  sbXTrans = createSpinBox(&wGLTools);
  sbYTrans = createSpinBox(&wGLTools);
  sbZTrans = createSpinBox(&wGLTools);

  // Connect the valueChanged signal in slider to the setRotation in glBox.
  // When the valueChanged function is loaded it load the setRotation
  // in glBox with the same parameter.
  QObject::connect( sliderXRot, SIGNAL(valueChanged(int)),
		    glBox,SLOT(setXRotation(int)) );

  QObject::connect( sliderYRot, SIGNAL(valueChanged(int)),
		    glBox,SLOT(setYRotation(int)) );

  QObject::connect( sliderZRot, SIGNAL(valueChanged(int)),
		    glBox,SLOT(setZRotation(int)) );

  // Connect SpinButton now
  QObject::connect( sbXTrans, SIGNAL(valueChanged(int)),
		    glBox,SLOT(setXTranslation(int)) );

  QObject::connect( sbYTrans, SIGNAL(valueChanged(int)),
		    glBox,SLOT(setYTranslation(int)) );

  QObject::connect( sbZTrans, SIGNAL(valueChanged(int)),
		    glBox,SLOT(setZTranslation(int)) );

  // Conect the xRotationChanged to the slider
  // When in the OpenGL Box change its view, this connection
  // update the values in the sliders.
  QObject::connect( glBox, SIGNAL(xRotationChanged(int)),
		    sliderXRot,SLOT(setValue(int)) );

  QObject::connect( glBox, SIGNAL(yRotationChanged(int)),
		    sliderYRot,SLOT(setValue(int)) );

  QObject::connect( glBox, SIGNAL(zRotationChanged(int)),
		    sliderZRot,SLOT(setValue(int)) );

  // Connect to SpinButton now
  QObject::connect( glBox, SIGNAL(xTranslationChanged(int)),
		    sbXTrans,SLOT(setValue(int)) );

  QObject::connect( glBox, SIGNAL(yTranslationChanged(int)),
		    sbYTrans,SLOT(setValue(int)) );

  QObject::connect( glBox, SIGNAL(zTranslationChanged(int)),
		    sbZTrans,SLOT(setValue(int)) );

  // Connect to double click mouse
  QObject::connect( glBox, SIGNAL(mouseDblClick()),
		    &wGLTools,SLOT(show()) );

  //Initialize the sliders
  sliderXRot->setValue(0);
  sliderYRot->setValue(0);
  sliderZRot->setValue(0);

  // Put the name to the window title
  #if QT_VERSION >= 0x040000
  setWindowTitle( tr("GEOMETER " PGR_VER_STR) );
  wGLTools.setWindowTitle( tr("OpenGL Tools") );
  #else
  setCaption( tr("GEOMETER " PGR_VER_STR) );
  wGLTools.setCaption( tr("OpenGL Tools") );
  #endif

  hBoxImages = new QHBoxLayout();
  gLayoutGLCtl = new QGridLayout(&wGLTools);
  vBoxMain = new QVBoxLayout(this);

  hBoxImages->setSpacing(10);
  vBoxMain->setSpacing(10);

  hBoxImages->addWidget( imgLeft );
  hBoxImages->addWidget( imgRight );

  gLayoutGLCtl->addWidget( sliderXRot,0,0);
  gLayoutGLCtl->addWidget( sliderYRot,0,1);
  gLayoutGLCtl->addWidget( sliderZRot,0,2);

  gLayoutGLCtl->addWidget(&lblX,1,0);
  gLayoutGLCtl->addWidget(&lblY,1,1);
  gLayoutGLCtl->addWidget(&lblZ,1,2);

  gLayoutGLCtl->addWidget( sbXTrans,2,0);
  gLayoutGLCtl->addWidget( sbYTrans,2,1);
  gLayoutGLCtl->addWidget( sbZTrans,2,2);

  vBoxMain->setMenuBar( menuBar );
  vBoxMain->addLayout( hBoxImages );
  vBoxMain->addWidget( glBox );
  vBoxMain->addWidget( txtEdit );

  //setLayout(vBoxMain);
  //wGLTools.setLayout(gLayoutGLCtl);

  cfg = COLOR_IMAGE;
}


MainWindow::~MainWindow() {

  delete hBoxImages;
  delete vBoxMain;

  delete glBox;

  delete sliderXRot;
  delete sliderYRot;
  delete sliderZRot;

  delete sbXTrans;
  delete sbYTrans;
  delete sbZTrans;

  delete gLayoutGLCtl;

  delete imgLeft;
  delete imgRight;

  delete exitAct;
  delete openCloseCameraAct;
  delete startStopCaptureAct;
  delete saveImgSeqAct;
  delete loadImgSeqAct;
  delete loadImageAct;
  delete loadCamParamAct;
  delete reloadConfigAct;
  delete harrisAct;

  for(short i=0;i<NUM_MENU_ACT;i++) {
    delete arrayAct[i];
  }

  delete menuFile;
  delete menuCamera;
  delete menuImages;
  delete menuFilters;
  delete menuActions;
  delete menuRobot;
  delete menuBar;
}

QSlider *MainWindow::createSlider(QWidget *parent) {

  // Create the slider and put the marks in right place
  QSlider *slider = new QSlider(Qt::Vertical, parent);

  #if QT_VERSION >= 0x040000
  slider->setTickPosition(QSlider::TicksRight);
  #else
  slider->setTickmarks( QSlider::Right );
  #endif

  slider->setRange(0, 360);
  //slider->setSingleStep(16);
  slider->setPageStep(15);
  slider->setTickInterval(15);
  return slider;
}

QSpinBox *MainWindow::createSpinBox(QWidget *parent) {

  // Create the spinbox and configure it
  QSpinBox *spinBox = new QSpinBox(parent);

  #if QT_VERSION >= 0x040000
  // Warrning: only in 32bit system
  spinBox->setMaximum(0x7FFFFFFF);
  spinBox->setMinimum(0x80000000);
  #else
  // Warrning: only in 32bit system
  spinBox->setMaxValue(0x7FFFFFFF);
  spinBox->setMinValue(0x80000000);
  #endif

  return spinBox;
}

void MainWindow::message(const char *msg) {
  txtEdit->append( QTime::currentTime().toString(tr("hh:mm:ss.zzz")) +
		  tr("   ") + tr( msg));
}

void MainWindow::warning(const char *msg) {
  #if QT_VERSION >= 0x040000
  txtEdit->setTextColor(QColor(255,0,0));
  #else
  txtEdit->setColor(QColor(255,0,0));
  #endif

  txtEdit->append( QTime::currentTime().toString(tr("hh:mm:ss.zzz")) +
		  tr("   ") + tr( msg));

  #if QT_VERSION >= 0x040000
  txtEdit->setTextColor(QColor(0,0,0));
  #else
  txtEdit->setColor(QColor(0,0,0));
  #endif
}

void MainWindow::createMenus() {

  // Create a menu bar
  menuBar = new QMenuBar( this );

  // Create actions of menus and configure it
  exitAct = new QtAction(tr("E&xit"),this, Qt::CTRL+Qt::Key_W);
  #if QT_VERSION >= 0x040000
   connect(exitAct, SIGNAL(triggered()), this, SLOT(exit()));
  #else
   connect(exitAct, SIGNAL(activated()), this, SLOT(exit()));
  #endif

  reloadConfigAct = new QtAction(tr("Reload configuration"),this);
  reloadConfigAct->setParameter(LOAD_CONFIG);
  connect(reloadConfigAct, SIGNAL(someAction(unsigned int)),
		  this, SLOT(setCfg(unsigned int)));

  openCloseCameraAct = new QtAction(tr("Open Camera"),this);
  openCloseCameraAct ->setParameter(OPEN_CAMERA);
  connect(openCloseCameraAct , SIGNAL(someAction(unsigned int)),
		  this, SLOT(setCfg(unsigned int)));

  startStopCaptureAct = new QtAction(tr("Start Capture"),this);
  startStopCaptureAct->setParameter(CAPTURE_IMG);
  connect(startStopCaptureAct, SIGNAL(someAction(unsigned int)),
		  this, SLOT(setCfg(unsigned int)));

  saveImgSeqAct = new QtAction(tr("Save Image Sequence"),this, Qt::CTRL+Qt::Key_V);
  saveImgSeqAct->setParameter(SAVE_IMG_SEQ);
  connect(saveImgSeqAct, SIGNAL(someAction(unsigned int)),
		  this, SLOT(setCfg(unsigned int)));

  loadImgSeqAct = new QtAction(tr("Load Image Sequence"),this);
  loadImgSeqAct->setParameter(LOAD_IMG_SEQ);
  connect(loadImgSeqAct, SIGNAL(someAction(unsigned int)),
		  this, SLOT(setCfg(unsigned int)));

  loadImageAct = new QtAction(tr("Load single Image"),this);
  loadImageAct->setParameter(LOAD_IMAGE);
  connect(loadImageAct, SIGNAL(someAction(unsigned int)),
		  this, SLOT(setCfg(unsigned int)));

  loadCamParamAct = new QtAction(tr("Load Camera parameters"),this);
  loadCamParamAct->setParameter(CAMERA_PARAM);
  connect(loadCamParamAct, SIGNAL(someAction(unsigned int)),
		  this, SLOT(setCfg(unsigned int)));

  harrisAct = new QtAction(tr("Apply Harris"),this);
  harrisAct->setParameter(HARRIS_FILTER);
  connect(harrisAct, SIGNAL(someAction(unsigned int)),
		  this, SLOT(setCfg(unsigned int)));

  arrayAct[0]= new QtAction(tr("Stereo click 3D"),this);
  arrayAct[0]->setParameter(CLICK_3D_ACT);
  connect(arrayAct[0], SIGNAL(someAction(unsigned int)),
		  this, SLOT(setCfg(unsigned int)));

  arrayAct[1]= new QtAction(tr("Apply Canny"),this);
  arrayAct[1]->setParameter(CANNY_FILTER);
  connect(arrayAct[1], SIGNAL(someAction(unsigned int)),
		  this, SLOT(setCfg(unsigned int)));

  arrayAct[2]= new QtAction(tr("Stereo matcher"),this);
  arrayAct[2]->setParameter(CLICK_MATCH);
  connect(arrayAct[2], SIGNAL(someAction(unsigned int)),
		  this, SLOT(setCfg(unsigned int)));

  arrayAct[3]= new QtAction(tr("Pose procedure"),this);
  arrayAct[3]->setParameter(POSE_PROC);
  connect(arrayAct[3], SIGNAL(someAction(unsigned int)),
		  this, SLOT(setCfg(unsigned int)));

  arrayAct[4]= new QtAction(tr("Connect to Robot"),this);
  arrayAct[4]->setParameter(ROBOT_CONNECT);
  connect(arrayAct[4], SIGNAL(someAction(unsigned int)),
		  this, SLOT(setCfg(unsigned int)));

  arrayAct[5]= new QtAction(tr("STOP MOVEMENT"),this, Qt::Key_Escape);
  arrayAct[5]->setParameter(ROBOT_STOP);
  connect(arrayAct[5], SIGNAL(someAction(unsigned int)),
		  this, SLOT(setCfg(unsigned int)));

  arrayAct[6]= new QtAction(tr("Get laser readings"),this);
  arrayAct[6]->setParameter(ROBOT_LASER);
  connect(arrayAct[6], SIGNAL(someAction(unsigned int)),
		  this, SLOT(setCfg(unsigned int)));

  arrayAct[7]= new QtAction(tr("Print robot status"),this, Qt::CTRL+Qt::Key_A);
  arrayAct[7]->setParameter(ROBOT_INFO);
  connect(arrayAct[7], SIGNAL(someAction(unsigned int)),
		  this, SLOT(setCfg(unsigned int)));

  arrayAct[8]= new QtAction(tr("Connect to Geometer"),this);
  arrayAct[8]->setParameter(GEOMETER_CONNECT);
  connect(arrayAct[8], SIGNAL(someAction(unsigned int)),
		  this, SLOT(setCfg(unsigned int)));

  arrayAct[9]= new QtAction(tr("Connect laser in Geometer"),this);
  arrayAct[9]->setParameter(GEOMETER_LASER);
  connect(arrayAct[9], SIGNAL(someAction(unsigned int)),
		  this, SLOT(setCfg(unsigned int)));

  arrayAct[10]= new QtAction(tr("Get laser readings in Geometer"),this);
  arrayAct[10]->setParameter(GEOMETER_GET_LASER);
  connect(arrayAct[10], SIGNAL(someAction(unsigned int)),
		  this, SLOT(setCfg(unsigned int)));

  arrayAct[11]= new QtAction(tr("glKinect"),this, Qt::CTRL+Qt::Key_K);
  arrayAct[11]->setParameter(GL_KINECT);
  connect(arrayAct[11], SIGNAL(someAction(unsigned int)),
		  this, SLOT(setCfg(unsigned int)));

  // Asign menus and its actions
  #if QT_VERSION >= 0x040000
  menuFile = menuBar->addMenu( tr("&File") );
  menuFile->addAction(reloadConfigAct);
  menuFile->addAction(exitAct);
  #else
  menuFile = new QTX_MENU( this );
  reloadConfigAct->addTo(menuFile);
  exitAct->addTo(menuFile);

  menuBar->insertItem(tr("&File"), menuFile);
  #endif

  #if QT_VERSION >= 0x040000
  menuCamera = menuBar->addMenu( tr("&Camera") );
  menuCamera->addAction(loadCamParamAct);
  menuCamera->addAction(openCloseCameraAct);
  menuCamera->addAction(startStopCaptureAct);
  #else
  menuCamera =  new QTX_MENU( this );
  loadCamParamAct->addTo(menuCamera);
  openCloseCameraAct->addTo(menuCamera);
  startStopCaptureAct->addTo(menuCamera);

  menuBar->insertItem(tr("&Camera"), menuCamera);
  #endif

  #if QT_VERSION >= 0x040000
  menuImages = menuBar->addMenu( tr("&Images") );
  menuImages->addAction(saveImgSeqAct);
  menuImages->addAction(loadImgSeqAct);
  menuImages->addAction(loadImageAct);
  #else
  menuImages =  new QTX_MENU( this );
  saveImgSeqAct->addTo(menuImages);
  loadImgSeqAct->addTo(menuImages);
  loadImageAct->addTo(menuImages);

  menuBar->insertItem(tr("&Images"), menuImages);
  #endif

  #if QT_VERSION >= 0x040000
  menuFilters = menuBar->addMenu( tr("Fil&ters") );
  menuFilters->addAction(harrisAct);
  menuFilters->addAction(arrayAct[1]);
  //menuFilters->addAction();
  #else
  menuFilters =  new QTX_MENU( this );
  harrisAct->addTo(menuFilters);
  arrayAct[1]->addTo(menuFilters);
  //->addTo(menuFilters);

  menuBar->insertItem(tr("Fil&ters"), menuFilters);
  #endif

  #if QT_VERSION >= 0x040000
  menuActions = menuBar->addMenu( tr("&Actions") );
  menuActions->addAction(arrayAct[0]);
  menuActions->addAction(arrayAct[2]);
  menuActions->addAction(arrayAct[3]);
  menuActions->addAction(arrayAct[11]);
  //menuActions->addAction();
  #else
  menuActions =  new QTX_MENU( this );
  arrayAct[0]->addTo(menuActions);
  arrayAct[2]->addTo(menuActions);
  arrayAct[3]->addTo(menuActions);
  arrayAct[11]->addTo(menuActions);
  //->addTo(menuActions);

  menuBar->insertItem(tr("&Actions"), menuActions);
  #endif

  #if QT_VERSION >= 0x040000
  menuRobot = menuBar->addMenu( tr("&Robot") );
  menuRobot->addAction(arrayAct[8]);
  menuRobot->addAction(arrayAct[9]);
  menuRobot->addAction(arrayAct[10]);
  menuRobot->addAction(arrayAct[4]);
  menuRobot->addAction(arrayAct[6]);
  menuRobot->addAction(arrayAct[5]);
  menuRobot->addAction(arrayAct[7]);
  //menuRobot->addAction();
  #else
  menuRobot =  new QTX_MENU( this );
  arrayAct[8]->addTo(menuRobot);
  arrayAct[9]->addTo(menuRobot);
  arrayAct[10]->addTo(menuRobot);
  arrayAct[4]->addTo(menuRobot);
  arrayAct[6]->addTo(menuRobot);
  arrayAct[5]->addTo(menuRobot);
  arrayAct[7]->addTo(menuRobot);
  //->addTo(menuRobot);

  menuBar->insertItem(tr("&Robot"), menuRobot);
  #endif

}

unsigned int *MainWindow::getCfg() {
  return &cfg;
}

void MainWindow::exit() {
  cfg &= ~CAPTURE_IMG; // stop capturing
  cfg &= ~OPEN_CAMERA; // closing the camera
  hide(); // hide the window
}

void MainWindow::setCfg(unsigned int conf) {
  switch (conf){
    case OPEN_CAMERA:
	      if (cfg & conf) {
                openCloseCameraAct->setText(tr("Open Camera"));
  		startStopCaptureAct->setText(tr("Start Capture"));
		cfg &= ~(OPEN_CAMERA | CAPTURE_IMG);
		message("Closing cameras.");
   		emit confChanged( ~OPEN_CAMERA  );
	      }
	      else {
                openCloseCameraAct->setText(tr("Close Camera"));
		cfg |= OPEN_CAMERA;
		message("Opening cameras.");
   		emit confChanged(OPEN_CAMERA);
	      }
	    break;
    case ~OPEN_CAMERA:
                openCloseCameraAct->setText(tr("Open Camera"));
		cfg &= ~(OPEN_CAMERA | CAPTURE_IMG);
	    break;
    case CAPTURE_IMG:
	      if (cfg & OPEN_CAMERA) {
	        if (cfg & ~LOAD_IMG_SEQ) {
	          if (cfg & conf) {
		    cfg &= ~CAPTURE_IMG;
		    message("Stop capturing images.");
  		    startStopCaptureAct->setText(tr("Start Capture"));
   		    emit confChanged(~CAPTURE_IMG);
	          } else {
		    cfg |= CAPTURE_IMG;
		    message("Start to capture images.");
  		    startStopCaptureAct->setText(tr("Stop Capture"));
	          }  /* if (cfg & conf)  */
	        } else {
		  warning("Please stop the image sequence play before capture.");
	        }
	      } else {
		  warning("Camera is not open.");
	      }
	    break;
    case COLOR_IMAGE:
	      if (cfg & conf) {
		cfg &= ~COLOR_IMAGE;
		message("Changing to B/W images.");
	      }
	      else {
		cfg |= COLOR_IMAGE;
		message("Changing to Color images.");
	      }
	    break;
    case SAVE_IMG_SEQ:
	      if (cfg & (OPEN_CAMERA | CAPTURE_IMG)) {
	        if (cfg & conf) {
                  saveImgSeqAct->setText(tr("Save Image Sequence"));
		  cfg &= ~SAVE_IMG_SEQ;
		  message("Stop saving images.");
   		  emit confChanged( ~SAVE_IMG_SEQ  );
	        } else {
                  saveImgSeqAct->setText(tr("Stop image saving"));
		  cfg |= SAVE_IMG_SEQ;
		  message("Start to save images ...");
   		  emit confChanged(SAVE_IMG_SEQ);
	        }
	      } else {
		  warning("Camera is not capturing images.");
	      }
	    break;
    case LOAD_IMG_SEQ:
	      if (cfg & ~CAPTURE_IMG) {
	        if (cfg & conf) {
                  loadImgSeqAct->setText(tr("Load Image Sequence"));
		  cfg &= ~LOAD_IMG_SEQ;
		  message("Stop video playing.");
   		  emit confChanged( ~LOAD_IMG_SEQ  );
	        } else {
                  loadImgSeqAct->setText(tr("Stop video play"));
		  message("Loading and playing video ...");
		  cfg |= LOAD_IMG_SEQ;
   		  emit confChanged(LOAD_IMG_SEQ);
	        }
	      } else {
		  warning("Camera is capturing please stop capturing.");
	      }
	    break;
    case LOAD_IMAGE:
	      if (cfg & ~(CAPTURE_IMG | LOAD_IMG_SEQ)) {
		message("Loading image ...");
   		emit confChanged(LOAD_IMAGE);
	      } else {
		warning("Camera is capturing please stop capturing.");
	      }
	    break;
    case LOAD_CONFIG:
	      message("Loading configuration file...");
   	      emit confChanged(LOAD_CONFIG);
	    break;
    case CAMERA_PARAM:
	      cfg |= CAMERA_PARAM;
   	      emit confChanged(CAMERA_PARAM);
	    break;
    case HARRIS_FILTER:
	      if (cfg & conf) {
                harrisAct->setText(tr("Apply Harris"));
		cfg &= ~HARRIS_FILTER;
		message("Stop applying Harris filter.");
	      } else {
                harrisAct->setText(tr("Stop Harris"));
		message("Applying Harris filter ...");
		cfg |= HARRIS_FILTER;
	      }
	    break;
    case CANNY_FILTER:
	      if (cfg & conf) {
                arrayAct[1]->setText(tr("Apply Canny"));
		cfg &= ~CANNY_FILTER;
		message("Stop applying canny filter");
	      } else {
                arrayAct[1]->setText(tr("Stop Apply Canny"));
		message("Start to apply canny filter");
		cfg |= CANNY_FILTER;
	      }
	    break;
    case POSE_PROC:
	      if (cfg & conf) {
                arrayAct[3]->setText(tr("Pose procedure"));
		cfg &= ~POSE_PROC;
		message("Stop applying pose procedure");
	      } else {
                arrayAct[3]->setText(tr("Stop Pose procedure"));
		message("Start to apply pose procedure");
		cfg |= POSE_PROC;
   	        emit confChanged(POSE_PROC);
	      }
	    break;
    case CLICK_3D_ACT:
	      if (cfg & conf) {
                arrayAct[0]->setText(tr("Stereo click 3D"));
		cfg &= ~CLICK_3D_ACT;
		message("Stop stereo click 3D");
	      } else {
                arrayAct[0]->setText(tr("** NO Stereo click 3D **"));
		message("Click on the left and right image to get a 3D point.");
		cfg |= CLICK_3D_ACT;
	      }
	    break;
    case CLICK_MATCH:
	      if (cfg & conf) {
                arrayAct[2]->setText(tr("Stereo matcher"));
		cfg &= ~CLICK_MATCH;
		message("Stop stereo matcher 3D");
	      } else {
                arrayAct[2]->setText(tr("** NO Stereo matcher**"));
		message("Click only on the left image to get a 3D point.");
		cfg |= CLICK_MATCH;
	      }
	    break;
    case GEOMETER_CONNECT:
	      message("Connecting to geometer ...");
   	      emit confChanged(GEOMETER_CONNECT);
	   break;
    case GEOMETER_LASER:
	      message("Connecting laser in geometer ...");
   	      emit confChanged(GEOMETER_LASER);
	   break;
    case GEOMETER_GET_LASER:
	      if (cfg & conf) {
                arrayAct[10]->setText(tr("Get Laser readings from geometer"));
		cfg &= ~GEOMETER_GET_LASER;
		message("Stop laser readings from geometer");
	      } else {
                arrayAct[10]->setText(tr("Stop Laser readings from geometer"));
		message("Start to get laser readings from geometer");
		cfg |= GEOMETER_GET_LASER;
	      }
	    break;
    case ROBOT_CONNECT:
	      message("Connecting to robot ...");
   	      emit confChanged(ROBOT_CONNECT);
	   break;
    case ROBOT_STOP:
   	      emit confChanged(ROBOT_STOP);
	    break;
    case ROBOT_INFO:
   	      emit confChanged(ROBOT_INFO);
	    break;
    case ROBOT_LASER:
	      if (cfg & conf) {
                arrayAct[6]->setText(tr("Get Laser readings"));
		cfg &= ~ROBOT_LASER;
		message("Stop laser readings");
	      } else {
                arrayAct[6]->setText(tr("Stop Laser readings"));
		message("Start to get laser readings");
		cfg |= ROBOT_LASER;
	      }
	    break;
    case GL_KINECT:
   	      emit confChanged(GL_KINECT);
	    break;

  } /* switch (conf) */
}

void MainWindow::keyPressEvent ( QKeyEvent * e) {

  switch( e->key() ) {
    case Qt::Key_S : // LEFT
      emit confChanged(ROBOT_LEFT);
    break;
    case Qt::Key_E: // UP
      emit confChanged(ROBOT_UP);
    break;
    case Qt::Key_F: // RIGHT
      emit confChanged(ROBOT_RIGHT);
    break;
    case Qt::Key_D: // DOWN
      emit confChanged(ROBOT_DOWN);
    break;
    case Qt::Key_T: // Increment velocity
      emit confChanged(ROBOT_SPEED_UP);
    break;
    case Qt::Key_G: // Decrement velocity
      emit confChanged(ROBOT_SPEED_DOWN);
    break;
    case Qt::Key_R: // Laser: points to line
      emit confChanged(LASER_P2L);
    break;
    case Qt::Key_1: emit confChanged(K_UNO); break;
    case Qt::Key_2: emit confChanged(K_DOS); break;
    case Qt::Key_3: emit confChanged(K_TRES); break;
    case Qt::Key_4: emit confChanged(K_CUAT); break;
    case Qt::Key_5: emit confChanged(K_CINC); break;
    case Qt::Key_6: emit confChanged(K_SEIS); break;
  }
}

