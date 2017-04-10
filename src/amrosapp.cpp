/*! \file amrosapp.cpp
 *   This is file where AmrosApp class is implemented.
 */

#include <qfiledialog.h>

#include <float.h>
#include <math.h>
#include <cstring>

#include "amrosapp.h"

#if defined(Q_CC_MSVC)
#pragma warning(disable:4996) // Unsafe function (freopen,fopen,sprintf)
#endif

AmrosApp::AmrosApp(int argc, char *argv[])
         : qtApplicationHandler(argc,argv)
{
  qtApplicationHandler.Reset();

  mainWindow = new MainWindow();
  
  imgCapture = 0;

  conf = mainWindow->getCfg();

#ifdef USE_KINECT
  capture = new CAPTURE_CLASS ( cfgPgr.kf.c_str() );
#else
  capture = new CAPTURE_CLASS ( CAPTURE_PARAMETERS );
#endif

  imagesCamera  = 0;
  imagesDisplay = 0;
  imagesProcess  = 0;

  wOImgs = 0;

  nCameras  = 0;

  QObject::connect(mainWindow, SIGNAL(confChanged(unsigned int)),
	 this, SLOT(action(unsigned int)));

  QObject::connect(&mainWindow->wEImg[0],SIGNAL(sDoubleClick(unsigned int)),
	 this, SLOT(action(unsigned int)));

  QObject::connect(&mainWindow->wEImg[1],SIGNAL(sDoubleClick(unsigned int)),
	 this, SLOT(action(unsigned int)));

  QObject::connect(&wEOImg[0],SIGNAL(sClick(unsigned int)),
	 this, SLOT(action(unsigned int)));

  QObject::connect(&wEOImg[1],SIGNAL(sClick(unsigned int)),
	 this, SLOT(action(unsigned int)));

  // Delete previos log file
  remove(cfgPgr.strLogFile.c_str());

  // Open the stream to redirect printf to file named "amros.log"
  if((streamOut = freopen(cfgPgr.strLogFile.c_str(), "w", stdout)) == NULL) {
    fprintf(stderr,"could not redirect stdout\n");
  }
  // Open the file to read what it say.
  if((streamIn = fopen(cfgPgr.strLogFile.c_str(), "r")) == NULL) {
    fprintf(stderr,"could not open file %s\n",cfgPgr.strLogFile.c_str());
  }

  // to display the first image
  displayRate = 1;

  imgSaveCount = 0;

  mainWindow->glBox->addGLObjects(&glObjs);
  mainWindow->glBox->setFixedSize(650,300);
  mainWindow->glBox->reset();
  mainWindow->resize(670,670);
  mainWindow->show();

  glObjs.setConfig(GL_VIEW_POINTS | GL_VIEW_CAMERA_P | GL_VIEW_LANDMARKS);

  for(short l=0; l<MAX_LANDMARKS_NUM; l++) {
    landmarks[l]=0;
  }
  numLandmarks = 0;

  rob = 0;
  robVel = 200;

  geometer=0;
  gc = 0;

#ifdef WIN32
  myfile.open("c:\\puntos.csv");
#else
  myfile.open("/tmp/puntos.csv");
#endif
}

AmrosApp::~AmrosApp(){

  myfile.close();

  for(unsigned short l=0; l<numLandmarks; l++) {
    delete landmarks[l];
  }

  if (geometer) {
    delete geometer;
  }

  if (gc) {
    delete gc;
  }

  if (rob) {
    delete rob;
    Aria::shutdown();
  }

  delete mainWindow;
 
  delete capture;

  if (imgCapture) delete imgCapture;

  freeImages();

  // Close the streams
  fclose (streamOut);
  fclose (streamIn);

  // if one want to open the console (DOS) uncoment this line
  //streamOut = freopen("CON", "w", stdout);
 
  if (rob) {
    Aria::exit(0);
  }
}


void AmrosApp::runGeometerClient()
{
  short lds; // laser data size
  double *ld; // laser data
  if (gc && gc->isConnected()) {
    if (gc->getPose()) {
      glObjs.glGeometer.moveTo(-gc->getY(), 0.0f,
                    gc->getX(),
                   -gc->getTh());

      glObjs.moveTo( gc->getY(),
            -gc->getX(), 0.0f,
             gc->getTh() );
    }
    if (*conf & GEOMETER_GET_LASER) {
      if (gc->getRawReadings()) {
        glObjs.vActualLaserPoints.clear();
        lds = gc->getLaserDataSize();
        //lds >>= 1; // lds /= 2
        ld  = gc->getLaserData();
        for(short punkt=0; punkt < lds; punkt+=2) {
          laserP3d.x = -ld[punkt + 1];
          laserP3d.y =  ld[punkt ];
          laserP3d.z =  300;
          glObjs.vActualLaserPoints.push_back(laserP3d);
          //glObjs.vAllLaserPoints.push_back(laserP3d);

          if ((displayRate == 1) && (*conf & SAVE_IMG_SEQ)) {
            // store the laser readings as originals
            lg.lLD.vLP.push_back( LaserPoint( laserP3d.y, -laserP3d.x) );
          }
        } // for punkt
      } // if get raw readings
    } // if get_laser
	if ((displayRate == 1) 
        && (*conf & SAVE_IMG_SEQ) /* TODO: Uncomment */
        ) {
	  lg.lLD.lp.x  = gc->getX();
	  lg.lLD.lp.y  = gc->getY();
	  lg.lLD.lp.th = gc->getTh();
	  lg.lLD.lp.count = imgSaveCount;
	  lg.add();
	}
  } // if gc
}

void AmrosApp::runKinect() {

  if ( (*conf & OPEN_CAMERA) && 
       (*conf & CAPTURE_IMG) ||
       (*conf & LOAD_IMG_SEQ)
     )
  {
    if (confK & _GL_KINECT) {
#ifdef USE_KINECT
      glObjs.rgbdI = capture->getRGBDImage();
      glObjs.getCloud(0);
      //printf("v(%d)=%d\n",glObjs.v3Dkp.size(),640*480);
      /*
      const uint16_t* bfd;
      int cont =0;
      bfd = glObjs.rgbdI->bufferDepth();
      for(int i=0; i<640*480; i++) {
        if (*bfd == 0) {
          cont++;
        }
      }
      printf("0=%d\n",cont);
      */
	  mainWindow->glBox->updateGL();
#else
      glObjs.rgbdI = 0;
#endif
    }
  }

}

