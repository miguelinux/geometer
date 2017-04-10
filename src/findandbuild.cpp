#include "findandbuild.h"
#include <cstdlib>

FindAndBuild::FindAndBuild() {
   ivtImages[L_IMG] = 0; ivtImages[R_IMG] = 0;
   cvImages[L_IMG] = 0; cvImages[R_IMG] = 0;

   setParam();
   setCanny();


   objRegion[L_IMG].nPixels = -1;
   objRegion[L_IMG].min_x=objRegion[L_IMG].min_y=0;
   objRegion[L_IMG].max_x=objRegion[L_IMG].max_y=0;

   objRegion[R_IMG].nPixels = -1;
   objRegion[R_IMG].min_x=objRegion[R_IMG].min_y=0;
   objRegion[R_IMG].max_x=objRegion[R_IMG].max_y=0;

   vP3D.clear();
   vP2D[L_IMG].clear();
   vP2D[R_IMG].clear();
   stereoCal = 0;
   posX = posY = posZ = posTh = 0.0f;
   paperMode = false;
}

FindAndBuild::FindAndBuild(CByteImage *leftImage, CByteImage *rightImage,
		short winSize, short nDispThreshold,
		float fMatchThreshold, bool bInputImagesAreUndistorted) {

   ivtImages[L_IMG] = 0; ivtImages[R_IMG] = 0;
   cvImages[L_IMG] = 0; cvImages[R_IMG] = 0;

   setImg(leftImage,L_IMG);
   setImg(rightImage,R_IMG);

   setParam(winSize, nDispThreshold, fMatchThreshold,
       bInputImagesAreUndistorted);
   setCanny();

   objRegion[L_IMG].nPixels = -1;
   objRegion[L_IMG].min_x=objRegion[L_IMG].min_y=0;
   objRegion[L_IMG].max_x=objRegion[L_IMG].max_y=0;

   objRegion[R_IMG].nPixels = -1;
   objRegion[R_IMG].min_x=objRegion[R_IMG].min_y=0;
   objRegion[R_IMG].max_x=objRegion[R_IMG].max_y=0;

   vP3D.clear();
   vP2D[L_IMG].clear();
   vP2D[R_IMG].clear();
   stereoCal = 0;
   posX = posY = posZ = posTh = 0.0f;
}

FindAndBuild::~FindAndBuild() {
  vP3D.clear();
  vP2D[L_IMG].clear();
  vP2D[R_IMG].clear();
  cvReleaseImageHeader(&cvImages[L_IMG]);
  cvReleaseImageHeader(&cvImages[R_IMG]);
}

void FindAndBuild::setImg(CByteImage *image, short nImage) {
  if (!image) return;
  //if (image->type != CByteImage::eGrayScale) {
    //return;
  //}
   ivtImages[nImage]=image;
   cvImages[nImage] = IplImageAdaptor::Adapt(image);
}

void FindAndBuild::setParam(short winSize, short nDispThreshold,
       float fMatchThreshold,
       bool bInputImagesAreUndistorted) {
   nWindowSize=winSize;
   dispThreshold = nDispThreshold;
   matchThreshold=fMatchThreshold;
   inputImagesAreUndistorted=bInputImagesAreUndistorted;
}

void FindAndBuild::setCanny(unsigned char nLowThreshold,
		unsigned char nHighThreshold) {
   cannyLowThreshold=nLowThreshold;
   cannyHighThreshold=nHighThreshold;
   //cvCanny(pIplInputImage, pIplOutputImage, nLowThreshold, nHighThreshold);
}

/*
For a faster operation on real video images the settings are: scale_factor=1.2, min_neighbors=2, flags=CV_HAAR_DO_CANNY_PRUNING, min_size=<minimum possible face size> (for example, ~1/4 to 1/16 of the image area in case of video conferencing). 
	   
*/

