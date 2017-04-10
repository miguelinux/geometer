/***************************************************************************
 *   Copyright (C) 2011 by gkwh (Heriberto Casarrubias Vargas)             *
 *   heribertocv@gmail.com                                                 *
 *                                                                         *
 *   Computer Vision & Robotics Research Group                             *
 *   Department of Automatic Control                                       *
 *   CINVESTAV-Guadalajara, México.                                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "rgbdimage.h"
#include <fstream>
#include <cstring>
#include <vector>

//---------------------------------------------------------------
const uint8_t rgbdColor [][3]={
  {0xFF, 0x00, 0x00}, //  0 Red
  {0x00, 0xFF, 0x00}, //  1 Green
  {0x00, 0x00, 0xFF}, //  2 Blue
  {0xFF, 0xFF, 0x00}, //  3 Yellow
  {0xFF, 0x00, 0xFF}, //  4 Magenta
  {0x00, 0xFF, 0xFF}, //  5 Cyan
  {0x7F, 0x00, 0x00},
  {0x00, 0x7F, 0x00},
  {0x00, 0x00, 0x7F},
  {0x7F, 0x7F, 0x00},
  {0x7F, 0x00, 0x7F},
  {0x00, 0x7F, 0x7F}
};

void setMyColor(uint8_t *i, short c)
{
  *i++ = rgbdColor[c][0]; 
  *i++ = rgbdColor[c][1]; 
  *i++ = rgbdColor[c][2]; 
}
//---------------------------------------------------------------
int qsort_compare_val (const void * a, const void * b)
{
  // Ascendentemente
  //return ( ((depth_color_info_t *)a)->val - ((depth_color_info_t *)b)->val );
  // Descendentemente
  return ( ((depth_color_info_t *)b)->val - ((depth_color_info_t *)a)->val );
}
//---------------------------------------------------------------
int qsort_compare_dist (const void * a, const void * b)
{
  // Ascendentemente
  return ( ((depth_color_info_t *)a)->dist - ((depth_color_info_t *)b)->dist );
}
//---------------------------------------------------------------
RGBDImage::RGBDImage(size_t width, size_t height):
m_width(width), m_height(height), _640x480(640*480)
{
	m_cvImageRGB = NULL;
	m_cvImageDepth = NULL;
	m_cvImageDepthUI16 = NULL;
	m_bufferDepthVisualizeUI8 = NULL;
	m_bufferDepthVisualizeUI16 = NULL;
	m_cloudPoints3D = NULL;

    dumpStats = false;
    hmax = 0;
    m_3dcloud = 0;
}

//---------------------------------------------------------------
RGBDImage::~RGBDImage()
{
	clearInternalMembers();

    if (m_3dcloud) {
      delete [] m_3dcloud;
    }
}

//---------------------------------------------------------------
void RGBDImage::setData(uint8_t * rgb, uint16_t * depth)
{
	clearInternalMembers();

	::memcpy(m_bufferRGB, rgb, sizeof(uint8_t)*m_width*m_height*3 );
	::memcpy(m_bufferDepth, depth, sizeof(uint16_t)*m_width*m_height );
}

//---------------------------------------------------------------
bool RGBDImage::get3DPoint(size_t u_rgb, size_t v_rgb, float & X, float & Y, float & Z)
{
	static double fx_d = 5.7789955959786357e+02;
	static double fy_d = 5.6556773292255900e+02;
	static double cx_d = 3.4708086815455886e+02;
	static double cy_d = 2.5981444729975124e+02;

	float depth = m_bufferDepth[ v_rgb*640 + u_rgb ];

	if( depth == 0 ){
		X = Y = Z = 0.0;
		return false;
	}
	// Map point in 3D World coordinates
	X = (u_rgb - cx_d) * depth / fx_d;
	Y = (v_rgb - cy_d) * depth / fy_d;
	Z = depth;
	return true;
}

//---------------------------------------------------------------
float * RGBDImage::get3DPoints(short version)
{
  if (!m_3dcloud) {
    m_3dcloud = new float[_640x480x3];
  }

  static double fx_d = 5.7789955959786357e+02;
  static double fy_d = 5.6556773292255900e+02;
  static double cx_d = 3.4708086815455886e+02;
  static double cy_d = 2.5981444729975124e+02;

  float depth;
  int idx1 = 0;
  float *p=m_3dcloud;

  int u_start = 0, u_end = 640;
  int v_start = 0, v_end = 480;

  switch(version) {
    case 1:
           u_start = 0; u_end = 640;
           v_start = 160; v_end = 320;
           idx1 =  v_start*640 + u_start;
    break;
  }

  for(register size_t v_d = v_start; v_d < v_end; v_d++)
  {
      switch(version) {
        case 1: 
               if (v_d % 20) {
                 idx1+=640;
                 continue;
               }
        break;
      }
      for(register size_t u_d = u_start; u_d < u_end; u_d++, idx1++)
      {
          depth = m_bufferDepth[idx1];
          if( !depth ){
              *p=0.0f; p++; // X
              *p=0.0f; p++; // Y
              *p=0.0f; p++; // Z
              continue;
          }
          *p = (u_d - cx_d) * depth / fx_d;  p++; // X
          *p = -(v_d - cy_d) * depth / fy_d; p++; // Y
          *p = -depth;                       p++; // Z
      }
  }
  return  m_3dcloud;
}

//---------------------------------------------------------------
void RGBDImage::getCloud3DPoints(std::vector< CvPoint3D32f > & cloudPoints3D,
    short version)
{
	if( m_cloudPoints3D == NULL ){
      computeCloudPoints(version);
	}

	cloudPoints3D.resize( m_cloudPoints3D->size() );
	cloudPoints3D.assign( m_cloudPoints3D->begin(), m_cloudPoints3D->end() );
}

//---------------------------------------------------------------
void RGBDImage::getCloud3DPoints(std::vector< CvPoint3D32f > & cloudPoints3D, IplImage* histogramXZ, IplImage* histogramYZ )
{

	const double fx_d = 5.7789955959786357e+02;
	const double fy_d = 5.6556773292255900e+02;
	const double cx_d = 3.4708086815455886e+02;
	const double cy_d = 2.5981444729975124e+02;

	register int idx = 0;
	register uchar *ptrData;
	register int X,Y,Z;

	if( cloudPoints3D.size() < _640x480 )
		cloudPoints3D.resize(_640x480);

	cvZero( histogramXZ );
	cvZero( histogramYZ );

	for(register size_t v_d = 0; v_d < 480; v_d++)
	{
		for(register size_t u_d = 0; u_d < 640; u_d++, idx++)
		{
			Z = m_bufferDepth[idx];
			if( !Z ){
				cloudPoints3D[idx].z = 0;
				continue;
			}
 			X = cloudPoints3D[idx].x = (u_d - cx_d) * Z / fx_d;
 			Y = cloudPoints3D[idx].y = (v_d - cy_d) * Z / fy_d;
			cloudPoints3D[idx].z = -Z;

			if( Z< 500 )
				continue;
			
			X += 3205; Y += 3200; Z += 5;
			if( Z<4800 && 0<X && X<6400 )
				ptrData = (uchar*) ( histogramXZ->imageData + (Z/10) * histogramXZ->widthStep + (X/10));
				(*ptrData)++;
			if( Z<4800 && 0<Y && Y<6400 )
				ptrData = (uchar*) ( histogramYZ->imageData + (Z/10) * histogramYZ->widthStep + (Y/10));
				(*ptrData)++;
		}
	}
}


//---------------------------------------------------------------
IplImage * RGBDImage::cvAdaptorRGB()
{
	if( m_cvImageRGB == NULL ) {
		m_cvImageRGB = cvCreateImageHeader(cvSize(m_width, m_height), IPL_DEPTH_8U, 3);
		cvSetImageData(m_cvImageRGB, (char *) m_bufferRGB, m_width * 3);
	}

	return m_cvImageRGB;
}

//---------------------------------------------------------------
IplImage * RGBDImage::cvAdaptorDepthNormalize()
{
	if( m_bufferDepthVisualizeUI8 == NULL ){
// 		m_bufferDepthVisualize = new uint16_t[640*480];
		m_bufferDepthVisualizeUI8 = new uint8_t[640*480];
	}

	computeDepthNormalized();

	if( m_cvImageDepth == NULL ){
// 		m_cvImageDepth = cvCreateImageHeader(cvSize(m_width, m_height), IPL_DEPTH_16U, 1);
// 		cvSetImageData(m_cvImageDepth, (uint16_t *) m_bufferDepthVisualize, m_cvImageDepth->widthStep );
		m_cvImageDepth = cvCreateImageHeader(cvSize(m_width, m_height), IPL_DEPTH_8U, 1);
		cvSetImageData(m_cvImageDepth, (uint8_t *) m_bufferDepthVisualizeUI8, m_cvImageDepth->widthStep );
	}

	return m_cvImageDepth;
}

//---------------------------------------------------------------
IplImage * RGBDImage::cvAdaptorDepthNormalizeUI16()
{
	if( m_bufferDepthVisualizeUI16 == NULL ){
		m_bufferDepthVisualizeUI16 = new uint16_t[640*480];
	}

	computeDepthNormalized( true );

	if( m_cvImageDepthUI16 == NULL ){
		m_cvImageDepthUI16 = cvCreateImageHeader(cvSize(m_width, m_height), IPL_DEPTH_16U, 1);
		cvSetImageData(m_cvImageDepthUI16, (uint16_t *) m_bufferDepthVisualizeUI16, m_cvImageDepthUI16->widthStep );
	}

	return m_cvImageDepthUI16;
}


//---------------------------------------------------------------
IplImage * RGBDImage::cvAdaptorDepthMask()
{
	if( m_bufferDepthVisualizeUI8 == NULL ){
// 		m_bufferDepthVisualize = new uint16_t[640*480];
 		m_bufferDepthVisualizeUI8 = new uint8_t[640*480];
	}

	if( m_cvImageDepth == NULL ){
		m_cvImageDepth = cvCreateImageHeader(cvSize(m_width, m_height), IPL_DEPTH_8U, 1);
// 		cvSetImageData(m_cvImageDepth, (uint16_t *) m_bufferDepthVisualize, m_width );
		cvSetImageData(m_cvImageDepth, (uint8_t *) m_bufferDepthVisualizeUI8, m_width );
	}

	const int numPixels = m_width * m_height;
	for( size_t idx = 0; idx < numPixels; ++idx ){
		m_bufferDepthVisualizeUI8[idx] = m_bufferDepth[idx]  ? 255 : 0;
	}
	
	return m_cvImageDepth;
}

//---------------------------------------------------------------
uint16_t* RGBDImage::bufferDepthVisualizeUI16()
{
	if( m_bufferDepthVisualizeUI16 == NULL ){
 		m_bufferDepthVisualizeUI16 = new uint16_t[640*480];
	}

	computeDepthNormalized( true );

	return m_bufferDepthVisualizeUI16;
}


//---------------------------------------------------------------
uint8_t* RGBDImage::bufferDepthVisualize2()
{
	if( m_bufferDepthVisualizeUI8 == NULL ){
 		m_bufferDepthVisualizeUI8 = new uint8_t[_640x480x3];
	}

	computeDepthNormalized1(1);
	//computeDepthNormalized2();
	//computeDepthNormalized4();

	return m_bufferDepthVisualizeUI8;
}
//---------------------------------------------------------------
uint8_t* RGBDImage::bufferDepthVisualize()
{
	if( m_bufferDepthVisualizeUI8 == NULL ){
// 		m_bufferDepthVisualize = new uint16_t[640*480];
 		m_bufferDepthVisualizeUI8 = new uint8_t[640*480];
	}

	//computeDepthNormalized();
	computeDepthNormalized1();

	return m_bufferDepthVisualizeUI8;
}

//---------------------------------------------------------------
void RGBDImage::computeDepthNormalized( bool useUInt16 )
{
	int idx =0;
	float g_pDepthHist[MAX_DEPTH]={0};
	unsigned int nNumberOfPoints = 0;

	// Calculate the accumulative histogram
	for (size_t y = 0; y < 480; ++y) {
		for (size_t x = 0; x < 640; ++x) {
			if (m_bufferDepth[idx] != 0) {
				g_pDepthHist[ m_bufferDepth[idx] ]++;
				nNumberOfPoints++;
			}
			idx++;
		}
	}

	for (int nIndex=1; nIndex<MAX_DEPTH; nIndex++) {
		g_pDepthHist[nIndex] += g_pDepthHist[nIndex-1];
	}

	if (nNumberOfPoints) {
		for (int nIndex=1; nIndex<MAX_DEPTH; nIndex++) {
			g_pDepthHist[nIndex] = (unsigned int)(255 * (1.0f - (g_pDepthHist[nIndex] / nNumberOfPoints)));
		}
	}

	idx =0;
	int nHistValue;
	register size_t offsetDest=0;

	if( useUInt16 ){
		for (size_t y = 0; y < 480; ++y) {
			for (size_t x = 0; x < 640; ++x ) {
				if( m_bufferDepth[idx] != 0 ) {
					nHistValue = g_pDepthHist[m_bufferDepth[idx]];
					m_bufferDepthVisualizeUI16[idx] = nHistValue;
				}else
					m_bufferDepthVisualizeUI16[idx] = 0;
				idx++;
			}
		}
	}else{
		for (size_t y = 0; y < 480; ++y) {
				for (size_t x = 0; x < 640; ++x ) {
					if( m_bufferDepth[idx] != 0 ) {
						nHistValue = g_pDepthHist[m_bufferDepth[idx]];
						m_bufferDepthVisualizeUI8[idx] = nHistValue;
					}else
						m_bufferDepthVisualizeUI8[idx] = 0;
					idx++;
				}
			}
	}
}

//---------------------------------------------------------------
void RGBDImage::computeDepthNormalized1(short size)
{
	int idx;
    int max=0;
    short nzp; // no zero points

    float colorCount; // Number of colors
    float colorVal; // Number of colors

    depth_info_t *pDepthHist;
    depth_color_info_t *pDHNZ; // Depth Hist No Zero

    uint8_t *pImg = m_bufferDepthVisualizeUI8;

    pDepthHist = new depth_info_t[MAX_DEPTH];

    ::memset(pDepthHist,0, sizeof(depth_info_t)*MAX_DEPTH);

	// Calculate the accumulative histogram
	for (idx = 0; idx < _640x480; idx++) {
      if (m_bufferDepth[idx] != 0) {
        pDepthHist[ m_bufferDepth[idx] ].val++;
      }
	}

    nzp=0;
	for (idx = 0; idx < MAX_DEPTH; idx++) {
      pDepthHist[ idx ].dist = idx;
      // Get the numbers of points no zeros
      if (pDepthHist[idx].val != 0 ) {
        nzp++;
        // And get max value
        if ( pDepthHist[ idx ].val > max) {
          max = pDepthHist[ idx ].val; 
        }
      }
    }

    pDHNZ = new depth_color_info_t[nzp];

    nzp=0;
    // Copy non zeros
	for (idx = 0; idx < MAX_DEPTH; idx++) {
      if (pDepthHist[idx].val != 0 ) {
        pDHNZ[nzp].dist = pDepthHist[idx].dist;
        pDHNZ[nzp].val  = pDepthHist[idx].val;
        nzp++;
      }
	}

    //::qsort (pDHNZ, nzp, sizeof(depth_color_info_t), qsort_compare_val);

    colorCount = 255.0f / (float)nzp; // Color partition
    colorVal = 255.0f;
	for (idx = 0; idx < nzp; idx++) {
      pDepthHist[ pDHNZ[idx].dist ].val = (int)(colorVal + 0.5);
      colorVal -= colorCount;
    }

    // Create the Depth-Image
    if (size) {
      for (idx = 0; idx < _640x480; idx++) {
        if ( m_bufferDepth[idx] == 0) {
          *pImg++ = 0x00; *pImg++ = 0x00; *pImg++ = 0x00; continue;
        }
        *pImg++ = (uint8_t)pDepthHist[ m_bufferDepth[idx] ].val;
        *pImg++ = (uint8_t)pDepthHist[ m_bufferDepth[idx] ].val;
        *pImg++ = (uint8_t)pDepthHist[ m_bufferDepth[idx] ].val;
      }
    } else {
      for (idx = 0; idx < _640x480; idx++) {
        if ( m_bufferDepth[idx] == 0) {
          *pImg++ = 0x00; continue;
        }
        *pImg++ = (uint8_t)pDepthHist[ m_bufferDepth[idx] ].val;
      }
    }

    delete [] pDepthHist;
    delete [] pDHNZ;
}

//---------------------------------------------------------------
void RGBDImage::computeDepthNormalized2()
{
    FILE *f=0;
	int idx;
    int max=0;
    short nzp; // no zero points
    float colorCount; // Number of colors
    float colorVal; // Number of colors
    bool colorFound;
    depth_info_t *pDepthHist;
    depth_color_info_t *pDHNZ; // Depth Hist No Zero
    uint8_t *pImg = m_bufferDepthVisualizeUI8;
    pDepthHist = new depth_info_t[MAX_DEPTH];

    ::memset(pDepthHist,0, sizeof(depth_info_t)*MAX_DEPTH);

	// Calculate the accumulative histogram
	for (idx = 0; idx < _640x480; idx++) {
      if (m_bufferDepth[idx] != 0) {
        pDepthHist[ m_bufferDepth[idx] ].val++;
      }
	}

    nzp=0;
	for (idx = 0; idx < MAX_DEPTH; idx++) {
      pDepthHist[ idx ].dist = idx;
      // Get the numbers of points no zeros
      if (pDepthHist[idx].val != 0 ) {
        nzp++;
        // And get max value
        if ( pDepthHist[ idx ].val > max) {
          max = pDepthHist[ idx ].val; 
        }
      }
    }

    pDHNZ = new depth_color_info_t[nzp];
    //::memset(pDHNZ,0, sizeof(depth_color_info_t)*nzp);

    nzp=0;
    // Copy non zeros
	for (idx = 0; idx < MAX_DEPTH; idx++) {
      if (pDepthHist[idx].val != 0 ) {
        pDHNZ[nzp].dist = pDepthHist[idx].dist;
        pDHNZ[nzp].val  = pDepthHist[idx].val;
        //pDHNZ[nzp].col = nzp;
        pDHNZ[nzp].col = 0;
        nzp++;
      }
	}

    if (dumpStats) {
      /**/ f = fopen("nozero.txt", "w");
      if (f != NULL ) {
        for (idx = 0; idx < nzp; idx++) {
          fprintf(f,"%5d\t%5d\n", pDHNZ[ idx ].dist, pDHNZ[ idx ].val);
        }
        fclose(f);
      } /**/
    }


    colorCount = 255.0f / (float)nzp; // Color partition
    colorVal = 255.0f;
	for (idx = 0; idx < nzp; idx++) {
      pDepthHist[ pDHNZ[idx].dist ].val = (int)(colorVal + 0.5);
      colorVal -= colorCount;
    }

    // short the array
    ::qsort (pDHNZ, nzp, sizeof(depth_color_info_t), qsort_compare_val);

    if (dumpStats) {
      /**/ f = fopen("ordenados.txt", "w");
      if (f != NULL ) {
        for (idx = 0; idx < nzp; idx++) {
          fprintf(f,"%5d\t%5d\n", pDHNZ[ idx ].dist, pDHNZ[ idx ].val);
        }
        fclose(f);
      }  /**/
    }

    // Put color to the first 12
    for(short i=0; i<12; i++) { pDHNZ[i].col = i+1; }


    // Create the Depth-Image
	for (idx = 0; idx < _640x480; idx++) {
      colorFound = false;
      if ( m_bufferDepth[idx] == 0) {
        *pImg++ = 0x00; *pImg++ = 0x00; *pImg++ = 0x00; continue;
      }
      
      /**/
      for(short i=0; i<12; i++) 
      {
        if ( m_bufferDepth[idx] == pDHNZ[i].dist) {
          *pImg++ = rgbdColor[i][0];
          *pImg++ = rgbdColor[i][1];
          *pImg++ = rgbdColor[i][2];
          colorFound = true;
          break; 
        }
      }
      /**/
      
      if (! colorFound ) {
        *pImg++ = (uint8_t)pDepthHist[ m_bufferDepth[idx] ].val;
        *pImg++ = (uint8_t)pDepthHist[ m_bufferDepth[idx] ].val;
        *pImg++ = (uint8_t)pDepthHist[ m_bufferDepth[idx] ].val;
      }
	}

    //makeH(pDHNZ, nzp, max); // Make Histrogram in RGB

    delete [] pDHNZ;
    delete [] pDepthHist;
    dumpStats = false;
}

