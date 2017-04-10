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


#ifndef RGBDIMAGE_H
#define RGBDIMAGE_H

/**
alloc RGBD maps  and methods to manipulate the kinects buffers
and extract basic information for them. Implements a wrapper
to openCV-IplImage

	@author Heriberto Casarrubias Vargas,,, <heribertocv@gmail.com>
*/

#include "cv.h"
#include <cstring>

#ifdef WIN32
typedef unsigned char       uint8_t;
typedef unsigned short int  uint16_t;
#else
 #include <stdint.h>
#endif

enum
  {
    MAX_DEPTH = 10000,
#define MAX_DEPTH MAX_DEPTH
    _640x480_ = 307200,
#define _640x480_ _640x480_
    _640x480x3 = 921600
#define _640x480x3 _640x480x3
  };

struct depth_info_t
{
  int dist;
  int val;
};

struct depth_color_info_t
{
  int dist;
  int val;
  uint8_t col;
};

class RGBDImage{
public:
    RGBDImage( size_t width = 640, size_t height = 480 );

    ~RGBDImage();
	
	/**
	 *    set the RGBD data
	 * @param rgb 	the rgb buffer
	 * @param depth the depth buffer
	 */
	void setData( uint8_t *rgb, uint16_t *depth );


	/**
	 * Adaptor to manipulate the RGB Image with Opencv methods
	 * @return an  IplImage pointer that is a wrapper to data RGB buffer
	 */
	IplImage* cvAdaptorRGB();

	/**
	 * Adaptor to manipulate the Depth Image with Opencv methods
	 * @return an  IplImage pointer that is a wrapper to data Depth-Normalize buffer
	 */
	IplImage* cvAdaptorDepthNormalize();

	/**
	 * Adaptor to manipulate the Depth Image with Opencv methods
	 * @return an  IplImage pointer that is a wrapper to data Depth-Normalize buffer
	 */
	IplImage* cvAdaptorDepthNormalizeUI16();


	/**
	 * Opencv-IplImage Adaptor to mask Image for depth Buffer 
	 * @return an  IplImage pointer that is a wrapper to mask image of depth
	 */
	IplImage* cvAdaptorDepthMask();



	/**
	 *    Return the correspondient 3D point associated to (u,v)-RGB point
	 * @param u_rgb
	 * @param v_rgb
	 * @param X
	 * @param Y
	 * @param Z
	 * @return true if is possible obtain an 3D point false in otherwise
	 */
	bool get3DPoint( size_t u_rgb, size_t v_rgb, float &X, float &Y, float &Z );

	/**
	 * 
	 * @param u_rgb 
	 * @param v_rgb 
	 * @return 
	 */
	uint16_t getDepth( size_t u_d, size_t v_d ){ return m_bufferDepth[ u_d + v_d*640 ];};
	/**
	 * 
	 * @param i 
	 * @return 
	 */
	uint16_t getDepth( int i ){ return m_bufferDepth[ i ]; };


	/**
	 * Compute the Cloud of points in 3D World Coordinates wrt RGB-camera
	 * 
	 * @param  cloudPoints reference  std::vector to allocate Cloud of Points, il::Vector
	 */
	void getCloud3DPoints( std::vector< CvPoint3D32f > &cloudPoints3D, short version = 0);

	/**
	 * Compute the Cloud of Points in 3D World Coordinates wrt RGB-camera and
	 * compute the histogram of the incidences (projection the data) on planes XZ and YZ
	 * @param cloudPoints3D 
	 * @param histogramXZ  to allocate the XZ proyection in centimeters, must be size RGBDImage->width x RGBDImage->heigth,
	 * @param histogramYZ  to allocate the YZ proyection in centimeters, must be size RGBDImage->width x RGBDImage->heigth
	 */
	void getCloud3DPoints( std::vector< CvPoint3D32f > &cloudPoints3D, IplImage* histogramXZ, IplImage* histogramYZ );

    float * get3DPoints(short version = 0);

	/**
	 *    Allow free acces to 3D cloud of points
	 * @return 
	 */
	std::vector< CvPoint3D32f >* cloudPoints3D();


	/**
	 *  
	 * @return the heigth of the RGBD image
	 */
	size_t height() const
	{
		return m_height;
	}

	/**
	 *
	 * @return the width of the RGBD image
	 */
	size_t width() const
	{
		return m_width;
	}

	/**
	 * Allow the access for read and write the RGB Buffer
	 * @return pointer to the RGB Buffer
	 */
	const uint8_t* bufferRGB() const
	{
		return m_bufferRGB;
	}

	/**
	 * Allow the access for read and write the Depth Buffer
	 * @return pointer to the Depth Buffer
	 */
// 	const uint16_t* bufferDepth() const
	const uint16_t* bufferDepth() const
	{
		return m_bufferDepth;
	}

	/**
	 * Allow access to Normalized Depth Buffer data, useful for display depth at viewer
	 * @return Depth Buffer data normalized
	 */
	uint16_t* bufferDepthVisualizeUI16();
	uint8_t*  bufferDepthVisualize();

	uint8_t*  bufferDepthVisualize2();


	/**
	 * 	dump the RGBD image data to *.rgbd file as binary file
	 * @param nameFile to write
	 * @return
	 */
	bool saveToFile(const char * nameFile );

	/**
	 *  Load RGBD image data from *.rgbd file
	 * @param nameFile 
	 * @return 
	 */
	bool loadFromFile(const char * nameFile );

	void computeDepthNormalized1(short size = 0);
	void computeDepthNormalized2();
	void computeDepthNormalized3();
	void computeDepthNormalized4();
    void scan(short dir, short line);

    void makeH(depth_color_info_t *pD, short s, int max); // Make Histrogram in RGB

    int  hmax;

    void findPlane(depth_info_t *pH, depth_color_info_t *pHS, short s);

    bool dumpStats;

private:
	size_t m_width;
	size_t m_height;

    const int _640x480;

	uint8_t  m_bufferRGB[640*480*3];	// allocate the rgb image data
	uint16_t m_bufferDepth[640*480];	// alocate the depth data
	uint8_t *m_bufferDepthVisualizeUI8;	// allocate the depth info normalized to visualize like grey image
	uint16_t *m_bufferDepthVisualizeUI16;	// allocate the depth info normalized to visualize like grey image
	std::vector< CvPoint3D32f > *m_cloudPoints3D; //allocate the clouds of points; 

	IplImage *m_cvImageRGB;
	IplImage *m_cvImageDepth;
	IplImage *m_cvImageDepthUI16;

	/**
	 *    Utility function
	 * @param useUInt16
	 */
	void computeDepthNormalized( bool useUInt16 = false );
	/**
	 *    Utility funciton 
	 */
	void computeCloudPoints(short version = 0);

	/**
	 *    Utility Function
	 */
	void clearInternalMembers();

    float *m_3dcloud;
};

#endif