void FindAndBuild::setStereoCalibration(CStereoCalibration *stCal) {
  stereoCal = stCal;

  Vec2d left, right;
  Vec3d p;

  Math3d::AddVecVec(stereoCal->GetLeftCalibration()->m_translation_inverse,
		    stereoCal->GetRightCalibration()->m_translation_inverse, p);
  Math3d::MulVecScalar(p, 0.5f, p);

  centerDeep = p.z;
  for(short i=0; i<NUM_OF_DISP; i++){
    p.z = centerDeep + i*INC_DEEP + INITIAL_DEEP;
    stereoCal->GetLeftCalibration()->GetCameraCoordinates(p, left);
    stereoCal->GetRightCalibration()->GetCameraCoordinates(p, right);
    dispProx[i] = short(Math2d::Distance(left, right) + 0.5f);
    //printf("%D(%d)=%d->%10.2f\n",i,dispProx[i],Math2d::Distance(left, right));
  }
}


short FindAndBuild::find(Landmark *lm, short nImg) {
  if ( !ivtImages[nImg] && !lm->cascade) {
    return 0;
  }

  double scale = 1.3;

  lm->objFound[nImg] = OBJ_NOT_FOUND;

  IplImage* small_img = cvCreateImage( cvSize( cvRound (
		ivtImages[nImg]->width/scale), cvRound (
		ivtImages[nImg]->height/scale)),
		8 /*depth*/, 1 /*bytes per pixel*/);
  cvResize( cvImages[nImg], small_img, CV_INTER_LINEAR );
  cvEqualizeHist( small_img, small_img );
  cvClearMemStorage( lm->storage );

  CvSeq* objsFound = cvHaarDetectObjects( small_img, lm->cascade,
		  lm->storage,1.1, 2, 0 /*CV_HAAR_DO_CANNY_PRUNING*/,
		  cvSize(30, 30) );

  //for(int i = 0; i < (objsFound ? objsFound->total : 0); i++ ) {
  // nPixels is used as the total of objects found
  // BORRAR objRegion[nImg].nPixels = (objsFound ? objsFound->total : 0);
  lm->objFound[nImg] = (objsFound ? objsFound->total : OBJ_NOT_FOUND);
  if (lm->objFound[nImg] > 0) {
    CvRect* r = (CvRect*)cvGetSeqElem( objsFound, 0/*i*/ );

    lm->region[nImg].x = cvRound(r->x*scale);
    lm->region[nImg].y = cvRound(r->y*scale);
    lm->region[nImg].width  = cvRound(r->width *scale);
    lm->region[nImg].height = cvRound(r->height*scale);

    /* BORRAR
    objRegion[nImg].min_x=cvRound(r->x*scale);
    objRegion[nImg].min_y=cvRound(r->y*scale);

    objRegion[nImg].max_x=cvRound((r->x + r->width)*scale);
    objRegion[nImg].max_y=cvRound((r->y + r->height)*scale);

    objRegion[nImg].centroid.x = cvRound((r->x + r->width/2)*scale);
    objRegion[nImg].centroid.y = cvRound((r->y + r->height/2)*scale);
    */
  }

  return lm->objFound[nImg];
}

void FindAndBuild::extractPoints(short nImg) {

  MyRegion objRegion;
  unsigned char *pix; // Pointer to pixels
  Vec2d p2D; // 2D image point


  // Create the process and temporary images
  procImg = new CByteImage( lndmrk->region[nImg].width,
                            lndmrk->region[nImg].height,
			    CByteImage::eGrayScale);
  CByteImage tmpImg(procImg);

  objRegion.min_x=lndmrk->region[nImg].x;
  objRegion.min_y=lndmrk->region[nImg].y;
  objRegion.max_x=lndmrk->region[nImg].x + lndmrk->region[nImg].width  -1;
  objRegion.max_y=lndmrk->region[nImg].y + lndmrk->region[nImg].height -1;

  // Copy the region of the object
  ImageProcessor::CopyImage(ivtImages[nImg], &tmpImg, &objRegion);

  // Getting the edges
  ImageProcessorCV::Canny(&tmpImg, procImg,
		  cannyLowThreshold, cannyHighThreshold);

  vP2D[nImg].clear();
  pix = procImg->pixels;
  for (short y=0; y < procImg->height; y++) {
    for (short x=0; x < procImg->width; x++) {
      if (*pix++) {
	//Obtain Canny points
	Math2d::SetVec(p2D, float(x + objRegion.min_x),
			    float(y + objRegion.min_y));
	vP2D[nImg].push_back(p2D);
      } // if (*pix++) 
    } // for x
  } // for y

  delete procImg;
}


