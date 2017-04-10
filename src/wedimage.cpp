#include "wedimage.h"

WEDImage::WEDImage() {
  // No image asigned
  numberOfImage=0;
}

void WEDImage::setImage(unsigned int nImage) {
  numberOfImage=nImage;
}

void WEDImage::click(QMouseEvent *mE) {
  mouseEvent = mE;
  emit sClick(numberOfImage | MOUSE_CLICK);
}

void WEDImage::move(QMouseEvent *mE) {
  mouseEvent = mE;
  emit sMove(numberOfImage | MOUSE_MOVE);
}

void WEDImage::doubleClick(QMouseEvent *mE) {
  mouseEvent = mE;
  emit sDoubleClick(numberOfImage | MOUSE_DBLCLK);
}