int AmrosApp::run() {

//glObjs.glAmros.moveTo(0,0,0,-90);

  // main loop
  for(;;){

    // Capture image from camera
    if ( (*conf & OPEN_CAMERA) && // if the camera is open 
         (*conf & CAPTURE_IMG) && // and is ok to capture
         capture->CaptureImage(imagesCamera) ) // and we get the images
    {				  // then we draw the images
	if (displayRate == 1) {
	  // Process the images to be displayed
          processImages();

	  if (*conf & SHOW_CAP_IMG) {
	    drawOrigImages();
	  }
	  else {
	    // Draw the images to the main window
            drawImages();
	  }

	  if (*conf & SAVE_IMG_SEQ) {
	    saveImages();
	  }
	  displayRate = cfgPgr.displayRate;

	  mainWindow->glBox->updateGL();
	}
	displayRate--;
    }

    runKinect();

    runGeometerClient();

    if (geometer &&
	geometer->robot.isConnected()) {

        glObjs.glGeometer.moveTo(-geometer->robot.getY(), 0.0f,
			          geometer->robot.getX(),
			         -geometer->robot.getTh());

        glObjs.moveTo( geometer->robot.getY(),
		      -geometer->robot.getX(), 0.0f,
		       geometer->robot.getTh() );

        fab.moveTo( geometer->robot.getY(),
		   -geometer->robot.getX(),
		    0.0f,
		    geometer->robot.getTh() );
        if (*conf & GEOMETER_GET_LASER) {
            geometer->getLaserReadings();
            glObjs.vActualLaserPoints.clear();
            for(short punkt=0; punkt < geometer->nLaserPoints; punkt++) {
                 laserP3d.x = -geometer->vLaserPoints[punkt].y;
                 laserP3d.y =  geometer->vLaserPoints[punkt].x;
                 laserP3d.z =  300;
                 glObjs.vActualLaserPoints.push_back(laserP3d);
                 glObjs.vAllLaserPoints.push_back(laserP3d);
		 //printf("x: %.2f, y: %.2f\n", laserP3d.x, laserP3d.y);

	         if ((displayRate == 1) && (*conf & SAVE_IMG_SEQ)) {
		   // store the laser readings as originals
                   lg.lLD.vLP.push_back( LaserPoint(
					   laserP3d.y,
					  -laserP3d.x) );
	         }

	        } // for punkt
	    //mb.setPoints(glObjs.vActualLaserPoints);
	    if ((displayRate == 1) && (*conf & SAVE_IMG_SEQ)) {
	         geometer->robot.lock();
	         lg.lLD.lp.x  = geometer->robot.getX();
	         lg.lLD.lp.y  = geometer->robot.getY();
	         lg.lLD.lp.th = geometer->robot.getTh();
                 geometer->robot.unlock();
	         lg.lLD.lp.count = imgSaveCount;
		 lg.add();
	    }
	} // if *conf & GEOMETER_GET_LASER 
    }  // if geometer->robot.isConnected()) 

    // Get laser readings
    if ( rob &&
	 rob->robotIsConnected>0 )
    {
      theRobPose = rob->getRobPose();

      glObjs.glAmros.moveTo(theRobPose(0,0),theRobPose(0,1), 0,theRobPose(0,2));
      glObjs.moveTo(theRobPose(0,0),theRobPose(0,1), 0,theRobPose(0,2));
      fab.moveTo(theRobPose(0,0),theRobPose(0,1), 0,theRobPose(0,2)-90.0);

      if (*conf & ROBOT_LASER) {
         theSensors = rob->getSensors();
         for (short sensor=0; sensor<theSensors.Number; sensor++) {
           if (std::strstr("fLaser",theSensors.Name[sensor])) {
             rob->robot_GetSensorCurrent(sensor);
             laserDataP=theSensors.Data[sensor];
             laserData=*laserDataP;

             //printf(" %d...%d\n",laserData.matrixCols(),laserData.matrixRows());
             if (laserData.matrixCols()>0){

               glObjs.vActualLaserPoints.clear();
               for(short punkt=0; punkt < laserData.matrixCols(); punkt++) {
                 laserP3d.x = laserData(0,punkt);
                 laserP3d.y = laserData(1,punkt);
                 laserP3d.z = 476;
                 glObjs.vActualLaserPoints.push_back(laserP3d);
                 glObjs.vAllLaserPoints.push_back(laserP3d);

	         if ((displayRate == 1) && (*conf & SAVE_IMG_SEQ)) {
                   lg.lLD.vLP.push_back( LaserPoint(laserData(0,punkt), laserData(1,punkt)) );
	         }

	       } // for punkt
	       mb.setPoints(glObjs.vActualLaserPoints);
	       if ((displayRate == 1) && (*conf & SAVE_IMG_SEQ)) {
	         lg.lLD.lp.x = theRobPose(0,0);
	         lg.lLD.lp.y = theRobPose(0,1);
	         lg.lLD.lp.th= theRobPose(0,2);
	         lg.lLD.lp.count = imgSaveCount;
		 lg.add();
	       }

	     } // if matrixCols
	   } // if fLaser
	 } // for sensor
      } // if ROBOT_LASER
    } // if rob

    // ***********************
    // *** SIMULATION AREA ***
    // ***********************
    // Capture image from file
    if ( (*conf & LOAD_IMG_SEQ) && // if is set load sequence image
         (*conf & ~CAPTURE_IMG) ) // and the camera is not capturing
    {
	if (displayRate == 1) {
#ifndef USE_KINECT
      if ( imgCapture && imgCapture->CaptureImage(imagesCamera) ) 
#else
      if (capture && capture->CaptureImage(imagesCamera) ) 
#endif
      {
	    // Process the images to be displayed
            processImages();

	    if (*conf & SHOW_CAP_IMG) {
	      drawOrigImages();
	    }
	    else {
	      // Draw the images to the main window
              drawImages();
	    } // if SHOW_CAP_IMG

	    //printf("%4d: imagen\n",imgSaveCount);
	    imgSaveCount++;
 
            // TODO delete only demostration
            //glObjs.glAmros.moveRel(20,10,0,10);
            //glObjs.moveRel(20,10,0,10);
            //fab.moveRel(20,10,0,10);
  
	    if (lg.loaded) {
              glObjs.vActualLaserPoints.clear();
              for(VLaserPoint::iterator ilp=lg.lLD.vLP.begin(); ilp != lg.lLD.vLP.end(); ilp++) {
                laserP3d.x = -ilp->y();
                laserP3d.y =  ilp->x();
                laserP3d.z = 300;
                glObjs.vActualLaserPoints.push_back(laserP3d);
                //glObjs.vAllLaserPoints.push_back(laserP3d);
	        //IVTPrintf::p(laserP3d,"laserP3d");
              }
	      //mb.setPoints(glObjs.vActualLaserPoints);
              //glObjs.glAmros.moveTo(lg.lLD.lp.x, lg.lLD.lp.y, 0, lg.lLD.lp.th);
              glObjs.glGeometer.moveTo(-lg.lLD.lp.y, 0.0f, lg.lLD.lp.x, -lg.lLD.lp.th);
              glObjs.moveTo(-lg.lLD.lp.y, lg.lLD.lp.x, 0, lg.lLD.lp.th);

              fab.moveTo( lg.lLD.lp.x, lg.lLD.lp.y, 0.0f, lg.lLD.lp.th - 90.0f);
	      lg.next();

	      mainWindow->glBox->updateGL();
	    } // if lg.loaded
	  }
	  else {
#ifndef USE_KINECT
	    imgCapture->Rewind();
#endif
	    lg.begin();
	  }
	  displayRate = cfgPgr.loadRate;
	} // if displayRate
	displayRate--;
    }

    // Send messages to Message window
    sendMsg();
			
    if (confK & _STEP) {
      *conf &= ~LOAD_IMG_SEQ;
      *conf &= ~CAPTURE_IMG;
    }
    // If the main winodw is hidden, maybe closed, then
    // call the exit handler to finish the application.
    if (mainWindow->isHidden()) {
      qtApplicationHandler.Exit();
    }

    // On Qt3 this works, put in Qt4/Windows no, so thats why 
    // "if (mainWindow->isHidden())" is needed,
    // keep it to backcompatibility.
    if (qtApplicationHandler.ProcessEventsAndGetExit()) {
      break;
    }
  } // for(;;)

  return 0;
}

void AmrosApp::sendMsg() {
  char stringMsg[121];
  short len;

  // Get a line from the file
  fgets(stringMsg,121,streamIn);

  while (! feof(streamIn) ) {
    // get size of string
    len=strlen(stringMsg);
    // put "end of line" "\0" where the CR is.
    // CR = Carrier Return = Enter character
    stringMsg[len-1] = '\0';
    // Send message
    mainWindow->message( stringMsg );
    // get new line
    fgets(stringMsg,121,streamIn);
  }
  // clear the flag of EOF
  clearerr(streamIn);

  // Flushing all data to stream out
  fflush(streamOut);

}

void AmrosApp::drawImages(){
  short cont = nCameras;
  while (cont--) {
    mainWindow->imgs[cont]->drawImage(imagesDisplay[cont]);
  }
}

