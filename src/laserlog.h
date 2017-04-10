/*! \file laserlog.h
 */
#ifndef __LASER_LOG_H
#define __LASER_LOG_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "laserpoint.h"

#define DOUBLE_BUFFER_SIZE 400

typedef std::vector<LaserPoint> VLaserPoint; 

   //!
   /*!
    *
    */
struct LaserPose {
   //!
   /*!
    *
    */
  unsigned int count;
   //!
   /*!
    *
    */
  double x;
   //!
   /*!
    *
    */
  double y;
   //!
   /*!
    *
    */
  double th;
   //!
   /*!
    *
    */
  short dataSize;
};

   //!
   /*!
    *
    */
struct LaserLogData {
   //!
   /*!
    *
    */
  LaserPose   lp;
   //!
   /*!
    *
    */
  VLaserPoint vLP;
};

   //!
   /*!
    *
    */
class LaserLog {
  private:

   //!
   /*!
    *
    */
   std::vector<LaserLogData> vLLD; 
   //!
   /*!
    *
    */
   std::vector<LaserLogData>::iterator it; 

   //!
   /*!
    *
    */
   std::ifstream ifs;  // Archivo si existe
   //!
   /*!
    *
    */
   std::ofstream ofs;  // Archivo si no existe

   //!
   /*!
    *
    */
   std::string strFile;  // Nombre del archivo a abrir

  
  public:

   //!
   /*!
    *
    */
   LaserLog();

   //!
   /*!
    *
    */
   LaserLog(const char *fileName);

   //!
   /*!
    *
    */
   ~LaserLog();

   //!
   /*!
    *
    */
   LaserLogData lLD;

   //!
   /*!
    *
    */
   bool loaded;

   //!
   /*!
    *
    */
   void add();

   //!
   /*!
    *
    */
   bool load();

   //!
   /*!
    *
    */
   void save();

   //!
   /*!
    *
    */
   void begin();

   //!
   /*!
    *
    */
   void next();

   //!
   /*!
    *
    */
   bool isEnd();

   // Asigna el archivo de configuración
   void setFile(const char *fileName);

};

#endif

