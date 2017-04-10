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


#include "RGBDImageProducer.h"

#include <XnOpenNI.h>

#include <iostream>
#include <cstdio>
#include <fstream>

// --------------------------------------------------------------------------
RGBDImageProducer::RGBDImageProducer( const char *nameFile ) :
    _640x480(640*480)
{
	//------------------------------
	//	setUp  DataSet
	//------------------------------
    connected = false;
	XnStatus nRetVal = XN_STATUS_OK;
	std::string file( nameFile );
	size_t init_extension = file.find_last_of(".")+1;
	std::string extension = file.substr(init_extension, std::string::npos );
	if( extension == std::string("xml") )
		m_streamFrom = useKinectDevice;
	else 
	if( extension == std::string("oni") )
		m_streamFrom = useOniFile;
	else{
		m_streamFrom = unknowStream;
		std::cerr<<"ERROR:: unknow file stream "<<nameFile<<std::endl;
		return;
	}
	
	if( m_streamFrom == useOniFile ) // init from ONI file
	{
		std::cerr<<"Stream from file: "<<nameFile<<std::endl;
		nRetVal = m_xn_context.Init();
		if (nRetVal != XN_STATUS_OK)
		{
			std::cerr<<"Init failed : "<<xnGetStatusString(nRetVal)<<std::endl;
		}
		// open input file
		nRetVal = m_xn_context.OpenFileRecording( nameFile );
		if (nRetVal != XN_STATUS_OK)
		{
			std::cerr<<"Open input file failed : "<<xnGetStatusString(nRetVal)<<std::endl;
		}
	}else
// 	if(  m_streamFrom == useKinectDevice ) //init from KINECT device
	{
		std::cerr<<"Stream from Kinect Device, using XML file: "<<nameFile<<std::endl;
		nRetVal = m_xn_context.InitFromXmlFile(nameFile);
		if (nRetVal != XN_STATUS_OK)
		{
			std::cerr<<"Open failed: "<<xnGetStatusString(nRetVal)<<std::endl;
		}
	}

	// Get depth node
	nRetVal = m_xn_context.FindExistingNode(XN_NODE_TYPE_DEPTH, m_xn_depthGenerator);
	if (nRetVal != XN_STATUS_OK)
	{
		std::cerr<<"Find depth generator failed : "<<xnGetStatusString(nRetVal)<<std::endl;
	}

	nRetVal = m_xn_context.FindExistingNode(XN_NODE_TYPE_IMAGE, m_xn_imageGenerator);
	if (nRetVal != XN_STATUS_OK)
	{
		std::cerr<<"Open failed g_context.FindExistingNode : "<<xnGetStatusString(nRetVal)<<std::endl;
	}

	// For alignment of images
	m_xn_depthGenerator.GetAlternativeViewPointCap().SetViewPoint(m_xn_imageGenerator);
	// For mirror images
	m_xn_context.SetGlobalMirror( !m_xn_context.GetGlobalMirror() );
	m_xn_context.SetGlobalMirror( !m_xn_context.GetGlobalMirror() );

    connected = true;


    m_nProjection = new XnPoint3D[_640x480];
    m_nRealWorld  = new XnPoint3D[_640x480];
	m_resultRGBDImage = NULL;
}

// --------------------------------------------------------------------------
RGBDImageProducer::~RGBDImageProducer()
{
	m_xn_context.Shutdown();
    if (m_nProjection) delete m_nProjection;
    if (m_nRealWorld)  delete m_nRealWorld;
	if (m_resultRGBDImage != NULL) delete m_resultRGBDImage;
    m_nProjection = 0;
    m_nRealWorld  = 0;
}

// --------------------------------------------------------------------------
RGBDImage* RGBDImageProducer::getRGBDImage(void )
{
	XnStatus rc;
	rc = XN_STATUS_OK;

	if(  m_streamFrom == unknowStream )
		return NULL;

	if( (rc = m_xn_context.WaitAndUpdateAll()) != XN_STATUS_EOF ){
		if (rc != XN_STATUS_OK) {
			std::printf("Read next frame failed : %s\n", xnGetStatusString(rc));
			return NULL;
		}
	}
	
	m_xn_depthGenerator.GetMetaData(m_xn_depthMetadata);
	m_xn_imageGenerator.GetMetaData(m_xn_imageMetaData);

	dumpDepth();
	dumpRGB();

	//create new RGBDImage
	if (m_resultRGBDImage == NULL) {
	  m_resultRGBDImage = new RGBDImage();
    }
	m_resultRGBDImage->setData( rgb, depth );

	return m_resultRGBDImage;
}


// --------------------------------------------------------------------------
void RGBDImageProducer::dumpRGB()
{
	int idx = 0;
	const XnUInt8* pImage = m_xn_imageMetaData.Data();
	const XnRGB24Pixel* pImageRow = m_xn_imageMetaData.RGB24Data();
	for (XnUInt y = 0; y < m_xn_imageMetaData.YRes(); ++y)
	{
		const XnRGB24Pixel* pImage = pImageRow;
		for (XnUInt x = 0; x < m_xn_imageMetaData.XRes(); ++x, ++pImage, ++idx)
		{
			rgb[idx*3] = (*pImage).nRed;
			rgb[idx*3+1] = (*pImage).nGreen;
			rgb[idx*3+2] = (*pImage).nBlue;
		}
		pImageRow += m_xn_imageMetaData.XRes();
	}
}

// --------------------------------------------------------------------------
void RGBDImageProducer::dumpDepth()
{
	//XnPoint3D nProjection[640*480];
	//XnPoint3D nRealWorld[640*480];

	XnStatus nRetVal = XN_STATUS_OK;

	int idx =0;

	const XnDepthPixel* pDepth = m_xn_depthGenerator.GetDepthMap();

	for (XnUInt y = 0; y < 480; ++y)
	{
		for (XnUInt x = 0; x < 640; ++x, ++pDepth)
		{
			m_nProjection[idx].X = x;  //transfer
			m_nProjection[idx].Y = y;
			m_nProjection[idx].Z = *pDepth;
			idx++;
		}
	}


	nRetVal = m_xn_depthGenerator.ConvertProjectiveToRealWorld(_640x480, m_nProjection, m_nRealWorld);
	if( nRetVal != XN_STATUS_OK ){
		std::cerr<<"ConvertProjectiveToRealWorld failed : "<<xnGetStatusString(nRetVal)<<std::endl;
	}

	idx = 0;
	for (XnUInt y = 0; y < 480; ++y)
	{
		for (XnUInt x = 0; x < 640; ++x)
		{
 			depth[(y*640+x)] = m_nRealWorld[idx].Z;
			idx++;
		}
	}

}