void AmrosApp::drawOrigImages(){
  short cont = nCameras;
  // Number of not visible window
  short nNotVisible = 0;
  while (cont--) {
    if (wOImgs[cont]) {
      if (wOImgs[cont]->isVisible()) {
	if (*conf & ANY_FILTER) {
          wOImgs[cont]->drawImage(imagesProcess[cont]);
	} else {
          wOImgs[cont]->drawImage(imagesCamera[cont]);
	}
      }
      else {
	nNotVisible++;
      }
    }
  }
  if (nNotVisible == nCameras) {
    cont = nCameras;
    while (cont--) {
      delete wOImgs[cont];
      wOImgs[cont] = 0;
    }
    *conf &= ~SHOW_CAP_IMG;
  }
}

void AmrosApp::setupOImg(){
  short cont = nCameras;
  const char *sImg[2] ={"Image Left", "Image Right"};
  while (cont--) {
    if (wOImgs[cont]) delete wOImgs[cont];
    wOImgs[cont]= new QtWImage(imgWidth,imgHeight,&wEOImg[cont]);
    wOImgs[cont]->show();
    // Put the name to the window title
    #if QT_VERSION >= 0x040000
    wOImgs[cont]->setWindowTitle( tr(sImg[cont]) );
    #else
    wOImgs[cont]->setCaption( tr(sImg[cont]) );
    #endif

  }
  drawOrigImages();
  // Asigning the Window Event to the images
  wEOImg[0].setImage(CAP_IMG_LEFT);
  wEOImg[1].setImage(CAP_IMG_RIGHT);
}


