/*! \file 
 */
#ifndef _WINDOW_EVENT_DISPLAY_IMAGE_H
#define _WINDOW_EVENT_DISPLAY_IMAGE_H

#include <qobject.h>
#include <qevent.h> 

#include "commonvars.h"

class WEDImage : public QObject
 {

  Q_OBJECT
	
  private:

    unsigned int numberOfImage;

  public:

    QMouseEvent *mouseEvent;

    WEDImage();

    void setImage(unsigned int nImage);

    void click(QMouseEvent *mE);
    void move(QMouseEvent *mE);
    void doubleClick(QMouseEvent *mE);

  signals:
    
    void sClick(unsigned int nI);
    void sMove(unsigned int nI);
    void sDoubleClick(unsigned int nI);

};

#endif

