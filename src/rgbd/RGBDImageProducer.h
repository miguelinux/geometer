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


#ifndef RGBDIMAGEPRODUCER_H
#define RGBDIMAGEPRODUCER_H

/**
This class can producer RGBDImages form kinect hardware devices or ONI files
The RGBDImage buffer Depth is in Real World millimeters

	@author Heriberto Casarrubias Vargas,,, <heribertocv@gmail.com>
*/


#include <XnCppWrapper.h>
#include "rgbdimage.h"

//#include <stdint.h>

class RGBDImageProducer{
public:
    /**
     *
     * @param nameFile the nameFile for try set up the stream
	 * 			if    type of nameFile is *.xml the stream is try to started from Kinect device 
	 *			if	  type of nameFile is *.oni the stream is try to started from oni file
     */
    RGBDImageProducer( const char *nameFile );

    ~RGBDImageProducer();

	/**
	 *    producer a RGBDImage take from the stream
	 * @param  
	 * @return a RGBDImage
	 */
	RGBDImage* getRGBDImage();

    /** Tell if the device is connected or the file is loadded correctly. */
    inline bool isConnected() {return connected;}

private: 
	enum StreamRGBD{ useKinectDevice=1, useOniFile=2, unknowStream };
	StreamRGBD m_streamFrom;
	uint8_t rgb[640*480*3];
	uint16_t depth[640*480];

	void dumpRGB();
	void dumpDepth();

	xn::Context m_xn_context;
	xn::DepthGenerator m_xn_depthGenerator;
	xn::ImageGenerator m_xn_imageGenerator;
	xn::DepthMetaData m_xn_depthMetadata;
	xn::ImageMetaData m_xn_imageMetaData;
	float m_histogramDepth[MAX_DEPTH];

    /** Connected to the device/file. */
    bool connected;

    const int _640x480;
	XnPoint3D *m_nProjection;
	XnPoint3D *m_nRealWorld;
	RGBDImage *m_resultRGBDImage;

};

#endif