void AmrosApp::action(unsigned int cfg) {
  QString sImgSeqLeft,sImgSeqRight;
  char c_str[31];
  switch(cfg)  {
    case OPEN_CAMERA:
      if (!capture) {
        capture = new CAPTURE_CLASS ( cfgPgr.kf.c_str() );
      }
      if ( !capture->OpenCamera() ) {
        mainWindow->warning("error: could not open camera");
        mainWindow->setCfg(~OPEN_CAMERA);

      } else {
	printf("Number of cameras: %d\n", capture->GetNumberOfCameras() );
        printf("Image properties: width: %d, height: %d\n",
			capture->GetWidth(), capture->GetHeight() );

	// Setup Images
        setupImages(capture);
	#ifdef USE_DFLY
	// if we need to swap stereo images
	if ((nCameras == 2) && (cfgPgr.boolFlag & CONF_SWAP_ST)){
	  capture->SwapCameras(0,1);
    printf("************** Swap ######################");
	}
	#endif
        #ifdef USE_SVS
          //capture->SetColor(false);
          //capture->SetRed(10);
          //capture->SetBlue(10);
          capture->SetExposure(40);
          //capture->SetGain(40);
          //capture->SetRectify(false);
        #endif
      }
    break;
    case ~OPEN_CAMERA:
      capture->CloseCamera();
    break;
    case ~CAPTURE_IMG:
      // To ensure that last image is displayed
      processImages();
      drawImages();
    break;
    case ~SAVE_IMG_SEQ:
        // save laser points to a file.
        lg.save();
    break;
    case SAVE_IMG_SEQ:
        imgSaveCount = 0;
    	#if QT_VERSION >= 0x040000
	sImgSeqLeft = QFileDialog::getExistingDirectory(mainWindow,
			tr("Save Directory of firt image"), ".",
                        QFileDialog::ShowDirsOnly |
			QFileDialog::DontResolveSymlinks); 
	if (nCameras == 2) {
	  sImgSeqRight = QFileDialog::getExistingDirectory(mainWindow,
			   tr("Save Directory of second image"), ".",
                           QFileDialog::ShowDirsOnly |
			   QFileDialog::DontResolveSymlinks); 
	}
	#else
	//sImgSeqLeft = QFileDialog::getOpenFileName();
	sImgSeqLeft = QFileDialog::getExistingDirectory();
	if (nCameras == 2) {
	  //sImgSeqRight = QFileDialog::getOpenFileName();
	  sImgSeqRight = QFileDialog::getExistingDirectory();
	}
	#endif
	if (sImgSeqLeft == "") {
          mainWindow->setCfg(SAVE_IMG_SEQ);
	  return;
	}
	sSavePath[0] = sImgSeqLeft.QTX_LATIN1;
	if (nCameras == 2) {
	  sSavePath[1] = sImgSeqRight.QTX_LATIN1;
	}
    break;
    case LOAD_IMG_SEQ:
    	#if QT_VERSION >= 0x040000
	sImgSeqLeft = QFileDialog::getOpenFileName(mainWindow,
		tr("Left camera image"),"",tr("Images (*.rgbd *.bmp *.ppm *.pgm)") );
	sImgSeqRight = QFileDialog::getOpenFileName(mainWindow,
		tr("Right camera image"),"",tr("Images (*.rgbd *.bmp *.ppm *.pgm)") );
	#else
	sImgSeqLeft = QFileDialog::getOpenFileName("", "Images (*.rgbd *.bmp *.ppm *.pgm)",
		mainWindow, "Open File Dialog", "Choose a file");
	sImgSeqRight = QFileDialog::getOpenFileName("", "Images (*.rgbd *.bmp *.ppm *.pgm)",
		mainWindow, "Open File Dialog", "Choose a file");
	#endif

	if (sImgSeqLeft == "") {
          mainWindow->setCfg(LOAD_IMG_SEQ);
	  return;
	}
#ifndef USE_KINECT
	if (imgCapture) delete imgCapture;
	if (sImgSeqRight == "") {
	  imgCapture = new CBitmapSequenceCapture(
			sImgSeqLeft.QTX_LATIN1  );
	} else {
	  imgCapture = new CBitmapSequenceCapture(
			sImgSeqLeft.QTX_LATIN1,
			sImgSeqRight.QTX_LATIN1 );
	}

        mainWindow->message("Openning Image Sequence Camera");
	if ( !imgCapture->OpenCamera() ) {
          mainWindow->warning("error: could not open image sequence camera");
          mainWindow->setCfg(LOAD_IMG_SEQ);
	  return;
	}

	setupImages(imgCapture);
#else
    if (capture) delete capture;
    capture = new CAPTURE_CLASS ( sImgSeqLeft.QTX_LATIN1 );
    mainWindow->message("Openning Image Sequence Camera");
    if ( !capture->OpenCamera() ) {
      mainWindow->warning("error: could not open image sequence camera");
      mainWindow->setCfg(LOAD_IMG_SEQ);
      delete capture;
      capture = 0;
      return;
    }
    setupImages(capture);
#endif
	displayRate = 1;

	// Loading laser log
	if (lg.load()) {
	  lg.begin();
          glObjs.setConfig(GL_VIEW_LASER_P);
          glObjs.vAllLaserPoints.clear();
          mainWindow->message("Laser log loaded.");
	}
    break;
    case ~LOAD_IMG_SEQ:
#ifndef USE_KINECT
    	delete imgCapture;
    	imgCapture = 0;
#else
    	delete capture;
    	capture = 0;
#endif
    break;
    case LOAD_IMAGE:
    	#if QT_VERSION >= 0x040000
	sImgSeqLeft = QFileDialog::getOpenFileName(mainWindow,
		tr("Left camera image"),"",tr("Images (*.bmp *.ppm *.pgm)") );
	sImgSeqRight = QFileDialog::getOpenFileName(mainWindow,
		tr("Right camera image"),"",tr("Images (*.bmp *.ppm *.pgm)") );
	#else
	sImgSeqLeft = QFileDialog::getOpenFileName("", "Images (*.bmp *.ppm *.pgm)",
		mainWindow, "Open File Dialog", "Left camera image");
	sImgSeqRight = QFileDialog::getOpenFileName("", "Images (*.bmp *.ppm *.pgm)",
		mainWindow, "Open File Dialog", "Right camera image");
	#endif

	if (sImgSeqLeft == "") {
	  return;
	}
	if (imgCapture) delete imgCapture;
	if (sImgSeqRight == "") {
	  imgCapture = new CBitmapSequenceCapture(
			sImgSeqLeft.QTX_LATIN1 );
	} else {
	  imgCapture = new CBitmapSequenceCapture(
			sImgSeqLeft.QTX_LATIN1,
			sImgSeqRight.QTX_LATIN1 );
	}

        mainWindow->message("Openning Image ");
	if ( !imgCapture->OpenCamera() ) {
          mainWindow->warning("error: could not open image");
	  delete imgCapture;
	  imgCapture=0;
	  return;
	}
	setupImages(imgCapture);
        if ( imgCapture->CaptureImage(imagesCamera) ) {
	  // Process the images to be displayed
          processImages();
	  // Draw the images to the window
          drawImages();

	  delete imgCapture;
	  imgCapture = 0;
	}
    break;
    case CAMERA_PARAM:
    	#if QT_VERSION >= 0x040000
	sImgSeqLeft = QFileDialog::getOpenFileName(mainWindow,
		tr("Choose camera parameters file"),"",tr("All Files (*.*)") );
	#else
	sImgSeqLeft = QFileDialog::getOpenFileName("", "All Files (*.*)",
		mainWindow, "Choose camera parameters", "Choose camera parameters file");
	#endif

	if (sImgSeqLeft == "") {
	  *conf &= ~CAMERA_PARAM;
	  return;
	}

	if (!stereoCalib.LoadCameraParameters(sImgSeqLeft.QTX_LATIN1)) {
	  mainWindow->warning("error: could not open camera parameter file");
	  *conf &= ~CAMERA_PARAM;
	  return;
	}

	/*
	Vec3d p;
	Vec2d le,ri;

	IVTPrintf::p(
	  stereoCalib.GetLeftCalibration()->m_translation_inverse,
	  "Left translation");
	IVTPrintf::p(
	  stereoCalib.GetRightCalibration()->m_translation_inverse,
	  "Right translation");
	Math3d::AddVecVec(
	  stereoCalib.GetLeftCalibration()->m_translation_inverse,
	  stereoCalib.GetRightCalibration()->m_translation_inverse,
	  p);
        Math3d::MulVecScalar(p, 0.5, p);
	IVTPrintf::p(p,"p");
	p.z += 600.0;
	IVTPrintf::p(p,"p + 600");
        stereoCalib.GetLeftCalibration()->GetCameraCoordinates(p, le);
	stereoCalib.GetRightCalibration()->GetCameraCoordinates(p, ri);
	IVTPrintf::p(le,"le");
	IVTPrintf::p(ri,"ri");
	printf("d=%f\n",Math2d::Distance(le,ri));
	*/

        mainWindow->message("Camera parameter file loaded.");
    break;
    case LOAD_CONFIG:
    	// Open an reading the configuration file
    	cfgPgr.open();
    break;
    case POSE_PROC:
	if (stereoCalib.LoadCameraParameters(cfgPgr.strCalibFile.c_str())) {
          mainWindow->message("Camera parameter file loaded.");
	  *conf |= CAMERA_PARAM;
	} else {
	  mainWindow->warning("Error: could not open camera parameter file");
	  mainWindow->warning( cfgPgr.strCalibFile.c_str() );
          mainWindow->setCfg(POSE_PROC);
	  return;
	}
	//mike
	//stereoCalib.SaveCameraParameters("CP.txt");
         //Math3d::Transpose(F, FT);
	 //IVTPrintf::p(F,"Fundamental matrix");
	
        // Casting the Landmarks
	numLandmarks = cfgPgr.vStrClassifierFile.size(); 
        for(unsigned short l=0; l<numLandmarks; l++) {
          if (landmarks[l]) delete landmarks[l];
          landmarks[l] = new Landmark( cfgPgr.vStrClassifierFile[l].c_str() );
	  if (!landmarks[l]->ok) {
            delete landmarks[l];
            landmarks[l] = 0;
	    numLandmarks = 0;
            sImgSeqLeft ="Can not open the training file: ";
            sImgSeqLeft+= cfgPgr.vStrClassifierFile[l].c_str();
	    mainWindow->warning(sImgSeqLeft.QTX_LATIN1);
            mainWindow->setCfg(POSE_PROC);
	    break;
	  } // if !ok
	  landmarks[l]->id=l+1;
        } // for l

	glObjs.numLandmarks = numLandmarks;
	fab.setCanny(0,50);
	fab.setStereoCalibration(&stereoCalib);
	fab.paperMode=true;

    break;
    case GEOMETER_CONNECT:
     if (cfgPgr.rg) {
        if (!gc) {
          gc = new GeometerClient();
        }
        gc->parseArgs(0,NULL);
        if (!gc->connect()) {
          mainWindow->warning("Can not connect to remote geometer");
          delete gc;
          gc = 0;
          return;
        }
        mainWindow->message("Remote Geometer connected.");
        glObjs.setConfig(GL_VIEW_LASER_P);
     } else {
        if (!geometer) {
          geometer = new Geometer();
        }
        if (geometer->robot.isConnected()) return;

        geometer->connectAndRun();

        if (geometer->robot.isConnected()) {
          mainWindow->message("Robot connected.");

          geometer->robot.lock();
          geometer->robot.enableMotors();
          geometer->robot.disableSonar ();
          geometer->robot.unlock();

          glObjs.setConfig(GL_VIEW_LASER_P);
        } else {
          mainWindow->warning("Can not connect to robot");
          delete geometer;
          geometer = 0;
        }
     } // if (cfgPgr.rg) 
    break;
    case GEOMETER_LASER:
      if (geometer) {
        if (geometer->robot.isConnected()) {
          if( geometer->connectLaser() ) {
	    mainWindow->message("Laser is connected.");
            glObjs.vAllLaserPoints.clear();
	  } else {
	    mainWindow->warning("Laser is not connected");
	  }
	} else {
	  mainWindow->warning("Robot is not connected");
	}
      } else {
         if (cfgPgr.rg) {
            if (gc) {
              gc->initLaser();
            }
         } else {
            mainWindow->warning("Robot is not set");
         }
      }
    break;
    //break;
    case ROBOT_CONNECT:

      //Aria::init();	
      std::strcpy(c_str,"localhost");

      if (!rob) {
        rob = new RobotController(c_str);
      }

      if (rob->robotIsConnected>0) {
        theSensors.Number=0;
        // c_str="28 0" -> Sonar disabled
        sprintf(c_str,"%d %d",ArCommands::SONAR, 0);
        rob->robot_MicroControllerCommand(c_str);
	mainWindow->message("Robot connected.");

        glObjs.setConfig(GL_VIEW_LASER_P);

      }
      else {
	mainWindow->warning("Can not connect to robot");
	delete rob;
	rob = 0;
      }
    break;
    case ROBOT_STOP:
      if (rob && rob->robotIsConnected>0) {
	mainWindow->message("Stop robot movement.");
        // c_str="29 0" -> stops the robot 
        sprintf(c_str,"%d %d",ArCommands::STOP, 0);
        rob->robot_MicroControllerCommand(c_str);
      } else {
        if (geometer && geometer->robot.isConnected() ) {
	  geometer->robot.lock();
	  geometer->robot.clearDirectMotion();
	  geometer->robot.stop();
	  geometer->robot.unlock();
        } else {
	 mainWindow->warning("Robot not connected.");
        }
      }
     if (cfgPgr.rg) {
       if (gc) {
         if (gc->stop()) mainWindow->message("Stop robot movement.");
       }
     }
    break;
    case ROBOT_LEFT:
      if (rob && rob->robotIsConnected>0) {
        // c_str="13 ANGLE" -> turn relative to current heading (degrees) 
        sprintf(c_str,"%d %d",ArCommands::DHEAD, 15);
        rob->robot_MicroControllerCommand(c_str);
      } else {
        if (geometer && geometer->robot.isConnected() ) {
	  geometer->robot.lock();
	  geometer->robot.setRotVel(10);
	  geometer->robot.unlock();
        } else {
           if (cfgPgr.rg) {
             if (gc) {
               gc->setRotVel(20);
             }
           } else {
             mainWindow->warning("Robot not connected.");
           }
        }
      }
    break;
    case ROBOT_RIGHT:
      if (rob && rob->robotIsConnected>0) {
        // c_str="13 ANGLE" -> turn relative to current heading (degrees) 
        sprintf(c_str,"%d %d",ArCommands::DHEAD, -15);
        rob->robot_MicroControllerCommand(c_str);
      } else {
        if (geometer && geometer->robot.isConnected() ) {
	  geometer->robot.lock();
	  geometer->robot.setRotVel(-10);
	  geometer->robot.unlock();
        } else {
         if (cfgPgr.rg) {
           if (gc) {
             gc->setRotVel(-20);
           }
         } else {
           mainWindow->warning("Robot not connected.");
         }
        }
      }
    break;
    case ROBOT_UP:

////////////////////////////////////////////////////////////////////
#if 0  
    def USE_KINECT
    if (confK & _GL_KINECT) {
      //const uint16_t* bfd;
      std::vector< CvPoint3D32f >  vp;
      std::vector< CvPoint3D32f >::iterator  ivp;
      //int cont =0;
      //bfd = glObjs.rgbdI->bufferDepth();
      glObjs.rgbdI->getCloud3DPoints(vp);
      float max, mix, may, miy, maz, miz;
      max=may=0.0f;
      maz=-5000.0f;
      mix=miy=miz=1000.0f;

      //for(int i=0; i<640*480; i++) 
      for(ivp=vp.begin(); ivp != vp.end(); ++ivp)
      {
        if (ivp->z != 0.0f) {
            if(ivp->x > max) max = ivp->x;
            if(ivp->x < mix) mix = ivp->x;
            if(ivp->y > may) may = ivp->y;
            if(ivp->y < miy) miy = ivp->y;
            if(ivp->z > maz) maz = ivp->z;
            if(ivp->z < miz) miz = ivp->z;
        }
      }
       //(    -2543.98828,    -1982.89722,    -9757.00000)
       //(     1720.75745,     2839.29492,        0.00000)
       //
       //(    -2602.17993,     -813.03412,    -9757.00000) 
       //(     2928.08105,     2382.70483,    -1480.00000)
      printf("(%15.5f,%15.5f,%15.5f) (%15.5f,%15.5f,%15.5f)\n",
          mix, miy, miz, max, may, maz);
      //printf("0=%d\n",cont);
      //glObjs.rgbdI->computeDepthNormalized2();
    }
#endif
////////////////////////////////////////////////////////////////////
      if (rob && rob->robotIsConnected>0) {
        // c_str="11 VEL" -> set the translational velocity (mm/sec) 
        sprintf(c_str,"%d %d",ArCommands::VEL, robVel);
        rob->robot_MicroControllerCommand(c_str);
      } else {
        if (geometer && geometer->robot.isConnected() ) {
	  geometer->robot.lock();
	  geometer->robot.setRotVel(0);
	  geometer->robot.setVel(robVel);
	  geometer->robot.unlock();
        } else {
         if (cfgPgr.rg) {
           if (gc) {
             gc->setRotVel(0);
             gc->setVel(robVel);
           }
         } else {
           mainWindow->warning("Robot not connected.");
         }
        }
      }
    break;
    case ROBOT_DOWN:
      if (rob && rob->robotIsConnected>0) {
        // c_str="11 VEL" -> set the translational velocity (mm/sec) 
        sprintf(c_str,"%d %d",ArCommands::VEL, -robVel);
        rob->robot_MicroControllerCommand(c_str);
      } else {
        if (geometer && geometer->robot.isConnected() ) {
	  geometer->robot.lock();
	  geometer->robot.setRotVel(0);
	  geometer->robot.setVel(-robVel);
	  geometer->robot.unlock();
        } else {
           if (cfgPgr.rg) {
             if (gc) {
               gc->setRotVel(0);
               gc->setVel(-robVel);
             }
           } else {
            mainWindow->warning("Robot not connected.");
           }
        }
      }
    break;
    case ROBOT_SPEED_UP:
	robVel+=30;
    //TODO
    //ktmp();
    break;
    case ROBOT_SPEED_DOWN:
	robVel-=30;
    break;
    case ROBOT_INFO:
      if (rob && rob->robotIsConnected>0) {
        sprintf(c_str,"Battery: %2.1f V",rob->getRobVoltage());
        mainWindow->message(c_str);
        sprintf(c_str,"Vel: %3d mm/sec",robVel);
        mainWindow->message(c_str);
        //mainWindow->message("Pos (x,y,th):");
        theRobPose = rob->getRobPose();
        sprintf(c_str,"(%5.1f,%5.1f,%5.1f)",
			theRobPose(0,0),theRobPose(0,1),theRobPose(0,2));
        sImgSeqLeft = "Pos (x,y,th): ";
        sImgSeqLeft += c_str;
        mainWindow->message(sImgSeqLeft.QTX_LATIN1);
		
      } else {
        if (geometer && geometer->robot.isConnected() ) {
	  geometer->robot.lock();
          printf("velocities: %.2f %.2f %.2f\n", geometer->robot.getVel(),
	    geometer->robot.getRotVel(), geometer->robot.getLatVel());
          printf("pose ---->: %.2f %.2f  angle: %.2f\n", geometer->robot.getX(),
	    geometer->robot.getY(), geometer->robot.getTh());
	  geometer->robot.unlock();
        } else {
	 mainWindow->warning("Robot not connected.");
        }
      }
     if (cfgPgr.rg) {
       if (gc) {
         if (gc->getPose()) {
          printf("velocities: %.2f\t Volt: %.2f\n", gc->getVel(),
              gc->getBatteryVoltage());
	    // TODO gc->getRotVel(), gc->getLatVel());
          printf("pose ---->: %.2f %.2f  angle: %.2f\n", gc->getX(),
	    gc->getY(), gc->getTh());
         } else {
	        mainWindow->warning("Can not get remote Pose.");
         }
       }
     }
    break;
    case LASER_P2L:
       mb.p2l();
    break;
    case GL_KINECT:
        if (confK & _GL_KINECT) {
          confK &= ~_GL_KINECT;
        } else {
          confK |= _GL_KINECT;
        }
        glObjs.setConfig(_GL_KINECT);
    break;
    case K_UNO:  // Open and load
      *conf |= LOAD_IMG_SEQ;
      action(LOAD_IMG_SEQ);
      *conf &= ~LOAD_IMG_SEQ;
      confK |= _STEP;
    break;
    case K_DOS: 
      if (*conf & OPEN_CAMERA) {
        *conf |= CAPTURE_IMG;
      } else {
        *conf |= LOAD_IMG_SEQ;
      }
	  displayRate = 1;
      confK |= _STEP;
    break;
    case K_TRES:
      confK &= ~_STEP;
      if (*conf & OPEN_CAMERA) {
        *conf |= CAPTURE_IMG;
      } else {
        *conf |= LOAD_IMG_SEQ;
      }
    break;
    case K_CUAT: // 4 save images
      sSavePath[0] = "img";
      saveImages(true);
      mainWindow->message("Images rgb y d saved.");
    break;
    case K_CINC: printf("Cinco\n"); break;
    case K_SEIS: printf("Seis\n"); break;
    default:
    	if (cfg & MOUSE_ANY) {
	  eventImages(cfg);
	}
    break;
  }
}

