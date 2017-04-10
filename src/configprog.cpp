#include "configprog.h"

ConfigProg::ConfigProg(){
  boolFlag  = 0;
  rg = false;
  setFile("amros.conf");
}

ConfigProg::ConfigProg(const char *fileName){
  boolFlag  = 0;
  rg = false;
  setFile(fileName);
}

ConfigProg::~ConfigProg(){
}

void ConfigProg::open(){
  char buffer[200];

  #ifdef WIN32
  char delim = '\\';
  #else
  char delim = '/';
  #endif

  if (ofs.is_open()) ofs.close();
  if (ifs.is_open()) ifs.close();

  ifs.clear();
  ifs.open(strFile.c_str());
  if (! ifs.is_open()){
    ofs.open(strFile.c_str());
    if (ofs.is_open()) {
      ofs << "# Configuration file\n";
      ofs << "#\n";
      ofs << "# Log file name\n";
      ofs << "logfile=amros.log\n\n";
      ofs << "# Use Mobileyes simulator (1 or 0)\n";
      ofs << "usesim=1\n\n";
      ofs << "# Display rate (how many images will be seen, one of X)\n";
      ofs << "displayrate=15\n\n";
      ofs << "# Loading rate\n";
      ofs << "loadrate=800\n\n";
      ofs << "# Swap stereo cameras (yes=1 or no=0)\n";
      ofs << "swapstereo=0\n\n";
      ofs << "# Calibration file\n";
      ofs << "calibfile="<<
	     ".."<<delim<<".."<<delim<<"files"<< delim<<"camera_calibration.txt"<< "\n\n";
      ofs << "# Landmark training files (semicolon separated)\n";
      ofs << "lmtfile="  <<
	     ".."<<delim<<".."<<delim<<"files"<< delim<<"feuer.xml;" <<
	     ".."<<delim<<".."<<delim<<"files"<< delim<<"strahl.xml" << "\n\n";
      ofs << "# RemoteGeometer (0 = false; 1 = true)\n";
      ofs << "rg=1\n\n";
      ofs << "# kinect File\n";
      ofs << "kf=C:\\kinect\\OpenNI\\Data\\amros.xml"<< "\n\n";
      ofs.close();
    }
    ifs.clear();
    ifs.open(strFile.c_str());
  }
   
  while (! ifs.eof() ) {
    ifs.getline (buffer,200);
    if (buffer[0]=='#' || buffer[0]==';' || buffer[0]=='\0') continue;
    parse(std::string(buffer));
  }
  ifs.close();
}

void ConfigProg::setFile(const char *fileName){
  strFile=fileName;
  open();
}

void ConfigProg::parse(std::string linea){
  std::string comando;
  std::string valor;
  char com_val=1;
  for(unsigned short j=0;j<linea.size();j++){
    if(linea[j]==' ') continue;
    if(linea[j]=='='){ com_val=0; continue; }
    if(com_val) comando+=linea[j];
      else valor+=linea[j];
  }
  asign(comando,valor);
}

void ConfigProg::asign(std::string com, std::string val){
  std::string strTmp;
  if (com == "logfile") strLogFile = val;
  if (com == "displayrate")  { 
	  displayRate=std::atoi(val.c_str());
	  //displayRate=1 << displayRate;
  }
  if (com == "loadrate")  loadRate=std::atoi(val.c_str());
  if (com == "usesim" && val=="1")  boolFlag |= CONF_USE_SIM;
  if (com == "swapstereo" && val=="1")  boolFlag |= CONF_SWAP_ST;
  if (com == "calibfile") strCalibFile = val;
  if (com == "kf") kf= val; // KinectFile
  if (com == "rg") 
	  rg=(val=="1"); // Remote Geometer
  if (com == "lmtfile") {
    vStrClassifierFile.clear();
    for(unsigned short i=0; i< val.size(); i++) {
      if(val[i]==';') {
        vStrClassifierFile.push_back(strTmp);
	strTmp.clear();
	continue;
      }
      strTmp += val[i];
    }
    vStrClassifierFile.push_back(strTmp);
  }
}