//---------------------------------------------------------------
void RGBDImage::computeDepthNormalized3()
{
    FILE *f=0;
	int idx;
    int max=0;
    short nzp; // no zero points
    float colorCount; // Number of colors
    float colorVal; // Number of colors
    bool colorFound;
    depth_info_t *pDepthHist;
    depth_info_t *pDHP;
    depth_color_info_t *pDHNZ; // Depth Hist No Zero
    uint8_t *pImg = m_bufferDepthVisualizeUI8;
    pDepthHist = new depth_info_t[MAX_DEPTH];

    ::memset(pDepthHist,0, sizeof(depth_info_t)*MAX_DEPTH);

	// Calculate the accumulative histogram
	for (idx = 0; idx < _640x480; idx++) {
      if (m_bufferDepth[idx] != 0) {
        pDepthHist[ m_bufferDepth[idx] ].val++;
      }
	}

    nzp=0;
	for (idx = 0; idx < MAX_DEPTH; idx++) {
      pDepthHist[ idx ].dist = idx;
      // Get the numbers of points no zeros
      if (pDepthHist[idx].val != 0 ) {
        nzp++;
        // And get max value
        if ( pDepthHist[ idx ].val > max) {
          max = pDepthHist[ idx ].val; 
        }
      }
    }

    pDHNZ = new depth_color_info_t[nzp];
    pDHP = new depth_info_t[nzp];
    //::memset(pDHNZ,0, sizeof(depth_color_info_t)*nzp);

    nzp=0;
    // Copy non zeros
	for (idx = 0; idx < MAX_DEPTH; idx++) {
      if (pDepthHist[idx].val != 0 ) {
        pDHNZ[nzp].dist = pDepthHist[idx].dist;
        pDHP[nzp].dist = 0;
        pDHNZ[nzp].val  = pDHP[nzp].val  = pDepthHist[idx].val;
        pDHNZ[nzp].col = nzp;
        nzp++;
      }
	}

    if (dumpStats) {
      /**/ f = fopen("nozero.txt", "w");
      if (f != NULL ) {
        for (idx = 0; idx < nzp; idx++) {
          fprintf(f,"%5d\t%5d\n", pDHNZ[ idx ].dist, pDHNZ[ idx ].val);
        }
        fclose(f);
      } /**/
    }


    colorCount = 255.0f / (float)nzp; // Color partition
    colorVal = 255.0f;
	for (idx = 0; idx < nzp; idx++) {
      pDepthHist[ pDHNZ[idx].dist ].val = (int)(colorVal + 0.5);
      colorVal -= colorCount;
    }

    // short the array
    ::qsort (pDHNZ, nzp, sizeof(depth_color_info_t), qsort_compare_val);

    if (dumpStats) {
      /**/ f = fopen("ordenados.txt", "w");
      if (f != NULL ) {
        for (idx = 0; idx < nzp; idx++) {
          fprintf(f,"%5d\t%5d\n", pDHNZ[ idx ].dist, pDHNZ[ idx ].val);
        }
        fclose(f);
      }  /**/
    }

    // Put color to the first 12
    //for(short i=0; i<12; i++) { pDHNZ[i].col = i+1; }
    findPlane(pDHP, pDHNZ, nzp);


    // Create the Depth-Image
	for (idx = 0; idx < _640x480; idx++) {
      colorFound = false;
      if ( m_bufferDepth[idx] == 0) {
        *pImg++ = 0x00; *pImg++ = 0x00; *pImg++ = 0x00; continue;
      }
      
      /**/
      //for(short i=0; i<12; i++) 
      for(short i=0; i<nzp; i++) {
        if ( m_bufferDepth[idx] == pDHNZ[i].dist) {
          if ( pDHNZ[i].col ) {
            //*pImg++ = rgbdColor[i][0];
            *pImg++ = rgbdColor[pDHNZ[i].col-1][0];
            *pImg++ = rgbdColor[pDHNZ[i].col-1][1];
            *pImg++ = rgbdColor[pDHNZ[i].col-1][2];
            colorFound = true;
          }
          break; 
        }
      }
      /**/
      
      if (! colorFound ) {
        *pImg++ = (uint8_t)pDepthHist[ m_bufferDepth[idx] ].val;
        *pImg++ = (uint8_t)pDepthHist[ m_bufferDepth[idx] ].val;
        *pImg++ = (uint8_t)pDepthHist[ m_bufferDepth[idx] ].val;
      }
	}

    if (hmax < max) {
      hmax = max;
      printf("Histogram MAX: %d\n", hmax);
    }
    makeH(pDHNZ, nzp, hmax); // Make Histrogram in RGB

    delete [] pDHP;
    delete [] pDHNZ;
    delete [] pDepthHist;
    dumpStats = false;
}

