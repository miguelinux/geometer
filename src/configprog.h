#ifndef _CONFIGPROG_H
#  define _CONFIGPROG_H

#  include <fstream>
#  include <iostream>
#  include <string>
#  include <vector>

#  include <cstdlib>

#  define CONF_USE_SIM 0x0001
#  define CONF_SWAP_ST 0x0002

   //!
   /*!
    *
    */
class ConfigProg {
  private:
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

    // divide la linea en comando y valor
   //!
   /*!
    *
    */
    void parse(std::string linea); 
    // asigna el valor al comando
   //!
   /*!
    *
    */
    void asign(std::string com, std::string val); 

  public:

   //!
   /*!
    *
    */
    ConfigProg();
   //!
   /*!
    *
    */
    ConfigProg(const char *fileName);
   //!
   /*!
    *
    */
    ~ConfigProg();
   
    // Variables de la configuración
   //!
   /*!
    *
    */
    short boolFlag;	
   //!
   /*!
    *
    */
    unsigned int displayRate;
   //!
   /*!
    *
    */
    unsigned int loadRate;
    
   //!
   /*!
    *
    */
    std::string strLogFile;
   //!
   /*!
    *
    */
    std::string strCalibFile;

   //!
   /*!
    *
    */
    std::string kf;  

   //!
   /*!
    *
    */
    bool rg;  

   //!
   /*!
    *
    */
    std::vector<std::string> vStrClassifierFile;

    // Abre el archivo y lo lee
   //!
   /*!
    *
    */
    void open();	  
    // Asigna el archivo de configuración
   //!
   /*!
    *
    */
    void setFile(const char *fileName);

};

#endif

