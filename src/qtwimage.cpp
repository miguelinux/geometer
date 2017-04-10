/*! \file qtwimage.cpp
 *   This is where the QtWImage class is implemented.
 */
#include <qpainter.h>
#include <qimage.h>

#include "qtwimage.h"

QtWImage::QtWImage(unsigned short width, unsigned short height,
            WEDImage *winEventInt, QWidget * parent) 
         : QWidget(parent)
{

  // Set the size of the window and dont permit to resize it.
  setFixedSize(width, height);

   // Initialize variables
   m_pBuffer = 0;
   imgBuff32w = 0;

   imgWidth = 0 ;
   imgHeight = 0;

   imgType = -1;

   wEvent = winEventInt;
}

QtWImage::~QtWImage(){
  // If we assign memory to m_pBuffer, then delete it on exit.
  if (m_pBuffer) delete [] m_pBuffer;
}
	
void QtWImage::drawImage(unsigned char *rawImage,
		  unsigned short w, unsigned short h, int type,
		  int x, int y) {

  // Number of pixels per image
  unsigned int nPixels;
  // Pointer to the image raw data (pixels)
  unsigned char *rawSource = rawImage;

  if (imgWidth != w || imgHeight != h) {
    imgWidth = w;
    imgHeight = h;

    if (m_pBuffer)
      delete [] m_pBuffer;

    m_pBuffer = new unsigned char[w * h * 4];

  }

  imgDspX = x;
  imgDspY = y;
  imgType = type;

  // Calculate the number of pixels
  nPixels = imgWidth * imgHeight;

  // Point to the local raw data
  imgBuff32w = (unsigned int *) m_pBuffer;

  if (imgType == CByteImage::eGrayScale) {
    while(nPixels--) {
      *imgBuff32w++ = QTBYTE_ALPHA32 | *rawSource << 16 | *rawSource << 8 | *rawSource;
      rawSource++;
    }
  }
  if (imgType == CByteImage::eRGB24) {
    while(nPixels--) {
      *imgBuff32w++ = QTBYTE_ALPHA32 | rawSource[0] << 16 | rawSource[1] << 8 | rawSource[2];
      rawSource += 3;
    }
    /****************************************************************************
     * This algorithm is useful but it storage in 0xABGR format.                *
     * Doesn't erase to improve it later.                                       *
     * **************************************************************************
    if (nPixels % 4 == 0) {
      // Pointer to the image raw data (pixels)
      unsigned int *rawSource = (unsigned int *) rawImage;
      nPixels >>= 2; // is the same as nPixels /= 4;
      
      while(nPixels--) {
        *imgBuff32w++ = QTBYTE_ALPHA32 |  (rawSource[0] & 0x00ffffff);
  
        *imgBuff32w++ = QTBYTE_ALPHA32 | ((rawSource[0] & 0xff000000) >> 24) |
	                                 ((rawSource[1] & 0x0000ffff) << 8);
  
        *imgBuff32w++ = QTBYTE_ALPHA32 | ((rawSource[1] & 0xffff0000) >> 16) |
	                                 ((rawSource[2] & 0x000000ff) << 16);
  
        *imgBuff32w++ = QTBYTE_ALPHA32 | ((rawSource[2] & 0xffffff00) >>  8);
  
        rawSource += 3;
      } // while(nPixels--) 
    } // if (nPixels % 4 == 0) 
    *******************************************************************************
    *******************************************************************************/
  }  // if (imgType == CByteImage::eRGB24) 

  #if QT_VERSION >= 0x040000
  repaint();
  #else
  repaint(false);
  #endif
}

void QtWImage::drawImage(CByteImage *img) {
  this->drawImage(img->pixels,
	  (unsigned short) img->width, (unsigned short) img->height, img->type);
}

void QtWImage::paintEvent(QPaintEvent *paintEvent){
  QPainter painter(this);
  if (imgType != -1) {

    #if QT_VERSION >= 0x040000
    QImage image(m_pBuffer, imgWidth, imgHeight, QImage::Format_RGB32);
    #else
    QImage image(m_pBuffer, imgWidth, imgHeight, 32, 0, 0, QImage::BigEndian);
    #endif

    painter.drawImage(imgDspX, imgDspY, image);
  } else {
    painter.drawText(this->height() >> 1,this->height() >> 1,tr("Image Not Loaded"));
  }
}

void QtWImage::mousePressEvent(QMouseEvent *event){
  // if wEvent is not declared then return.
  if (!wEvent) return;
  
  wEvent->click(event);

}

void QtWImage::mouseMoveEvent(QMouseEvent *event){
  // if wEvent is not declared then return.
  if (!wEvent) return;

  wEvent->move(event);

}

void QtWImage::mouseDoubleClickEvent(QMouseEvent *event) {
  // if wEvent is not declared then return.
  if (!wEvent) return;

  wEvent->doubleClick(event);

}

