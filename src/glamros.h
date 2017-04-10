#ifndef _GL_AMROS_H
#define _GL_AMROS_H

#include "glsick.h"
#include "glpowerbot.h"

   //!
   /*!
    *
    */
class GlAmros {
 private:
   //!
   /*!
    *
    */
  float posX;
   //!
   /*!
    *
    */
  float posY;
   //!
   /*!
    *
    */
  float posZ;
   //!
   /*!
    *
    */
  float posTh;

   //!
   /*!
    *
    */
  float scale;

 public:
   //!
   /*!
    *
    */
  GlAmros();
   //!
   /*!
    *
    */
  GlAmros(float scale);

   //!
   /*!
    *
    */
  ~GlAmros();

   //!
   /*!
    *
    */
  GlSick sick;
   //!
   /*!
    *
    */
  GlPowerBot pbot;

  // Se mueve a la posicion (nX, nY, nZ)
   //!
   /*!
    *
    */
  void moveTo(float nX, float nY, float nZ, float nTh);
  // Se mueve a la posicion relativa (nX, nY, nZ)
   //!
   /*!
    *
    */
  void moveRel(float nX=0, float nY=0, float nZ=0, float nTh=0);

  // Debuelve su posición en X
   //!
   /*!
    *
    */
  float getX();
  // Debuelve su posición en Y
   //!
   /*!
    *
    */
  float getY();
  // Debuelve su posición en Z
   //!
   /*!
    *
    */
  float getZ();
  // Debuelve su Angulo en grados
   //!
   /*!
    *
    */
  float getTh();

   //!
   /*!
    *
    */
  void draw();

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

};

#endif