bool FindAndBuild::build(Landmark *landmark) {

  if (landmark->objFound[L_IMG] == OBJ_NOT_FOUND &&
      landmark->objFound[R_IMG] == OBJ_NOT_FOUND ) return false;

  // Centroid left and right
  Vec2d punkt[2];
  Vec3d worldPoint;
  double mX,mY,mZ, c, s;
  int diff=0, nPunkt;
  unsigned short nLandmarks;
  
  lndmrk = landmark;

  diff = std::abs(lndmrk->region[L_IMG].width - lndmrk->region[R_IMG].width);
  if (diff > THRESHOLD_DIFF_AREA) return false;

  diff = std::abs(lndmrk->region[L_IMG].height - lndmrk->region[R_IMG].height);
  if (diff > THRESHOLD_DIFF_AREA) return false;

  // Extract the Canny points
  extractPoints(L_IMG); 
  //extractPoints(R_IMG); 

  Math2d::SetVec(punkt[L_IMG], float(lndmrk->region[L_IMG].x + lndmrk->region[L_IMG].width /2.0f),
		       float(lndmrk->region[L_IMG].y + lndmrk->region[L_IMG].height/2.0f));
  Math2d::SetVec(punkt[R_IMG], float(lndmrk->region[R_IMG].x + lndmrk->region[R_IMG].width /2.0f),
		       float(lndmrk->region[R_IMG].y + lndmrk->region[R_IMG].height/2.0f));

  lndmrk->cenDisp = short(Math2d::Distance(punkt[L_IMG],punkt[R_IMG]));

  // Finding disparity approximately(diff used as a counter)
  for(diff=0; diff<NUM_OF_DISP; diff++){
    if (lndmrk->cenDisp > dispProx[diff]) break;
  }
  if (diff == NUM_OF_DISP) {
    printf("***** END MEASURE *******\n");
  }

  vP3D.clear();
  nPunkt = 0;
  mX = mY = mZ = 0.0;
  for(Vec2dList::iterator it = vP2D[L_IMG].begin(); it != vP2D[L_IMG].end(); it++) {
    diff = matchpoint(ivtImages[L_IMG], ivtImages[R_IMG],
		      int(it->x+0.5), int(it->y+0.5),
		      31,   // windows size 1-49, 21
		      dispProx[diff-1]*2,// minDisp 
		      dispProx[diff]*2,  // maxDisp
		      punkt[R_IMG], worldPoint, 0.1f);
    if (diff != NOT_FOUND_MATCH) {
      nPunkt++;
      mX += worldPoint.x;
      mY += worldPoint.y;
      mZ += worldPoint.z;
      // TODO uncomment for only demostration
      vP3D.push_back(worldPoint);
    }
    // Taking only 1 of 2 points
    it++;
    if (it == vP2D[L_IMG].end()) break;
  } // for Vec2dList

  mX/=(double)nPunkt;
  mY/=(double)nPunkt;
  mZ/=(double)nPunkt;

  c = std::cos(posTh);
  s = std::sin(posTh);

  printf("Landmark pose (%5f,%5f,%5f)\n",mX,mY,mZ);

  nLandmarks = lndmrk->addPose(
   /* Original expresion
   c*mX + s*mZ + c*GLOBAL_CAM_X + s*GLOBAL_CAM_Z - GLOBAL_CAM_X + posX + GLOBAL_CAM_X,
   mY + posY + GLOBAL_CAM_Y,
   -s*mX + c*mZ - s*GLOBAL_CAM_X + c*GLOBAL_CAM_Z - GLOBAL_CAM_Z + posZ + GLOBAL_CAM_Z,
   */
   c*mX + s*mZ + c*GLOBAL_CAM_X + s*GLOBAL_CAM_Z + posX,
   mY + posY + GLOBAL_CAM_Y,
   -s*mX + c*mZ - s*GLOBAL_CAM_X + c*GLOBAL_CAM_Z + posZ
  );

  if (lndmrk->numFound < nLandmarks) {
    lndmrk->numFound++;
  }

  printf("landmark: %d id: %d(%d) deep:%10.2f\n",
	  nLandmarks, lndmrk->id, lndmrk->numFound, mZ);

  return true;
}

