// ****************************************************************************
// This file is part of the Integrating Vision Toolkit (IVT).
//
// The IVT is maintained by the Karlsruhe Institute of Technology (KIT)
// (www.kit.edu) in cooperation with the company Keyetech (www.keyetech.de).
//
// Copyright (C) 2010 Karlsruhe Institute of Technology (KIT).
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//
// 3. Neither the name of the KIT nor the names of its contributors may be
//    used to endorse or promote products derived from this software
//    without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE KIT AND CONTRIBUTORS “AS IS” AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE KIT OR CONTRIBUTORS BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
// THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// ****************************************************************************
// ****************************************************************************
// Filename:  KinectCapture.cpp
// Author:    Miguel BERNAL MARIN
// Date:      2011
// ****************************************************************************


// ****************************************************************************
// Includes
// ****************************************************************************

#include <new> // for explicitly using correct new/delete operators on VC DSPs

#include "KinectCapture.h"

#include "Image/ImageProcessor.h"
#include "Image/ByteImage.h"

#include "rgbdimage.h"
#include "RGBDImageProducer.h"

// ****************************************************************************
// Constructor / Destructor
// ****************************************************************************

CKinectCapture::CKinectCapture(const char *pFilePath) :
    _640x480(640*480)
{
	m_sFilePath = "";
	m_sFilePath += pFilePath;

    m_rgbdProducer = 0;
    m_rgbdImage = 0;

	m_bOK = false;
    m_isFile = false;
}

CKinectCapture::~CKinectCapture()
{
    if (m_rgbdProducer)
        delete m_rgbdProducer;
}


// ****************************************************************************
// Methods
// ****************************************************************************

bool CKinectCapture::OpenCamera()
{
    int s;
    short i;
	m_bOK = false;
    m_isFile = false;

    if (m_rgbdProducer) {
      delete m_rgbdProducer;
    }

    s = m_sFilePath.size();
    if (m_sFilePath[s-4] == 'r' && m_sFilePath[s-1] == 'd') {
      m_isFile = true;
	  m_bOK = true;
      for(i=0; i<4; i++) {
        sPath[i]=m_sFilePath[i+s-9];
      }
      sPath[i]=0;
      numInicial = ::atoi(sPath);
      numNext = numInicial;
      //printf("%d=%s\n", numInicial, sPath);
      for(i=0; i<s-9; i++) {
        sPath[i]=m_sFilePath[i];
      }
      sPath[i]=0;
      //printf("%s\n", sPath);
      m_sFilePath = sPath;
    } else {
      m_rgbdProducer = new RGBDImageProducer(m_sFilePath.c_str());

      if ( !m_rgbdProducer->isConnected() ) {
	    return m_bOK;
      }
	  m_bOK = true;
    }

	return m_bOK;
}

void CKinectCapture::CloseCamera()
{
	m_bOK = false;
    m_isFile = false;
    if (m_rgbdProducer)
        delete m_rgbdProducer;

    m_rgbdProducer = 0;
}

bool CKinectCapture::CaptureImage(CByteImage **ppImages)
{
    const uint8_t *rgbCamera;
    uint8_t *depthCamera;

    uint8_t *rgb2Image;
    uint8_t *depth2Image;
    FILE *f;

	if (!m_bOK)
		return false;

    if (!m_isFile) {
      m_rgbdImage = m_rgbdProducer->getRGBDImage();
    } else {
      if (!m_rgbdImage) {
        m_rgbdImage = new RGBDImage();
      }
      sprintf(sPath,"%s%.4i.rgbd",m_sFilePath.c_str(), numNext);
      f = fopen(sPath,"r");
      if (f == NULL) {
        numNext = numInicial;
        // TODO: is for compatibily with amros
        //sprintf(sPath,"%s%.4i.rgbd",m_sFilePath.c_str(), numNext);
        return false;
      } else {
        fclose(f);
      }
      // TODO: delete next line; only for debug
      printf("%.4i.rgbd\n", numNext);
      numNext++;
      m_rgbdImage->loadFromFile(sPath);
    }

    if (m_rgbdImage == NULL)
		return false;

	depth2Image = reinterpret_cast<uint8_t *>(ppImages[0]->pixels);
	rgb2Image   = reinterpret_cast<uint8_t *>(ppImages[1]->pixels);

    //if (numNext == 12) m_rgbdImage->dumpStats=true;

    depthCamera = m_rgbdImage->bufferDepthVisualize2();
    rgbCamera = m_rgbdImage->bufferRGB();

    ::memcpy(depth2Image, depthCamera, sizeof(uint8_t)*_640x480*3 );
    ::memcpy(rgb2Image,   rgbCamera,   sizeof(uint8_t)*_640x480*3 );

    /**
    depthCamera = m_rgbdImage->bufferDepthVisualize();
    for(int i=0; i<_640x480; i++) {
      *depth2Image++ = *depthCamera;
      *depth2Image++ = *depthCamera;
      *depth2Image++ = *depthCamera;
      depthCamera++;
    }
    **/

	return true;
}

