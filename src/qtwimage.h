/*! \file qtWImage.h
 *   This is the header file where is defined the class QtWImage.
 *   The QtWImage is a Qt widget image, almos the same as QTWindow, but
 *   focused as simple widget.
 */
#ifndef _QT_WIDGET_IMGAGE_H
#define _QT_WIDGET_IMGAGE_H

#include <qwidget.h>

#include "Image/ByteImage.h"
#include "commonvars.h"
#include "wedimage.h"


//!  QtWImage class, a Qt Widget Image.
/*!
 * This is where the images will be displayed using a simple Qt widget.
 */
class QtWImage : public QWidget
{

  Q_OBJECT
	
  private:
	
   //! Pixel buffer member.
   /*! This is where the pixel will be stored in raw format.
    */
   unsigned char *m_pBuffer;
   //! Image buffer of 32 bits word.
   /*! This pointer helps us to improve the copy of the image.
    */
   unsigned int  *imgBuff32w;
   //! The image width.
   unsigned short imgWidth;
   //! The image height.
   unsigned short imgHeight;

   //! The X initial position to display the image.
   int imgDspX;
   //! The Y initial position to display the image.
   int imgDspY;
   //! The type of the image Grayscale or RGB
   int imgType;

   //! The internal window event interface.
   WEDImage *wEvent;

  public:
   //! The constructor of the QtWImage class.
   /*!
    *  Here is where the values of variables are initialized.
    *  \param width is the width of the widget (image) to display.
    *  \param height is the height of the widget (image) to display.
    *  \param winEventInt is the Event objet that keep the mouse event data.
    *  \param parent is the widget parent of this widget.
    */
   QtWImage(unsigned short width, unsigned short height,
            WEDImage *winEventInt = 0, QWidget * parent = 0);
	
   //! The destructor of the QtWImage class.
   /*!
    *  Clear all the used variables.
    */
   ~QtWImage();
   //! Draw an image in the widget.
   /*! Draw an image in the widget using the raw data of the image.
    *  \param rawImage is the raw data of the image.
    *  \param w is the width of the image.
    *  \param h is the height of the image.
    *  \param type is the type of the image GrayScale or RGB.
    *  \param x is the inital coodinate position to display the image
    *         into the widget.
    *  \param y is the inital coordinate position to display the image
    *         into the widget.
    */
   void drawImage(unsigned char *rawImage,
		  unsigned short w, unsigned short h, int type = -1,
		  int x = 0, int y = 0);

   //! Draw a byte image in the widget.
   /*! Is the same as drawImage(), but only recibe the image parameter and
    *  put the intal coordinates in position (0,0).
    *  \param img is the byte image to display
    *  \sa drawImage().
    */
   void drawImage(CByteImage *img);

  protected:

   //! Is the event where the widget is painted.
   /*! This method is called when the widget needs to repaint on the screen,
    *  and here is where the image is drawing on the widget.
    *  \param paintEvent is the paint event objet.
    */
   void paintEvent(QPaintEvent *paintEvent);
   //! Receive mouse press events for the widget. 
   /*! This method is called when the mouse is pressed
    *  inside the widget.
    *  \param event is the mouse event objet.
    */
   void mousePressEvent(QMouseEvent *event);
   //! Receive mouse move events for the widget. 
   /*! This method is called when the mouse is moved
    *  inside the widget.
    *  \param event is the mouse event objet.
    */
   void mouseMoveEvent(QMouseEvent *event);
   //! Receive mouse double click events for the widget. 
   /*! This method is called when the mouse is double
    *  clicked inside the widget.
    *  \param event is the mouse event objet.
    */
   void mouseDoubleClickEvent(QMouseEvent *event);
	
};

#endif /* _QT_BILD_H_ */

