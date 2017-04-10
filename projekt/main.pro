# Release or Debug project
#CONFIG += debug
CONFIG += release

CONFIG += warn_on

# Use Qt library
CONFIG += qt
QT += opengl

include( sources.pri )

#Name of the application
TARGET = amros

DESTDIR = bin

OBJECTS_DIR = tmp

MOC_DIR = tmp

#INCLUDEPATH += ../include ../include/fiitb

win32 {
  TEMPLATE = vcapp
  include( windows.pri )
}

unix {
  include( unix.pri )

  !exists( $$IVTDIR ) {
    error( "Please update the IVT directory (IVTDIR variable) in unix.pri file." )
  }
  !exists( $$ARIADIR ) {
    error( "Please update the ARIA directory (ARIADIR variable) in unix.pri file." )
  }
}