void FindAndBuild::clearRegion(CByteImage *img, short nImg, short color) {
  CByteImage *pImg = img;
  short row;
  //short offset = 3 * (y * pImg->width + x);

  if (img->type == CByteImage::eGrayScale) {
    pImg->pixels += (lndmrk->region[nImg].y * pImg->width +
		     lndmrk->region[nImg].x);
    for(row=0; row < lndmrk->region[nImg].height; row++) {
      memset(pImg->pixels, color, lndmrk->region[nImg].width);
      pImg->pixels += pImg->width;
    }
  } else if (img->type == CByteImage::eRGB24) {
    pImg->pixels += 3 * (lndmrk->region[nImg].y * pImg->width +
		    lndmrk->region[nImg].x);
    for(row=0; row < lndmrk->region[nImg].height; row++) {
      memset(pImg->pixels, color, 3 * lndmrk->region[nImg].width);
      pImg->pixels += 3 * pImg->width;
    }
  }
}

void FindAndBuild::showPoints(CByteImage *img, short nImg, short r, short g, short b) {

  MyRegion objRegion;
  objRegion.min_x=lndmrk->region[nImg].x;
  objRegion.min_y=lndmrk->region[nImg].y;
  objRegion.max_x=lndmrk->region[nImg].x + lndmrk->region[nImg].width  -1;
  objRegion.max_y=lndmrk->region[nImg].y + lndmrk->region[nImg].height -1;

  if (paperMode) {
    //clearRegion(img,nImg);
    PrimitivesDrawer::DrawRegion (img, objRegion, 255, 255, 255, -1);
  }
  PrimitivesDrawer::DrawRegion (img, objRegion, 0, 0);

  for(Vec2dList::iterator it = vP2D[nImg].begin(); it != vP2D[nImg].end(); it++) {
    PrimitivesDrawer::DrawPoint(img,*it,r,g,b);
  }
}

int FindAndBuild::matchpoint(const CByteImage *pLeftImage,
		const CByteImage *pRightImage,
		int x, int y, int nWindowSize,
	int d1, int d2, Vec2d &result, Vec3d &result_3d,
	float fThreshold, bool bInputImagesAreUndistorted)
{
	if (d1 < 0) d1 = 0;
	if (d2 < 0) d2 = 0;
	if (d1 >= pLeftImage->width) d1 = pLeftImage->width - 1;
	if (d2 >= pLeftImage->width) d2 = pLeftImage->width - 1;
	
	float *values = new float[pLeftImage->width];
	const int best_d = SingleZNCC(pLeftImage, pRightImage, x, y, nWindowSize, d1, d2, values);

	if (best_d != -1 && values[best_d] > fThreshold)
	{
		const double y0 = values[best_d - 1];
		const double y1 = values[best_d];
		const double y2 = values[best_d + 1];
		const double xmin = (y0 - y2) / (2 * (y0 - 2 * y1 + y2));
		const double disparity = best_d + xmin;
		
		float m, c;
		Vec2d point_left = { float(x), float(y) };
		stereoCal->CalculateEpipolarLineInRightImage(point_left, m, c);
		
		result.x = float(x - disparity);
		result.y = float(m * (x - disparity) + c);
		
		stereoCal->Calculate3DPoint(point_left, result, result_3d, false, !bInputImagesAreUndistorted);

		delete [] values;
		
		return best_d;
	}
	
	delete [] values;
	
	return NOT_FOUND_MATCH;
}

