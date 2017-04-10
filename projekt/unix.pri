# Define only if use SVS
#DEFINES += USE_SVS

# Where the ivt library is
IVTDIR = /opt/IVT

# Add it to the include path
INCLUDEPATH += $${IVTDIR}/src 

# Where the Open CV is located
INCLUDEPATH += /usr/local/include/opencv

# Libraries
# uncoment this if you are using il
#LIBS += -lil

LIBS += -L$${IVTDIR}/lib/linux -livtgui -livtvideocapture -livt -livtopencv
#LIBS += -L$${IVTDIR}/lib/linux-svs -livtgui -livtvideocapture -livt -livtopencv

# uncoment this if you are using OpenGL
LIBS += -lGL -lGLU -lglut

# uncoment this if you are using HighGUI
#LIBS += -lhighgui

# uncoment this if you are using OpenCV
#LIBS += -lcxcore -lcv -lcvaux
LIBS += -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_ml
LIBS += -lopencv_video -lopencv_features2d -lopencv_calib3d 
LIBS += -lopencv_objdetect -lopencv_contrib -lopencv_legacy -lopencv_flann


# uncoment this if you are using libdc1394 v1
#LIBS += -lraw1394 -ldc1394_control

# uncoment this if you are using libdc1394 v2
LIBS += -lraw1394 -ldc1394

# uncoment this if you are using SVS
#LIBS += -L/usr/local/svs/bin -lsvs -lsvscalc -lsvscap


# Aria directory
ARIADIR = /usr/local/Aria

# Aria headers
INCLUDEPATH += $${ARIADIR}/include $${ARIADIR}/ArNetworking/include
 
# Aria libraries
LIBS += -L$${ARIADIR}/lib -lAria -lArNetworking

