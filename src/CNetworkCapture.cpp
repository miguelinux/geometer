// *****************************************************************
// This file is part of the IVT - Integrating Vision Toolkit.
//
// Copyright (C) 2004-2009 Pedram Azad, Chair Prof. Dillmann (IAIM),
// Institute for Computer Science and Engineering,
// University of Karlsruhe. All rights reserved.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public
// License along with this program; if not, write to the Free
// Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
// Boston, MA 02110-1301, USA.
// *****************************************************************

// *****************************************************************
// Filename:  VideoCaptureInterface.h
// Copyright: Pedram Azad, Chair Prof. Dillmann (IAIM),
//            Institute for Computer Science and Engineering (CSE),
//            University of Karlsruhe. All rights reserved.
// Author:    Pedram Azad
// Date:      2004
// *****************************************************************

#if 0

#ifndef _VIDEO_CAPTURE_INTERFACE_H_
#define _VIDEO_CAPTURE_INTERFACE_H_


// *****************************************************************
// necessary includes
// *****************************************************************

#include "Image/ByteImage.h"



// *****************************************************************
// CVideoCaptureInterface
// *****************************************************************

class CVideoCaptureInterface
{
public:
	enum VideoMode
	{
		e320x240,
		e640x480,
		e800x600,
		e768x576,
		e1024x768,
		e1280x960,
		eNone
	};
	
	enum ColorMode
	{
		eRGB24,
		eBayerPatternToRGB24,
		eGrayScale,
		eYUV411ToRGB24
	};

	enum FrameRate
	{
		e60fps,
		e30fps,
		e15fps,
		e7_5fps,
		e3_75fps,
		e1_875fps
	};

	virtual bool OpenCamera() = 0;
	virtual void CloseCamera() = 0;
	virtual bool CaptureImage(CByteImage **ppImages) = 0;

	virtual int GetWidth() = 0;
	virtual int GetHeight() = 0;
	virtual CByteImage::ImageType GetType() = 0;
	virtual int GetNumberOfCameras() = 0;
	
	virtual ~CVideoCaptureInterface() { }
};



#endif /* _VIDEO_CAPTURE_INTERFACE_H_ */

#endif