void AmrosApp::setupImages(CVideoCaptureInterface *cap){

  short cont;

  freeImages();

  nCameras  = cap->GetNumberOfCameras();

  // For this program only use the first 2 cameras
  if (nCameras > 2) {
    nCameras  = 2;
  }

  imgWidth  = cap->GetWidth();
  imgHeight = cap->GetHeight();
  imgType   = cap->GetType();

  imagesCamera  = new CByteImage*[nCameras];
  imagesDisplay = new CByteImage*[nCameras];
  imagesProcess = new CByteImage*[nCameras];
  wOImgs = new QtWImage*[nCameras];

  cont = nCameras;
  while (cont--) {
    imagesCamera[cont]  = new CByteImage(imgWidth,
                                         imgHeight,
					 imgType);
    imagesProcess[cont] = new CByteImage(imgWidth,
                                         imgHeight,
					 //imgType);
					 CByteImage::eGrayScale);
    imagesDisplay[cont] = new CByteImage(DSP_IMG_WIDTH,
		                         DSP_IMG_HEIGHT,
					 imgType);
    wOImgs[cont] = 0;

    // Set the images to Find and Build
    fab.setImg(imagesProcess[cont],cont);
  }
}

void AmrosApp::processKinectImg()
{
#ifdef USE_KINECT

#endif
}