//---------------------------------------------------------------
void RGBDImage::computeDepthNormalized4()
{
  //int i, idx;
  short step = 10;
  uint8_t *pImg = m_bufferDepthVisualizeUI8;
  short i;

  //::memset(pImg, 0xFF, sizeof(uint8_t)*_640x480x3);
  // Vertical
  for (i=0; i<640; i+=step) {
    scan(1,i);
  }
  // Horizontal
  for (i=0; i<480; i+=step) {
    scan(0,i);
  }

  /******
  // Vertical
  //pImg = m_bufferDepthVisualizeUI8 + 640*step*3;
  for (idx = 0; idx < _640x480; idx+=step) {
    if ( m_bufferDepth[idx] == 0) {
      pImg+=step*3; continue;
    }
    *pImg++ = m_bufferRGB[idx*3+0];
    *pImg++ = m_bufferRGB[idx*3+1];
    *pImg++ = m_bufferRGB[idx*3+2];
    pImg+=step*3-3;
  }
  // Horizontal
  pImg = m_bufferDepthVisualizeUI8 + 640*step*3;
  for (idx = 640*step; idx<_640x480; idx+=640*step) {
    i=0;
    while (i<640 && idx<_640x480) {
      if (m_bufferDepth[idx] == 0) {
        i++; idx++; pImg+=3; continue;
      }
      *pImg++ = m_bufferRGB[idx*3+0];
      *pImg++ = m_bufferRGB[idx*3+1];
      *pImg++ = m_bufferRGB[idx*3+2];
      i++;  idx++;
    }
    pImg += 640*step*3;
  }
  ******/
}

