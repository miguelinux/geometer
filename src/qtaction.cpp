/*! \file qtaction.cpp
 *   This is where the QtAction class is implemented.
 */
#include "qtaction.h"

QtAction::QtAction(QString text, QObject *parent, int shortcut)
	: QAction(parent)
{
  setText(text);

  // if a shortcut is set, then define it
  if (shortcut) {
    #if QT_VERSION >= 0x040000
    setShortcut(shortcut);
    #else
    setAccel(shortcut);
    #endif
  }

  // Connectin the trigged signel with anyAction to emit
  // someAction signal.
  #if QT_VERSION >= 0x040000
  QObject::connect( this, SIGNAL(triggered()),
		    this, SLOT(anyAction()) );

  #else
  QObject::connect( this, SIGNAL(activated()),
		    this, SLOT(anyAction()) );
  #endif
}
	
QtAction::~QtAction(){
}

void QtAction::setParameter(unsigned int param){
  parameter = param;
}

void QtAction::anyAction() {
  // Now emit the signal with "parameter" value in it.
  emit someAction(parameter);
}