void AmrosApp::processImages(){
  short cont = nCameras;
  //MyRegion *regObj[2];
  CByteImage **imgTmp = new CByteImage*[nCameras];

  // Used in Canny
  IplImage *pIplInputImage;
  IplImage *pIplOutputImage;

  bool searchLandmark = true;

  //short wechsel = 1;

  while (cont--) {
    switch (*conf & ANY_FILTER) {
      case HARRIS_FILTER:
        imgTmp[cont] = new CByteImage(imgWidth, imgHeight,
				      CByteImage::eGrayScale);
        ImageProcessor::ConvertImage(imagesCamera[cont], imgTmp[cont]);
        harrisPFound[cont] = ImageProcessor::CalculateHarrisInterestPoints(
			    imgTmp[cont], vHarrisPoints, MAX_HARRIS_POINTS);
	printf("H(%d)=%d\n",cont,harrisPFound[cont]);
        //ImageProcessor::ConvertImage(imgTmp[cont], imagesProcess[cont]);
        ImageProcessor::CopyImage(imgTmp[cont], imagesProcess[cont]);
	for (short points=0; points<harrisPFound[cont]; points++) {
	  //PrimitivesDrawer::DrawPoint(imagesProcess[cont], vHarrisPoints[points],255,0,0);
          DrawAnX(imagesProcess[cont], vHarrisPoints[points], 255,255,255);
	}
	delete imgTmp[cont];
      break;
      case CANNY_FILTER:
        imgTmp[cont] = new CByteImage(imgWidth, imgHeight,
				      CByteImage::eGrayScale);
	//delete imagesProcess[cont];
	//imagesProcess[cont] = new CByteImage(imgTmp[cont]);
        //imagesProcess[cont] = new CByteImage(imgWidth, imgHeight,
				      //CByteImage::eGrayScale);
        ImageProcessor::ConvertImage(imagesCamera[cont], imgTmp[cont]);
        //ImageProcessorCV::Canny(imgTmp[cont], imagesProcess[cont],10,80);
	//La siguiente es como la ve el Harr
	
        pIplInputImage = IplImageAdaptor::Adapt(imgTmp[cont]);
        pIplOutputImage = IplImageAdaptor::Adapt(imagesProcess[cont]);

	cvEqualizeHist( pIplInputImage, pIplInputImage);
        cvCanny(pIplInputImage, pIplOutputImage, 0, 50);

        //ImageProcessorCV::Canny(imgTmp[cont], imagesProcess[cont],0,50);

        cvReleaseImageHeader(&pIplInputImage);
        cvReleaseImageHeader(&pIplOutputImage);
	delete imgTmp[cont];
      break;
      // TODO BORRAR
      //default:
        // nur für CLICK_MATCH, dann löschen
	//if (*conf & CLICK_MATCH) {
          //ImageProcessor::ConvertImage(imagesCamera[cont],
			      //imagesProcess[cont]);
	//}
      //break;
    } // switch ANY_FILTER
    if (*conf & POSE_PROC) {
      ImageProcessor::ConvertImage(imagesCamera[cont],
			      imagesProcess[cont]);

      if (searchLandmark) {

  	searchLandmark = false;

        for(unsigned short l=0; l<numLandmarks; l++) {
	  if ( fab.find(landmarks[l], cont) ) {
	    if (!cont &&
	        landmarks[l]->objFound[L_IMG] > OBJ_NOT_FOUND &&
	        landmarks[l]->objFound[R_IMG] > OBJ_NOT_FOUND )
	    {
	      if (fab.build(landmarks[l])) {

		// Uncommento to show the points
		/*** show points ***
		for(Vec3dList::iterator it = fab.vP3D.begin(); it != fab.vP3D.end(); it++) {
	          glObjs.vCameraPoints.push_back( *it );
		}
		*** end show points ***/

		glObjs.vLandmarks[l] = landmarks[l]->getPose();

	        switch(l) {
	          case 0:
                    fab.showPoints(imagesCamera[L_IMG],L_IMG);
                    //fab.showPoints(imagesCamera[R_IMG],R_IMG);
		  break;
		  case 1:
                    fab.showPoints(imagesCamera[L_IMG],L_IMG,255,0,0);
                    //fab.showPoints(imagesCamera[R_IMG],R_IMG,0,255,0);
		  break;
		  case 2:
                    fab.showPoints(imagesCamera[L_IMG],L_IMG,180,180,180);
                    //fab.showPoints(imagesCamera[R_IMG],R_IMG,0,255,0);
		  break;
		  case 3:
                    fab.showPoints(imagesCamera[L_IMG],L_IMG,120,120,120);
                    //fab.showPoints(imagesCamera[R_IMG],R_IMG,0,255,0);
		  break;
		  case 4:
                    fab.showPoints(imagesCamera[L_IMG],L_IMG,90,90,90);
                    //fab.showPoints(imagesCamera[R_IMG],R_IMG,0,255,0);
		  break;
	        } // switch l

		//TODO DEL img to no storage to much space
                sSavePath[0] = "limg";
                sSavePath[1] = "rimg";
                //saveImages();

	      } // if build

		  //fab.showPoints(imagesCamera[L_IMG],L_IMG,(l*255),255,(l));
	      landmarks[l]->reset();
	    }
  	    searchLandmark = true;
	  } // if find
        } // for LANDMARKS
      } // if searchLandmark
    } // if  POSE_PROC
  } // while cont


  processKinectImg();

  // Resize the images to display
  cont = nCameras;
  while (cont-- && !(*conf & SHOW_CAP_IMG)) {
    if (*conf & ANY_FILTER)  {
      if (imagesDisplay[cont]->type != imagesProcess[cont]->type) {
        delete imagesDisplay[cont];
        imagesDisplay[cont] = new CByteImage(DSP_IMG_WIDTH, DSP_IMG_HEIGHT,
					imagesProcess[cont]->type);
      }
      ImageProcessor::Resize(imagesProcess[cont],imagesDisplay[cont]);
    } else {
      if (imagesDisplay[cont]->type != imgType) {
        delete imagesDisplay[cont];
        imagesDisplay[cont] = new CByteImage(DSP_IMG_WIDTH, DSP_IMG_HEIGHT, imgType);
      }
      ImageProcessor::Resize(imagesCamera[cont],imagesDisplay[cont]);
    }
  } 
  //ImageProcessor::CopyImage(imagesCamera[cont],imagesProcess[cont]);
  delete []imgTmp;
  
}