//---------------------------------------------------------------
void RGBDImage::scan(short dir, short line)
{
  short i, max, step;
  int idx;
  uint8_t *pImg = m_bufferDepthVisualizeUI8;
  uint16_t  val[3];

  if (dir == 0){ // Horizontal
    step = 1;
    max  = 640;
    idx  = 640*line;
  } else {
    step = 640;
    max  = 480;
    idx  = line;
  }
  pImg += idx*3;
  val[0] = 0;
  val[1] = 0;
  val[2] = 0;
  for (i=0; i<max; i++, idx+=step) {
    if (m_bufferDepth[idx] == 0) {
      pImg+=step*3; continue;
    }
    if(m_bufferDepth[idx] == val[0] ) {
      setMyColor(pImg,3);
    } else {
      if(m_bufferDepth[idx] > val[0] ) {
        setMyColor(pImg,0);
      } else {
        setMyColor(pImg,1);
      }
    }
    val[0]=m_bufferDepth[idx];
    /*
    *pImg++ = m_bufferRGB[idx*3+0];
    *pImg++ = m_bufferRGB[idx*3+1];
    *pImg++ = m_bufferRGB[idx*3+2];
    pImg+=step*3-3;
    */
    pImg+=step*3;
  }
}

//---------------------------------------------------------------
void RGBDImage::findPlane(depth_info_t *pH, depth_color_info_t *pHS, short s)
{
  short i,k;
  int dVal;
  float rate = 0.0f;
  for(i=0; i<12; i++) {
    if (! pH[ pHS[i].col ].dist) {

      // Derecha
      k = pHS[i].col;
      do {
        pH[k].dist = i+1;
        dVal = pH[k].val - pH[k+1].val;
        //if (dVal > 0 && (k+2 < s)) {
          //dVal = pH[k].val - pH[k+2].val;
        //}
        k++;
      } while (dVal > 0 && (k+1 < s));

      // Izquierda
      k = pHS[i].col;
      do {
        pH[k].dist = i+1;
        dVal = pH[k].val - pH[k-1].val;
        //if (dVal > 0 && (k-2 > 0)) {
          //dVal = pH[k].val - pH[k-2].val;
        //}
        k--;
      } while (dVal > 0 && (k-1 > 0));

      //rate = pH[k].val / pH[k+1].val;
    } 
  } // for i 0..12
  // Clear color
  for(i=0; i<s; i++) {
    k=pHS[i].col;
    pHS[i].col=pH[k].dist;
  }

}

