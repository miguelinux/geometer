#include "landmark.h"

Landmark::Landmark(const char *sCascadeClassifier) {
  cascade = (CvHaarClassifierCascade*)cvLoad( sCascadeClassifier, 0, 0, 0 );
  if (!cascade) {
    ok = false;
    return ;
  }
  storage = cvCreateMemStorage(0);
  reset();
  ok = true;
  numFound=0;
}

Landmark::~Landmark() {
  if (ok) cvReleaseMemStorage( &storage );
}

void Landmark::reset(){
  for(short i=0; i<IMAGES_NUMBER; i++) {
    objFound[i] = OBJ_NOT_FOUND;
  }
}

std::vector<CvPoint3D64f> Landmark::getPose() {
   return vPos3D; 
}

unsigned short Landmark::addPose(double x, double y, double z) {
   std::vector<CvPoint3D64f>::iterator it;
   unsigned short nLandmark = 0, nLmFound=0;
   bool isNew = true;
   double d;
   for (it=vPos3D.begin(); it != vPos3D.end(); it++) {
	 d = dist3D(*it, cvPoint3D64f(x,y,z));
     if (d < MAX_DISTANCE) {
       isNew = false;       
	   nLmFound = nLandmark+1;
	 }
     nLandmark++;
   }
   if (isNew) {
     vPos3D.push_back(cvPoint3D64f(x,y,z));
     nLmFound++;
   }
   return nLmFound;
}

double Landmark::dist3D(CvPoint3D64f p1, CvPoint3D64f p2) {
  double x1 = p1.x - p2.x;
  double x2 = p1.y - p2.y;
  double x3 = p1.z - p2.z;
  return std::sqrt(x1 * x1 + x2 * x2 + x3 * x3);
}