void AmrosApp::saveImages(bool bothImg) {
  #ifdef WIN32
  char delim = '\\';
  #else
  char delim = '/';
  #endif
  char sPath[101];
#ifdef USE_KINECT
  RGBDImage *rgbdI;
  rgbdI = capture->getRGBDImage();
  sprintf(sPath, "%s%cimage-%.4i.rgbd", sSavePath[0].c_str(),
		  delim,imgSaveCount);
  rgbdI->saveToFile(sPath);
  if (bothImg) {
    sprintf(sPath, "%s%cd-%.4i.bmp", sSavePath[0].c_str(),
		  delim,imgSaveCount);
    imagesCamera[0]->SaveToFile(sPath);
    sprintf(sPath, "%s%crgb-%.4i.bmp", sSavePath[0].c_str(),
		  delim,imgSaveCount);
    imagesCamera[1]->SaveToFile(sPath);
  }
#else
  short cont = nCameras;
  while (cont--) {
    sprintf(sPath, "%s%cimage-%.4i.bmp", sSavePath[cont].c_str(),
		  delim,imgSaveCount);
    if (*conf & ANY_FILTER) {
      imagesProcess[cont]->SaveToFile(sPath);
    } else {
      imagesCamera[cont]->SaveToFile(sPath);
    }
  }
#endif
  imgSaveCount++;
}

void AmrosApp::eventImages(unsigned int evFlag) {
  float mLine,cLine;
  //char sPath[51];
  switch(evFlag & ANY_IMAGE){
    case DSP_IMG_LEFT:
      if (evFlag & MOUSE_DBLCLK) {
        if (nCameras) { 
          *conf |= SHOW_CAP_IMG;
	  setupOImg();
	}
      }
    break;
    case DSP_IMG_RIGHT:
      if (evFlag & MOUSE_DBLCLK) {
        if (nCameras == 2 ) {
          *conf |= SHOW_CAP_IMG;
	  setupOImg();
	}
      }
    break;
    case CAP_IMG_LEFT:
      if (evFlag & MOUSE_CLICK) {
#ifdef USE_KINECT
       RGBDImage *rgbdI;
       if (capture) {
         rgbdI = capture->getRGBDImage();
         imgPoints[0].x = wEOImg[0].mouseEvent->x();
         imgPoints[0].y = wEOImg[0].mouseEvent->y();
         printf("(%3.f,%3.f) = ", imgPoints[0].x, imgPoints[0].y);
         for(short k=0; k<=11; k++) {
           printf("%5d,",
            rgbdI->getDepth((size_t)imgPoints[0].x,
              k-6+(size_t)imgPoints[0].y));
         }
         printf("\n");
       }
#endif
	if (*conf & CAMERA_PARAM) {
	  if (*conf & CLICK_3D_ACT) {
            imgPoints[0].x = wEOImg[0].mouseEvent->x();
            imgPoints[0].y = wEOImg[0].mouseEvent->y();
	    *conf |= IMG_LEFT_CLK;

      // Epipolar
      stereoCalib.CalculateEpipolarLineInRightImage(
          imgPoints[0], mLine, cLine);
      
      Vec2d p1 = { 0.0, cLine };
      Vec2d p2 = { 639.0, mLine * 639.0 + cLine };
      
      PrimitivesDrawer::DrawLine(imagesCamera[1],p1,p2,255,0,0);
      DrawAnX(imagesCamera[0], imgPoints[0],255,255,0);
      drawOrigImages();

      // Epipolar
	    if (*conf & IMG_RIGHT_CLK) {
	      *conf &= ~IMG_BOTH_CLK;
	      stereoCalib.Calculate3DPoint(
			  imgPoints[0], imgPoints[1], worldPoint, false);

	      // **********
	      //IVTPrintf::p(imgPoints[0],"left  point");
	      //IVTPrintf::p(imgPoints[1],"right point");
	      //printf("image distant= %f\n",
		      //Math2d::Distance(imgPoints[0],imgPoints[1]));


	      printf("3d point = %6.2f %6.2f %6.2f\n",
			  worldPoint.x, worldPoint.y, worldPoint.z);

        // Save to CSV
        // To visualice the 3D points in OpenGL acording to the robot
        // we interchange the coordinates (x,y,z) -> (x,z,-y)
        il_p3d.set(worldPoint.x, worldPoint.z, -worldPoint.y);
        myfile << il_p3d << std::endl;

	      //worldPoint.x/=100.0f;
	      //worldPoint.y/=100.0f;
	      //worldPoint.z/=100.0f;
	      glObjs.vCameraPoints.push_back(worldPoint);
	    }
          }
	  if (*conf & CLICK_MATCH) {
	    //Math3d::SetVec(worldPoint,0.0,0.0,0.0);
	    worldPoint = Math3d::zero_vec;
	    imgPoints[1] = Math2d::zero_vec;

            imgPoints[0].x = wEOImg[0].mouseEvent->x();
            imgPoints[0].y = wEOImg[0].mouseEvent->y();
	    //*conf |= IMG_LEFT_CLK;
	    //if (*conf & IMG_RIGHT_CLK) {
	      //*conf &= ~IMG_BOTH_CLK;

	      int ret=1;
	      ret = matchpoint(imagesProcess[0], imagesProcess[1],
			      imgPoints[0].x, imgPoints[0].y,
			      31, 150, 220,
			      imgPoints[1], worldPoint, 0.1f);
	      //printf("ret= %d\n",ret);
	      /*
	      matchpoint(imagesCamera[0], imagesCamera[1],
			          imgPoints[0].x, imgPoints[0].y,
				  21,   // windows size 1-49, 21
				  150,   // 0,minDisp 0-500, 150
				  220, // 400,maxDisp 0-500, 220
				  imgPoints[1], worldPoint, 
				  2.5f);
	      *************************************************/

	      //stereoCalib.Calculate3DPoint(
			  //imgPoints[0], imgPoints[1], worldPoint, false);
			  

	      //printf("2d point = (%f,%f)\n", imgPoints[1].x, imgPoints[1].y);

	      printf("3d point = %6.2f %6.2f %6.2f\n",
			  worldPoint.x, worldPoint.y, worldPoint.z);


	      // Epipolar
 	      stereoCalib.CalculateEpipolarLineInRightImage(
			      imgPoints[0], mLine, cLine);
	      
              Vec2d p1 = { 0.0, cLine };
              Vec2d p2 = { 639.0, mLine * 639.0 + cLine };
	      
	      PrimitivesDrawer::DrawLine(imagesCamera[1],p1,p2,255,0,0);
	      drawOrigImages();
	      // Epipolar

	      if (imgPoints[1].x && imgPoints[1].y) {
	        DrawAnX(imagesCamera[1], imgPoints[1],255,255,0);
	        drawOrigImages();

	        //worldPoint.x/=100.0f;
	        //worldPoint.y/=100.0f;
	        //worldPoint.z/=100.0f;
	        glObjs.vCameraPoints.push_back(worldPoint);
	      }

	    //}
	  }
	  if (*conf & POSE_PROC) {
             //sprintf(sPath, "img-pose-%.4i.ppm", imgSaveCount);
	     //printf("saving: %s\n",sPath);
             //imagesCamera[0]->SaveToFile(sPath);
	  } // if  CLICK_MATCH
	}
      }
    break;
    case CAP_IMG_RIGHT:
      //if ((evFlag & MOUSE_CLICK) && (*conf & (CAMERA_PARAM | CLICK_3D_ACT))) {
      if (evFlag & MOUSE_CLICK) {
	if (*conf & CAMERA_PARAM) {
	  if (*conf & CLICK_3D_ACT) {
            imgPoints[1].x = wEOImg[1].mouseEvent->x();
            imgPoints[1].y = wEOImg[1].mouseEvent->y();
	    *conf |= IMG_RIGHT_CLK;

	    DrawAnX(imagesCamera[1], imgPoints[1],255,255,0);
	    drawOrigImages();

	    if (*conf & IMG_LEFT_CLK) {
	      *conf &= ~IMG_BOTH_CLK;
	      stereoCalib.Calculate3DPoint(
			  imgPoints[0], imgPoints[1], worldPoint, false);
	      
	      //IVTPrintf::p(imgPoints[0],"left  point");
	      //IVTPrintf::p(imgPoints[1],"right point");
	      //printf("image distant= %f\n",
		      //Math2d::Distance(imgPoints[0],imgPoints[1]));
	      
	      printf("3d point = %6.2f %6.2f %6.2f\n",
			  worldPoint.x, worldPoint.y, worldPoint.z);

        // Save to CSV
        // To visualice the 3D points in OpenGL acording to the robot
        // we interchange the coordinates (x,y,z) -> (x,z,-y)
        il_p3d.set(worldPoint.x, worldPoint.z, -worldPoint.y);
        myfile << il_p3d << std::endl;

	      //worldPoint.x/=100.0f;
	      //worldPoint.y/=100.0f;
	      //worldPoint.z/=100.0f;
	      glObjs.vCameraPoints.push_back(worldPoint);
	    }
          }
	}
      }
    break;
  }
  //printf("cfg=%08x\timg=%08x\tmaus=%08x\n",
//	 evFlag, evFlag & ANY_IMAGE, evFlag & MOUSE_ANY);
}

