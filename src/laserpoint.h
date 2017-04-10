/*! \file laserpoint.h
 */
#ifndef __LASER_POINT_H
#define __LASER_POINT_H

   //!
   /*!
    *
    */
class LaserPoint {
  private:

   //!
   /*!
    *
    */
   double p[2];

  public:

   //!
   /*!
    *
    */
   LaserPoint();

   //!
   /*!
    *
    */
   LaserPoint(double x, double y);

   //!
   /*!
    *
    */
   ~LaserPoint();

   //!
   /*!
    *
    */
   double x();

   //!
   /*!
    *
    */
   double y();

   //!
   /*!
    *
    */
   void setX(double x);

   //!
   /*!
    *
    */
   void setY(double y);

};

#endif

