/*! \file commonvars.h
 *   This is the header file where is defined the common variables
 *   in the project.
 */
#ifndef COMMON_VARS_H
#define COMMON_VARS_H

/*! \def PGR_VER_STR
 *  Is the Program version in string mode
 */
#define PGR_VER_STR "v.0.0.16"
/*! \def DSP_IMG_WIDTH
 *  Is the with of the image to be displayed in the main window.
 */
#define DSP_IMG_WIDTH   320
/*! \def DSP_IMG_HEIGHT
 *  Is the height of the image to be displayed in the main window.
 */
#define DSP_IMG_HEIGHT  240

/*! \def MAX_HARRIS_POINTS
 *  Is the number of interestest points in Harris filter
 */
#define MAX_HARRIS_POINTS  1000

/*! \def MAX_LANDMARKS_NUM 
 *  Is the maximum number of landmarks
 */
#define MAX_LANDMARKS_NUM 10

/*! \def M_PI 
 *  Definition of M_PI value in win32 systems (3.14159265358979323846).
 */
#ifndef M_PI
#define M_PI        3.14159265358979323846
#endif

/*! \def GLOBAL_CAM_X 
 *  Camera position X repect to the center of coordinates of the laser
 */
#define GLOBAL_CAM_X -20

/*! \def GLOBAL_CAM_Y 
 *  Camera position Y repect to the center of coordinates of the laser
 */
#define GLOBAL_CAM_Y -670

/*! \def GLOBAL_CAM_Z 
 *  Camera position Z repect to the center of coordinates of the laser
 */
#define GLOBAL_CAM_Z 340


/*! \def QTBYTE_ALPHA32
 *  Is the position of the alpha byte value in a Qt widget in a 32bit word.
 */
#define QTBYTE_ALPHA32 0xFF000000
/*! \def QTBYTE_RED32
 *  Is the position of the red byte value in a Qt widget in a 32bit word.
 */
#define QTBYTE_RED32   0x00FF0000
/*! \def QTBYTE_GREEN32
 *  Is the position of the green byte value in a Qt widget in a 32bit word.
 */
#define QTBYTE_GREEN32 0x0000FF00
/*! \def QTBYTE_BLUE32
 *  Is the position of the blue byte value in a Qt widget in a 32bit word.
 */
#define QTBYTE_BLUE32  0x000000FF

/************************************************************************/
/*! \brief FLAGS CONSTANT
 *  0xmffff0ii
 *  where
 *  m are mouse events
 *  f are flag values
 *  i are 255 posible combination values (images, togles button, etc).
 */


/*! \def OPEN_CAMERA  
 *  Open camera flag, it is set if the camera is open.
 */
#define OPEN_CAMERA  0x00010000
/*! \def CAPTURE_IMG  
 *  Capture image flag, it is set when the capture start.
 */
#define CAPTURE_IMG  0x00020000
/*! \def COLOR_IMAGE  
 *  Color image flag, it is set if the image is a RGB image
 */
#define COLOR_IMAGE  0x00040000
/*! \def SAVE_IMG_SEC
 *  Image sequence save flag, it is set when one want to save an image sequence
 */
#define SAVE_IMG_SEQ 0x00080000
/*! \def LOAD_IMG_SEQ
 *  Image sequence load flag, it is set when one want to load an image sequence
 */
#define LOAD_IMG_SEQ 0x00100000
/*! \def SHOW_CAP_IMG
 *  Image sequence load flag, it is set when one want to load an image sequence
 */
#define SHOW_CAP_IMG 0x00200000

/*! \def CAMERA_PARAM
 *  Is set when we have loaded the paremeters of the camera
 */
#define CAMERA_PARAM 0x00400000

/*! \def HARRIS_FILTER
 *  Defied when wants apply harris' filter on a image
 */
#define HARRIS_FILTER 0x00800000
/*! \def CANNY_FILTER
 *  Defied when wants apply cannys' filter on a image
 */
#define CANNY_FILTER  0x01000000
/*! \def POSE_PROC
 *  Defied when wants apply the pose procedure in images
 */
#define POSE_PROC     0x02000000
/*! \def ANY_FILTER
 *  Defined as flag of any filter applied
 */
#define ANY_FILTER    0x01800000

/*! \def ROBOT_LASER
 *  Defined when wants to enable the laser readings
 */
#define ROBOT_LASER   0x04000000

/*! \def GEOMETER_GET_LASER
 *  This flag is set when wants to get the laser data.
 */
#define GEOMETER_GET_LASER 0x08000000


/*! \def MOUSE_CLICK  
 *  Identificator of the mouse click event 
 */
#define MOUSE_CLICK  0x10000000
/*! \def MOUSE_MOVE   
 *  Identificator of the mouse move event 
 */
