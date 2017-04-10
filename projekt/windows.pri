####################################################
#                                                  #
#  Put the camera that you will use in the project #
#                                                  #
#  USE_SVS                                         #
#  USE_OPENCV                                      #
#  USE_DFLY                                        #
#  USE_BITMAP                                      #
#  USE_KINECT                                      #
#                                                  #
####################################################

DEFINES += USE_KINECT

# C/C++->Code Generator->Runtime Library: MTd
# Linker->Input->Ignore Specific Library: msvcrtd.lib

# Use console or windows application
#CONFIG += console
CONFIG += windows 

# Where the Open CV is located
INCLUDEPATH += C:/nix/opencv/include
INCLUDEPATH += C:/nix/opencv/include/opencv

QMAKE_LIBDIR += C:/nix/opencv/x86/vc9/lib
#QMAKE_LIBDIR += C:/nix/opencv/x86/vc9/staticlib

# Libraries from OpenCV
LIBS += opencv_core231.lib \
        opencv_contrib231.lib \
        opencv_legacy231.lib \
        opencv_objdetect231.lib \
        opencv_calib3d231.lib \
        opencv_features2d231.lib \
        opencv_video231.lib \
        opencv_highgui231.lib \
        opencv_ml231.lib \
        opencv_imgproc231.lib \
        opencv_flann231.lib

# If you are using HighGUI form OpenCV uncomment following line
#LIBS += highgui.lib

# If you are using SVS MEGA-D camera uncomment following line
#LIBS += pimmegaapi.lib svs.lib svsgrab.lib

# If you are using DragonFly camera uncomment following line
#LIBS += PGRFlyCapture.lib


# Where the ivt library is
IVTDIR = C:/Users/mbernalm/Documents/progs/IVT

# Add it to the include path
INCLUDEPATH += $${IVTDIR}/src 
QMAKE_LIBDIR += $${IVTDIR}/lib/win32 

# Including IVT library
CONFIG(debug) {
  CONFIG(qt) {
    LIBS += ivtguiqtd.lib
  }
  LIBS += ivtd.lib ivtopencvd.lib
} else {
  CONFIG(qt) {
    LIBS += ivtguiqt.lib
  }
  LIBS += ivt.lib ivtopencv.lib
} 


# Aria directory
#ARIADIR = C:\Programme\MobileRobots\Aria
ARIADIR = C:/MobileRobots/Aria

# Aria headers
INCLUDEPATH += $${ARIADIR}/include $${ARIADIR}/ArNetworking/include
 
# Aria libraries
LIBS += $${ARIADIR}/lib/AriaVC9.lib
LIBS += $${ARIADIR}/lib/ArNetworkingVC9.lib



# Kinect directory
OPENNIDIR = C:/kinect/OpenNI

INCLUDEPATH += $${OPENNIDIR}/include $${OPENNIDIR}/include/Win32

LIBS += $${OPENNIDIR}/lib/openNI.lib




