#ifndef _GL_SICK_H
#define _GL_SICK_H 

#ifdef WIN32
 #include <windows.h>
#endif
#include <GL/gl.h>

   //!
   /*!
    *
    */
#define GL_SICK_LASER_ON 0x01000000

   //!
   /*!
    *
    */
#define SICK_WIDTH  155
   //!
   /*!
    *
    */
#define SICK_HEIGHT 185
   //!
   /*!
    *
    */
#define SICK_DEEP   156

   //!
   /*!
    *
    */
#define SICK_CENTRE_OF_MIRROR_D 93
   //!
   /*!
    *
    */
#define SICK_CENTRE_OF_LIGHT_W  77.5
   //!
   /*!
    *
    */
#define SICK_CENTRE_OF_LIGHT_H  63


   //!
   /*!
    *
    */
class GlSick {
 private:
   //!
   /*!
    *
    */
  int conf;
   //!
   /*!
    *
    */
  short laserAng;
   //!
   /*!
    *
    */
  bool bLDir;
   //!
   /*!
    *
    */
  float scale;

   //!
   /*!
    *
    */
  GLuint oglList;

 public:

   //!
   /*!
    *
    */
  GlSick();

   //!
   /*!
    *
    */
  ~GlSick();

   //!
   /*!
    *
    */
  void setup();

   //!
   /*!
    *
    */
  void setScale(float s);

   //!
   /*!
    *
    */
  void draw();

   //!
   /*!
    *
    */
  void setConfig(int c);

};

#endif