void  RGBDImage::makeH(depth_color_info_t *pD, short s, int max)
{
  uint8_t *bar, *lastLine;
  short limit = 320;
  short i, h, val;
  ::memset(m_bufferRGB, 0xFF, sizeof(uint8_t)*_640x480*3 );
  if (s < limit) {
    limit = s;
  }
  lastLine = m_bufferRGB + 640*479*3;

  ::qsort (pD, s, sizeof(depth_color_info_t), qsort_compare_dist);

  for(i=0; i<s; i++) {
    val = (short)((float)pD[i].val / (float)max * 480);
    bar = lastLine + i*2*3;
    for (h=0; h<val; h++){
      if (pD[i].col) {
        bar[0] = bar[3] = rgbdColor[pD[i].col-1][0];
        bar[1] = bar[4] = rgbdColor[pD[i].col-1][1];
        bar[2] = bar[5] = rgbdColor[pD[i].col-1][2];
      } else {
        ::memset(bar, 0, sizeof(uint8_t)*2*3 );
      }
      bar -= 640*3;
    }
  }
}

//---------------------------------------------------------------
std::vector< CvPoint3D32f >* RGBDImage::cloudPoints3D()
{
	if( m_cloudPoints3D == NULL ){
		computeCloudPoints();
	}

	return m_cloudPoints3D;
}

