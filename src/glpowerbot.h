#ifndef _GL_POWERBOT_H
#define _GL_POWERBOT_H 

#ifdef WIN32
 #include <windows.h>
#endif
#include <GL/gl.h>

   //!
   /*!
    *
    */
class GlPowerBot {
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
  GlPowerBot();

   //!
   /*!
    *
    */
  ~GlPowerBot();

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