int FindAndBuild::SingleZNCC(const CByteImage *pInputImage1,
	const CByteImage *pInputImage2, int x, int y, int nWindowSize,
	int d1, int d2, float *values)
{
	const int width = pInputImage1->width;
	const int height = pInputImage1->height;
	
	if (x < nWindowSize / 2 || x >= width - nWindowSize / 2 || y < nWindowSize / 2 || y >= height - nWindowSize / 2)
		return -1;
	
	const unsigned char *input_left = pInputImage1->pixels;
	unsigned char *input_right = pInputImage2->pixels;
	const int nVectorLength = nWindowSize * nWindowSize;
	
	float *vector1 = new float[nVectorLength];
	float *vector2 = new float[nVectorLength];
	
	const int offset = (y - nWindowSize / 2) * width + (x - nWindowSize / 2);
	const int diff = width - nWindowSize;
	Vec2d camera_point = { float(x), float(y) };
	int k;
	
	// calculate mean value
	float mean = 0;
	{
		for (int y = 0, offset2 = offset, offset3 = 0; y < nWindowSize; y++, offset2 += diff)
			for (int x = 0; x < nWindowSize; x++, offset2++, offset3++)
			{
				vector1[offset3] = input_left[offset2];
				mean += vector1[offset3];
			}
	}
	mean /= nVectorLength;
	
	// apply additive and muliplicative normalization
	float factor = 0;
	for (k = 0; k < nVectorLength; k++)
	{
		vector1[k] -= mean;
		factor += vector1[k] * vector1[k];
	}
	
	if (factor < 60 * nVectorLength)
		return -1;
	
	factor = 1 / sqrtf(factor);
	for (k = 0; k < nVectorLength; k++)
		vector1[k] *= factor;
	
	float best_value = FLT_MIN;
	int d, best_d = 0;

	const int max_d = d2 < x ? d2 : x;
	
	float m, c;
	stereoCal->CalculateEpipolarLineInRightImage(camera_point, m, c);
	
	// determine correspondence
	for (d = d1; d <= max_d; d++)
	{
		const int yy = int(m * (x - d) + c + 0.5) - nWindowSize / 2;
		
		if (yy < 0 || yy >= height)
			continue;
		
		const int offset_right = yy * width + (x - d - nWindowSize / 2);
		// by Mike
		//const int offset_diff = offset_right - offset;
		
		// determine mean value
		float mean = 0;
		for (int y = 0, offset2 = offset_right, offset3 = 0; y < nWindowSize; y++, offset2 += diff)
			for (int x = 0; x < nWindowSize; x++, offset2++, offset3++)
			{
				vector2[offset3] = input_right[offset2];
				mean += vector2[offset3];
				
			}
		mean /= nVectorLength;
		
		// apply additive and multiplicative normalization
		float factor = 0;
		for (k = 0; k < nVectorLength; k++)
		{
			vector2[k] -= mean;
			factor += vector2[k] * vector2[k];
		}

		if (factor < 60 * nVectorLength)
			continue;

		factor = 1 / sqrtf(factor);
		for (k = 0; k < nVectorLength; k++)
			vector2[k] *= factor;
			
		float value = 0;
		for (k = 0; k < nVectorLength; k++)
			value += vector1[k] * vector2[k];
		
		// save correlation results for subpixel calculations
		values[d] = value;
		
		// determine maximum correlation value
		if (value > best_value)
		{
			best_value = value;
			best_d = d;
		}
	}
	
	delete [] vector1;
	delete [] vector2;
	
	return best_d;
}

void FindAndBuild::moveTo(double nX, double nY, double nZ, double nTh){
  posX  = nX;
  posY  = nZ; // Changed to the camera coordinate system
  posZ  = nY; // Changed to the camera coordinate system
  posTh = -nTh/180*M_PI; // Changed to negative and radians
}

void FindAndBuild::moveRel(double nX, double nY, double nZ, double nTh){
  posX  += nX;
  posY  += nZ; // Changed to the camera coordinate system
  posZ  += nY; // Changed to the camera coordinate system
  posTh += -nTh/180*M_PI; // Changed to negative and radians


  while (posTh<0)   posTh+=360;
  while (posTh>360) posTh-=360;
}