//---------------------------------------------------------------
void RGBDImage::computeCloudPoints(short version)
{
	//register size_t offsetRGB=0, offsetDepth=0;

	if( m_cloudPoints3D != NULL ){
      delete m_cloudPoints3D;
    }
	m_cloudPoints3D = new std::vector<CvPoint3D32f>(_640x480);

	double fx_d = 5.7789955959786357e+02;
	double fy_d = 5.6556773292255900e+02;
	double cx_d = 3.4708086815455886e+02;
	double cy_d = 2.5981444729975124e+02;

	float depth;
	int idx1 = 0;

    int u_start = 0, u_end = 640;
    int v_start = 0, v_end = 480;

    switch(version) {
      case 1:
             u_start = 0; u_end = 640;
             v_start = 160; v_end = 320;
	         idx1 =  v_start*640 + u_start;
      break;
    }

	for(register size_t v_d = v_start; v_d < v_end; v_d++)
	{
        switch(version) {
          case 1: 
                 if (v_d % 20) {
                   idx1+=640;
                   continue;
                 }
          break;
        }
		for(register size_t u_d = u_start; u_d < u_end; u_d++, idx1++)
		{
			depth = m_bufferDepth[idx1];
			if( !depth ){
// 				cloudPoints3D[idx1][0] = cloudPoints3D[idx1][1] = 
				(m_cloudPoints3D->operator[](idx1)).z = 0;
				continue;
			}
			(m_cloudPoints3D->operator[](idx1)).x = (u_d - cx_d) * depth / fx_d;
			(m_cloudPoints3D->operator[](idx1)).y = -(v_d - cy_d) * depth / fy_d;
			(m_cloudPoints3D->operator[](idx1)).z = -depth;
		}
	}
}

