/*! \file mapbuilder.h
 */
#ifndef __MAP_BUILDER_H
#define __MAP_BUILDER_H

#include "Math/Math3d.h"

   //!
   /*!
    *
    */
class MapBuilder {

  private:

   //!
   /*!
    *
    */
   Vec3dList v3DLaserPoints;

   //!
   /*!
    *
    */
   double odoPosX;
   //!
   /*!
    *
    */
   double odoPosY;
   //!
   /*!
    *
    */
   double odoPosTh;

  public:

   //!
   /*!
    *
    */
   MapBuilder();

   //!
   /*!
    *
    */
   ~MapBuilder();

   //!
   /*!
    *
    */
   void setPoints(Vec3dList v3DL);

   //!
   /*!
    *
    */
   void p2l();

};

#endif