#define MOUSE_MOVE   0x20000000
/*! \def MOUSE_DBLCLK 
 *  Identificator of the mouse double click event 
 */
#define MOUSE_DBLCLK 0x40000000

/*! \def MOUSE_ANY
 *  Identificator of any mouse event
 */
#define MOUSE_ANY    0x70000000

/*! \def IMG_LEFT_CLK
 *  Set if Left Image Clicked
 */
#define IMG_LEFT_CLK  0x00008000
/*! \def IMG_RIGHT_CLK
 *  Set if Right Image Clicked
 */
#define IMG_RIGHT_CLK 0x00004000
/*! \def IMG_BOTH_CLK
 *  Flag of two images clicked
 */
#define IMG_BOTH_CLK  0x0000C000
/*! \def CLICK_3D_ACT
 *  Flag Action to start click on images to get 3D
 */
#define CLICK_3D_ACT  0x00002000
/*! \def CLICK_MATCH
 *  Flag Action to start click on left image to get 3D
 */
#define CLICK_MATCH   0x00001000


/*! \def DSP_IMG_LEFT
 *  Is the number of the Image in the program. Using these values
 *  to be comptabiel with the configuration variable.
 *  This is the left image to be displayed
 */
#define DSP_IMG_LEFT  0x00000000
/*! \def DSP_IMG_RIGHT
 *  Is the number of the Image in the program. Using these values
 *  to be comptabiel with the configuration variable.
 *  This is the right image to be displayed
 */
#define DSP_IMG_RIGHT 0x00000001
/*! \def CAP_IMG_LEFT
 *  Is the number of the Image in the program. Using these values
 *  to be comptabiel with the configuration variable.
 *  This is the left image captured from the camera.
 */
#define CAP_IMG_LEFT  0x00000002
/*! \def CAP_IMG_RIGHT
 *  Is the number of the Image in the program. Using these values
 *  to be comptabiel with the configuration variable.
 *  This is the right image captured from the camera.
 */
#define CAP_IMG_RIGHT 0x00000003
/*! \def ANY_IMAGE
 *  Flag for all the images
 */
#define ANY_IMAGE     0x00000003

/*! \def LOAD_CONFIG
 *  This flag is set when wants to reload the configuration file.
 */
#define LOAD_CONFIG   0x00000004
/*! \def LOAD_IMAGE
 *  This flag is set when wants to load a single image.
 */
#define LOAD_IMAGE    0x00000005

/*! \def ROBOT_CONNECT
 *  This flag is set when wants to connecto to the robot server.
 */
#define ROBOT_CONNECT 0x00000006

/*! \def ROBOT_STOP
 *  This flag is set when wants to stop movement of the robot.
 */
#define ROBOT_STOP    0x00000007

/*! \def ROBOT_LEFT
 *  This flag is set when wants to turn robot left.
 */
#define ROBOT_LEFT    0x00000008

/*! \def ROBOT_RIGHT
 *  This flag is set when wants to turn robot right.
 */
#define ROBOT_RIGHT   0x00000009

/*! \def ROBOT_UP
 *  This flag is set when wants to move the robot forwards
 */
#define ROBOT_UP      0x0000000A

/*! \def ROBOT_DOWN
 *  This flag is set when wants to move the robot backwards
 */
#define ROBOT_DOWN    0x0000000B

/*! \def ROBOT_SPEED_DOWN
 *  This flag is set when wants to decrement the velocity
 */
#define ROBOT_SPEED_DOWN 0x0000000C

/*! \def ROBOT_SPEED_UP
 *  This flag is set when wants to increment the velocity
 */
#define ROBOT_SPEED_UP   0x0000000D

/*! \def ROBOT_INFO
 *  This flag is set when wants to know the robot voltage, speed, and position
 */
#define ROBOT_INFO    0x0000000E

/*! \def LASER_P2L
 *  This flag is set when wants to convert the laser points to line
 */
#define LASER_P2L     0x0000000F

/*! \def GEOMETER_CONNECT
 *  This flag is set when wants to connecto to geometer.
 */
#define GEOMETER_CONNECT 0x00000010

/*! \def GEOMETER_LASER
 *  This flag is set when wants to connecto the laser in geometer.
 */
#define GEOMETER_LASER 0x00000011

#define GL_KINECT  0x00000012
#define _GL_KINECT 0x00000001

#define GL_REPAINT  0x00000013
#define _GL_REPAINT 0x00000002

#define _STEP  0x00000004

#define K_UNO  0x00000014
#define K_DOS  0x00000015
#define K_TRES 0x00000016
#define K_CUAT 0x00000017
#define K_CINC 0x00000018
#define K_SEIS 0x00000019

#endif