void  AmrosApp::freeImages() {
  short cont = nCameras;
  while (cont--) {
    if (imagesCamera && imagesCamera[cont]) {
      delete imagesCamera[cont];
    }
    if (imagesDisplay && imagesDisplay[cont]) {
      delete imagesDisplay[cont];
    }
    if (imagesProcess && imagesProcess[cont]) {
      delete imagesProcess[cont];
    }
    if (wOImgs && wOImgs[cont]) {
      delete wOImgs[cont];
    }
  }
  if (imagesCamera)  delete []imagesCamera;
  if (imagesDisplay) delete []imagesDisplay;
  if (imagesProcess)  delete []imagesProcess;
  if (wOImgs)  delete []wOImgs;

  imagesCamera  = 0;
  imagesDisplay = 0;
  imagesProcess  = 0;
  wOImgs = 0;
}

int AmrosApp::matchpoint(const CByteImage *pLeftImage,
		const CByteImage *pRightImage,
		int x, int y, int nWindowSize,
	int d1, int d2, Vec2d &result, Vec3d &result_3d,
	float fThreshold, bool bInputImagesAreUndistorted)
{
	if (d1 < 0) d1 = 0;
	if (d2 < 0) d2 = 0;
	if (d1 >= pLeftImage->width) d1 = pLeftImage->width - 1;
	if (d2 >= pLeftImage->width) d2 = pLeftImage->width - 1;
	
	float *values = new float[pLeftImage->width];
	const int best_d = SingleZNCC(pLeftImage, pRightImage, x, y, nWindowSize, d1, d2, values);

	if (best_d != -1 && values[best_d] > fThreshold)
	{
		const double y0 = values[best_d - 1];
		const double y1 = values[best_d];
		const double y2 = values[best_d + 1];
		const double xmin = (y0 - y2) / (2 * (y0 - 2 * y1 + y2));
		const double disparity = best_d + xmin;
		
		float m, c;
		Vec2d point_left = { x, y };
		stereoCalib.CalculateEpipolarLineInRightImage(point_left, m, c);
		
		result.x = x - disparity;
		result.y = m * (x - disparity) + c;
		
		stereoCalib.Calculate3DPoint(point_left, result, result_3d, false, !bInputImagesAreUndistorted);

		delete [] values;
		
		return best_d;
	}
	
	delete [] values;
	
	return 0;
}

int AmrosApp::SingleZNCC(const CByteImage *pInputImage1,
	const CByteImage *pInputImage2, int x, int y, int nWindowSize,
	int d1, int d2, float *values)
{
	const int width = pInputImage1->width;
	const int height = pInputImage1->height;
	
	if (x < nWindowSize / 2 || x >= width - nWindowSize / 2 || y < nWindowSize / 2 || y >= height - nWindowSize / 2)
		return -1;
	
	const unsigned char *input_left = pInputImage1->pixels;
	unsigned char *input_right = pInputImage2->pixels;
	const int nVectorLength = nWindowSize * nWindowSize;
	
	float *vector1 = new float[nVectorLength];
	float *vector2 = new float[nVectorLength];
	
	const int offset = (y - nWindowSize / 2) * width + (x - nWindowSize / 2);
	const int diff = width - nWindowSize;
	Vec2d camera_point = { x, y };
	int k;
	
	// calculate mean value
	float mean = 0;
	{
		for (int y = 0, offset2 = offset, offset3 = 0; y < nWindowSize; y++, offset2 += diff)
			for (int x = 0; x < nWindowSize; x++, offset2++, offset3++)
			{
				vector1[offset3] = input_left[offset2];
				mean += vector1[offset3];
			}
	}
	mean /= nVectorLength;
	
	// apply additive and muliplicative normalization
	float factor = 0;
	for (k = 0; k < nVectorLength; k++)
	{
		vector1[k] -= mean;
		factor += vector1[k] * vector1[k];
	}
	
	if (factor < 60 * nVectorLength)
		return -1;
	
	factor = 1 / sqrtf(factor);
	for (k = 0; k < nVectorLength; k++)
		vector1[k] *= factor;
	
	float best_value = FLT_MIN;
	int d, best_d = 0;

	const int max_d = d2 < x ? d2 : x;
	
	float m, c;
	stereoCalib.CalculateEpipolarLineInRightImage(camera_point, m, c);
	
	// determine correspondence
	for (d = d1; d <= max_d; d++)
	{
		const int yy = int(m * (x - d) + c + 0.5) - nWindowSize / 2;
		
		if (yy < 0 || yy >= height)
			continue;
		
		const int offset_right = yy * width + (x - d - nWindowSize / 2);
		// by Mike
		//const int offset_diff = offset_right - offset;
		
		// determine mean value
		float mean = 0;
		for (int y = 0, offset2 = offset_right, offset3 = 0; y < nWindowSize; y++, offset2 += diff)
			for (int x = 0; x < nWindowSize; x++, offset2++, offset3++)
			{
				vector2[offset3] = input_right[offset2];
				mean += vector2[offset3];
				
			}
		mean /= nVectorLength;
		
		// apply additive and multiplicative normalization
		float factor = 0;
		for (k = 0; k < nVectorLength; k++)
		{
			vector2[k] -= mean;
			factor += vector2[k] * vector2[k];
		}

		if (factor < 60 * nVectorLength)
			continue;

		factor = 1 / sqrtf(factor);
		for (k = 0; k < nVectorLength; k++)
			vector2[k] *= factor;
			
		float value = 0;
		for (k = 0; k < nVectorLength; k++)
			value += vector1[k] * vector2[k];
		
		// save correlation results for subpixel calculations
		values[d] = value;
		
		// determine maximum correlation value
		if (value > best_value)
		{
			best_value = value;
			best_d = d;
		}
	}
	
	delete [] vector1;
	delete [] vector2;
	
	return best_d;
}

void AmrosApp::DrawAnX(CByteImage *pImage, const Vec2d &point,
		short r, short g, short b)
{
  PrimitivesDrawer::DrawPoint(pImage,
		int(point.x + 0.5), int(point.y + 0.5), 
		~r & 0xFF , ~g & 0xFF, ~b & 0xFF);
  PrimitivesDrawer::DrawPoint(pImage,
		int(point.x - 1.5), int(point.y - 1.5), r, g, b);
  PrimitivesDrawer::DrawPoint(pImage,
		int(point.x + 1.5), int(point.y + 1.5), r, g, b);
  PrimitivesDrawer::DrawPoint(pImage,
		int(point.x - 1.5), int(point.y + 1.5), r, g, b);
  PrimitivesDrawer::DrawPoint(pImage,
		int(point.x + 1.5), int(point.y - 1.5), r, g, b);
}

void AmrosApp::ktmp()
{
#ifdef USE_KINECT
   RGBDImage *rgbdI;
   rgbdI = capture->getRGBDImage();
   rgbdI->dumpStats = true;
   rgbdI->bufferDepthVisualize2();
#endif
}

