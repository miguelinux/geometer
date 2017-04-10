#include "laserlog.h"
#include <ctime>

LaserLog::LaserLog() {

  char strTime[25];
  time_t rawtime;

  loaded=false;
  it=vLLD.begin();


  time ( &rawtime );

#ifdef WIN32
  struct tm  timeinfo;
  localtime_s (&timeinfo, &rawtime );
  sprintf_s(strTime, 25, "%4d_%2d_%2d_%2d_%02d_%02d.log", 1900+timeinfo.tm_year,
        timeinfo.tm_mon+1, timeinfo.tm_mday, timeinfo.tm_hour,
        timeinfo.tm_min, timeinfo.tm_sec);
#else
  struct tm  *timeinfo;
  timeinfo = localtime ( &rawtime );
  sprintf(strTime, "%4d_%2d_%2d_%2d_%02d_%02d.log", 1900+timeinfo->tm_year,
        timeinfo->tm_mon+1, timeinfo->tm_mday, timeinfo->tm_hour,
        timeinfo->tm_min, timeinfo->tm_sec);
#endif

  //setFile("laserlog.dat");
  setFile(strTime);
}

LaserLog::LaserLog(const char *fileName) {
  setFile(fileName);
  loaded=false;
  it=vLLD.begin();
}

LaserLog::~LaserLog() {
}

void LaserLog::add() {
   vLLD.push_back(lLD);
   lLD.vLP.clear();
}

bool LaserLog::load() {
  double dblBuffer[DOUBLE_BUFFER_SIZE];
  LaserLogData log;
  loaded=false;

  if (ofs.is_open()) ofs.close();
  if (ifs.is_open()) ifs.close();

  ifs.clear();
  ifs.open(strFile.c_str(), std::ios::in | std::ios::binary);

  if (ifs.is_open()){
    // clear vector laser log data
    vLLD.clear();
    while (! ifs.eof() ) {
      // Read header 
      ifs.read((char *)&log.lp, sizeof(LaserPose));
      // Read data 
      ifs.read((char *)dblBuffer, sizeof(double)*log.lp.dataSize);
      log.vLP.clear();
      for(short i=0; i<log.lp.dataSize; i+=2){
        log.vLP.push_back( LaserPoint( dblBuffer[i], dblBuffer[i+1]) );
      }
      vLLD.push_back(log);
    }
    ifs.close();
    loaded=true;
  }
  return loaded;
}

void LaserLog::save() {
  double dblBuffer[DOUBLE_BUFFER_SIZE];
  //std::vector<LaserLogData>::iterator it; 
  VLaserPoint::iterator ilp;
  short dataSize;

  if (vLLD.empty()) return;

  if (ofs.is_open()) ofs.close();
  if (ifs.is_open()) ifs.close();

  ofs.open(strFile.c_str(), std::ios::out | std::ios::trunc | std::ios::binary);
  if (ofs.is_open()) {
    for(it=vLLD.begin(); it != vLLD.end(); it++) {
      dataSize=0;
      for(ilp=it->vLP.begin(); ilp != it->vLP.end(); ilp++) {
        dblBuffer[dataSize++] =  ilp->x();
        dblBuffer[dataSize++] =  ilp->y();
      }
      it->lp.dataSize = dataSize;
      ofs.write((char *)&it->lp, sizeof(LaserPose));
      ofs.write((char *)dblBuffer, sizeof(double)*dataSize);
    }
    ofs.close();
    vLLD.clear();
  }
}

void LaserLog::setFile(const char *fileName) {
  strFile=fileName;
}

void LaserLog::begin(){
  if (vLLD.empty()) return;
  it=vLLD.begin();
  lLD = *it;
}

void LaserLog::next(){
  if (! isEnd() ) {
    it++;
    lLD = *it;
  }
  else {
    begin();
  }
}
   
bool LaserLog::isEnd(){
  return it == vLLD.end();
}

