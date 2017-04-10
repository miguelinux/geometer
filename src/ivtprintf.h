/*! \file ivtprintf.h
 *   This is the header 
 */
#ifndef _IVT_PRINTF_H
#define _IVT_PRINTF_H

#include "Math/Math2d.h"
#include "Math/Math3d.h"

   //!
   /*!
    *
    */
class IVTPrintf {
  public:
   //!
   /*!
    *
    */
    static void p(Vec2d v, const char *str = 0);
   //!
   /*!
    *
    */
    static void p(Mat2d v, const char *str = 0);
   //!
   /*!
    *
    */
    static void p(Vec3d v, const char *str = 0);
   //!
   /*!
    *
    */
    static void p(Mat3d v, const char *str = 0);
};

#endif