//---------------------------------------------------------------
bool RGBDImage::saveToFile(const char * nameFile  )
{
	//int numData = 640*480;

	std::ofstream fileToWrite(nameFile,  std::ofstream::binary);

	fileToWrite.write( (char * )m_bufferRGB,   sizeof(uint8_t )*_640x480*3 );
	fileToWrite.write( (char * )m_bufferDepth, sizeof(uint16_t)*_640x480 );
	fileToWrite.close();

	return true;
}

//---------------------------------------------------------------
bool RGBDImage::loadFromFile(const char * nameFile)
{
	//int numData = 640*480;

	std::ifstream fileToRead( nameFile, std::ifstream::binary);
		
	fileToRead.read( (char * )m_bufferRGB,   sizeof(uint8_t )*_640x480*3 );
	fileToRead.read( (char * )m_bufferDepth, sizeof(uint16_t)*_640x480 );

	fileToRead.close();
	return true;
}

//---------------------------------------------------------------
void RGBDImage::clearInternalMembers()
{
	if( m_cvImageRGB != NULL )
		cvReleaseImageHeader( &m_cvImageRGB );
	if( m_cvImageDepth != NULL )
		cvReleaseImageHeader( &m_cvImageDepth );
	if( m_cvImageDepthUI16 != NULL )
		cvReleaseImageHeader( &m_cvImageDepthUI16 );

	if( m_bufferDepthVisualizeUI8 != NULL )
		delete [] m_bufferDepthVisualizeUI8; 
	if( m_bufferDepthVisualizeUI16 != NULL )
		delete [] m_bufferDepthVisualizeUI16; 
	if( m_cloudPoints3D != NULL )
		delete m_cloudPoints3D;

	m_cvImageRGB = NULL;
	m_cvImageDepth = NULL;
	m_cvImageDepthUI16 = NULL;
	m_bufferDepthVisualizeUI8 = NULL;
	m_bufferDepthVisualizeUI16 = NULL;
	m_cloudPoints3D = NULL;
}

