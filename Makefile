#############################################################################
# Makefile for building: bin/amros
# Generated by qmake (2.01a) (Qt 4.6.2) on: lun abr 18 09:38:10 2011
# Project:  projekt/main.pro
# Template: app
# Command: /usr/bin/qmake -unix -o Makefile projekt/main.pro
#############################################################################

####### Compiler, tools and options

CC            = gcc
CXX           = g++
DEFINES       = -DQT_NO_DEBUG -DQT_OPENGL_LIB -DQT_GUI_LIB -DQT_CORE_LIB -DQT_SHARED
CFLAGS        = -m64 -pipe -O2 -D_REENTRANT -Wall -W $(DEFINES)
CXXFLAGS      = -m64 -pipe -O2 -D_REENTRANT -Wall -W $(DEFINES)
INCPATH       = -I/usr/lib64/qt/mkspecs/linux-g++-64 -Iprojekt -I/usr/lib64/qt/include/QtCore -I/usr/lib64/qt/include/QtGui -I/usr/lib64/qt/include/QtOpenGL -I/usr/lib64/qt/include -Iprojekt -Iinclude -Iinclude/fiitb -Iinclude -Iinclude/fiitb -I/opt/IVT/src -I/usr/include/opencv -I/usr/local/Aria/include -I/usr/local/Aria/ArNetworking/include -I/usr/include -Itmp -I.
LINK          = g++
LFLAGS        = -m64 -Wl,-O1 -Wl,-rpath,/usr/lib64/qt/lib
LIBS          = $(SUBLIBS)  -L/usr/lib64/qt/lib -L/usr/X11R6/lib64 -lil -L/opt/IVT/lib/linux -livtgui -livtvideocapture -livt -livtopencv -lglut -lcxcore -lcv -lcvaux -lraw1394 -ldc1394 -L/usr/local/Aria/lib -lAria -lArNetworking -lQtOpenGL -L/usr/lib64/qt/lib -L/usr/X11R6/lib64 -lQtGui -lQtCore -lGLU -lGL -lpthread 
AR            = ar cqs
RANLIB        = 
QMAKE         = /usr/bin/qmake
TAR           = tar -cf
COMPRESS      = gzip -9f
COPY          = cp -f
SED           = sed
COPY_FILE     = $(COPY)
COPY_DIR      = $(COPY) -r
STRIP         = strip
INSTALL_FILE  = install -m 644 -p
INSTALL_DIR   = $(COPY_DIR)
INSTALL_PROGRAM = install -m 755 -p
DEL_FILE      = rm -f
SYMLINK       = ln -f -s
DEL_DIR       = rmdir
MOVE          = mv -f
CHK_DIR_EXISTS= test -d
MKDIR         = mkdir -p

####### Output directory

OBJECTS_DIR   = tmp/

####### Files

SOURCES       = src/amrosapp.cpp \
		src/CNetworkCapture.cpp \
		src/configprog.cpp \
		src/findandbuild.cpp \
		src/geometer.cpp \
		src/glamros.cpp \
		src/glbox.cpp \
		src/glgeometer.cpp \
		src/globjects.cpp \
		src/glpowerbot.cpp \
		src/glsick.cpp \
		src/ivtprintf.cpp \
		src/landmark.cpp \
		src/laserlog.cpp \
		src/laserpoint.cpp \
		src/main.cpp \
		src/mainwindow.cpp \
		src/mapbuilder.cpp \
		src/qtaction.cpp \
		src/qtwimage.cpp \
		src/wedimage.cpp \
		src/fiitb/aria_client.cpp tmp/moc_amrosapp.cpp \
		tmp/moc_glbox.cpp \
		tmp/moc_mainwindow.cpp \
		tmp/moc_qtaction.cpp \
		tmp/moc_qtwimage.cpp \
		tmp/moc_wedimage.cpp
OBJECTS       = tmp/amrosapp.o \
		tmp/CNetworkCapture.o \
		tmp/configprog.o \
		tmp/findandbuild.o \
		tmp/geometer.o \
		tmp/glamros.o \
		tmp/glbox.o \
		tmp/glgeometer.o \
		tmp/globjects.o \
		tmp/glpowerbot.o \
		tmp/glsick.o \
		tmp/ivtprintf.o \
		tmp/landmark.o \
		tmp/laserlog.o \
		tmp/laserpoint.o \
		tmp/main.o \
		tmp/mainwindow.o \
		tmp/mapbuilder.o \
		tmp/qtaction.o \
		tmp/qtwimage.o \
		tmp/wedimage.o \
		tmp/aria_client.o \
		tmp/moc_amrosapp.o \
		tmp/moc_glbox.o \
		tmp/moc_mainwindow.o \
		tmp/moc_qtaction.o \
		tmp/moc_qtwimage.o \
		tmp/moc_wedimage.o
DIST          = /usr/lib64/qt/mkspecs/common/g++.conf \
		/usr/lib64/qt/mkspecs/common/unix.conf \
		/usr/lib64/qt/mkspecs/common/linux.conf \
		/usr/lib64/qt/mkspecs/qconfig.pri \
		/usr/lib64/qt/mkspecs/features/qt_functions.prf \
		/usr/lib64/qt/mkspecs/features/qt_config.prf \
		/usr/lib64/qt/mkspecs/features/exclusive_builds.prf \
		/usr/lib64/qt/mkspecs/features/default_pre.prf \
		projekt/projekt/sources.pri \
		projekt/projekt/unix.pri \
		/usr/lib64/qt/mkspecs/features/release.prf \
		/usr/lib64/qt/mkspecs/features/default_post.prf \
		/usr/lib64/qt/mkspecs/features/qt.prf \
		/usr/lib64/qt/mkspecs/features/unix/opengl.prf \
		/usr/lib64/qt/mkspecs/features/unix/thread.prf \
		/usr/lib64/qt/mkspecs/features/moc.prf \
		/usr/lib64/qt/mkspecs/features/warn_on.prf \
		/usr/lib64/qt/mkspecs/features/resources.prf \
		/usr/lib64/qt/mkspecs/features/uic.prf \
		/usr/lib64/qt/mkspecs/features/yacc.prf \
		/usr/lib64/qt/mkspecs/features/lex.prf \
		/usr/lib64/qt/mkspecs/features/include_source_dir.prf \
		projekt/main.pro
QMAKE_TARGET  = amros
DESTDIR       = bin/
TARGET        = bin/amros

first: all
####### Implicit rules

.SUFFIXES: .o .c .cpp .cc .cxx .C

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.C.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.c.o:
	$(CC) -c $(CFLAGS) $(INCPATH) -o "$@" "$<"

####### Build rules

all: Makefile $(TARGET)

$(TARGET):  $(OBJECTS)  
	@$(CHK_DIR_EXISTS) bin/ || $(MKDIR) bin/ 
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(OBJCOMP) $(LIBS)

Makefile: projekt/main.pro  /usr/lib64/qt/mkspecs/linux-g++-64/qmake.conf /usr/lib64/qt/mkspecs/common/g++.conf \
		/usr/lib64/qt/mkspecs/common/unix.conf \
		/usr/lib64/qt/mkspecs/common/linux.conf \
		/usr/lib64/qt/mkspecs/qconfig.pri \
		/usr/lib64/qt/mkspecs/features/qt_functions.prf \
		/usr/lib64/qt/mkspecs/features/qt_config.prf \
		/usr/lib64/qt/mkspecs/features/exclusive_builds.prf \
		/usr/lib64/qt/mkspecs/features/default_pre.prf \
		projekt/sources.pri \
		projekt/unix.pri \
		/usr/lib64/qt/mkspecs/features/release.prf \
		/usr/lib64/qt/mkspecs/features/default_post.prf \
		/usr/lib64/qt/mkspecs/features/qt.prf \
		/usr/lib64/qt/mkspecs/features/unix/opengl.prf \
		/usr/lib64/qt/mkspecs/features/unix/thread.prf \
		/usr/lib64/qt/mkspecs/features/moc.prf \
		/usr/lib64/qt/mkspecs/features/warn_on.prf \
		/usr/lib64/qt/mkspecs/features/resources.prf \
		/usr/lib64/qt/mkspecs/features/uic.prf \
		/usr/lib64/qt/mkspecs/features/yacc.prf \
		/usr/lib64/qt/mkspecs/features/lex.prf \
		/usr/lib64/qt/mkspecs/features/include_source_dir.prf \
		/usr/lib64/qt/lib/libQtOpenGL.prl \
		/usr/lib64/qt/lib/libQtGui.prl \
		/usr/lib64/qt/lib/libQtCore.prl
	$(QMAKE) -unix -o Makefile projekt/main.pro
/usr/lib64/qt/mkspecs/common/g++.conf:
/usr/lib64/qt/mkspecs/common/unix.conf:
/usr/lib64/qt/mkspecs/common/linux.conf:
/usr/lib64/qt/mkspecs/qconfig.pri:
/usr/lib64/qt/mkspecs/features/qt_functions.prf:
/usr/lib64/qt/mkspecs/features/qt_config.prf:
/usr/lib64/qt/mkspecs/features/exclusive_builds.prf:
/usr/lib64/qt/mkspecs/features/default_pre.prf:
projekt/sources.pri:
projekt/unix.pri:
/usr/lib64/qt/mkspecs/features/release.prf:
/usr/lib64/qt/mkspecs/features/default_post.prf:
/usr/lib64/qt/mkspecs/features/qt.prf:
/usr/lib64/qt/mkspecs/features/unix/opengl.prf:
/usr/lib64/qt/mkspecs/features/unix/thread.prf:
/usr/lib64/qt/mkspecs/features/moc.prf:
/usr/lib64/qt/mkspecs/features/warn_on.prf:
/usr/lib64/qt/mkspecs/features/resources.prf:
/usr/lib64/qt/mkspecs/features/uic.prf:
/usr/lib64/qt/mkspecs/features/yacc.prf:
/usr/lib64/qt/mkspecs/features/lex.prf:
/usr/lib64/qt/mkspecs/features/include_source_dir.prf:
/usr/lib64/qt/lib/libQtOpenGL.prl:
/usr/lib64/qt/lib/libQtGui.prl:
/usr/lib64/qt/lib/libQtCore.prl:
qmake:  FORCE
	@$(QMAKE) -unix -o Makefile projekt/main.pro

dist: 
	@$(CHK_DIR_EXISTS) projekt/tmp/amros1.0.0 || $(MKDIR) projekt/tmp/amros1.0.0 
	$(COPY_FILE) --parents $(SOURCES) $(DIST) projekt/tmp/amros1.0.0/ && $(COPY_FILE) --parents include/amrosapp.h include/cameramacros.h include/CNetworkCapture.h include/commonvars.h include/configprog.h include/findandbuild.h include/geometer.h include/glamros.h include/glbox.h include/glgeometer.h include/globjects.h include/glpowerbot.h include/glsick.h include/ivtprintf.h include/landmark.h include/laserlog.h include/laserpoint.h include/mainwindow.h include/mapbuilder.h include/qt3qt4macros.h include/qtaction.h include/qtwimage.h include/wedimage.h include/fiitb/aria_client.h include/fiitb/matrixt.h projekt/tmp/amros1.0.0/ && $(COPY_FILE) --parents src/amrosapp.cpp src/CNetworkCapture.cpp src/configprog.cpp src/findandbuild.cpp src/geometer.cpp src/glamros.cpp src/glbox.cpp src/glgeometer.cpp src/globjects.cpp src/glpowerbot.cpp src/glsick.cpp src/ivtprintf.cpp src/landmark.cpp src/laserlog.cpp src/laserpoint.cpp src/main.cpp src/mainwindow.cpp src/mapbuilder.cpp src/qtaction.cpp src/qtwimage.cpp src/wedimage.cpp src/fiitb/aria_client.cpp projekt/tmp/amros1.0.0/ && (cd `dirname projekt/tmp/amros1.0.0` && $(TAR) amros1.0.0.tar amros1.0.0 && $(COMPRESS) amros1.0.0.tar) && $(MOVE) `dirname projekt/tmp/amros1.0.0`/amros1.0.0.tar.gz . && $(DEL_FILE) -r projekt/tmp/amros1.0.0


clean:compiler_clean 
	-$(DEL_FILE) $(OBJECTS)
	-$(DEL_FILE) *~ core *.core


####### Sub-libraries

distclean: clean
	-$(DEL_FILE) $(TARGET) 
	-$(DEL_FILE) Makefile


mocclean: compiler_moc_header_clean compiler_moc_source_clean

mocables: compiler_moc_header_make_all compiler_moc_source_make_all

compiler_moc_header_make_all: tmp/moc_amrosapp.cpp tmp/moc_glbox.cpp tmp/moc_mainwindow.cpp tmp/moc_qtaction.cpp tmp/moc_qtwimage.cpp tmp/moc_wedimage.cpp
compiler_moc_header_clean:
	-$(DEL_FILE) tmp/moc_amrosapp.cpp tmp/moc_glbox.cpp tmp/moc_mainwindow.cpp tmp/moc_qtaction.cpp tmp/moc_qtwimage.cpp tmp/moc_wedimage.cpp
tmp/moc_amrosapp.cpp: include/geometer.h \
		include/fiitb/aria_client.h \
		include/fiitb/matrixt.h \
		include/configprog.h \
		include/mainwindow.h \
		include/qtaction.h \
		include/qtwimage.h \
		include/commonvars.h \
		include/wedimage.h \
		include/glbox.h \
		include/globjects.h \
		include/glamros.h \
		include/glsick.h \
		include/glpowerbot.h \
		include/glgeometer.h \
		include/qt3qt4macros.h \
		include/cameramacros.h \
		include/findandbuild.h \
		include/landmark.h \
		include/ivtprintf.h \
		include/mapbuilder.h \
		include/laserlog.h \
		include/laserpoint.h \
		include/amrosapp.h
	/usr/lib64/qt/bin/moc $(DEFINES) $(INCPATH) include/amrosapp.h -o tmp/moc_amrosapp.cpp

tmp/moc_glbox.cpp: include/globjects.h \
		include/glamros.h \
		include/glsick.h \
		include/glpowerbot.h \
		include/glgeometer.h \
		include/commonvars.h \
		include/glbox.h
	/usr/lib64/qt/bin/moc $(DEFINES) $(INCPATH) include/glbox.h -o tmp/moc_glbox.cpp

tmp/moc_mainwindow.cpp: include/qtaction.h \
		include/qtwimage.h \
		include/commonvars.h \
		include/wedimage.h \
		include/glbox.h \
		include/globjects.h \
		include/glamros.h \
		include/glsick.h \
		include/glpowerbot.h \
		include/glgeometer.h \
		include/qt3qt4macros.h \
		include/mainwindow.h
	/usr/lib64/qt/bin/moc $(DEFINES) $(INCPATH) include/mainwindow.h -o tmp/moc_mainwindow.cpp

tmp/moc_qtaction.cpp: include/qtaction.h
	/usr/lib64/qt/bin/moc $(DEFINES) $(INCPATH) include/qtaction.h -o tmp/moc_qtaction.cpp

tmp/moc_qtwimage.cpp: include/commonvars.h \
		include/wedimage.h \
		include/qtwimage.h
	/usr/lib64/qt/bin/moc $(DEFINES) $(INCPATH) include/qtwimage.h -o tmp/moc_qtwimage.cpp

tmp/moc_wedimage.cpp: include/commonvars.h \
		include/wedimage.h
	/usr/lib64/qt/bin/moc $(DEFINES) $(INCPATH) include/wedimage.h -o tmp/moc_wedimage.cpp

compiler_rcc_make_all:
compiler_rcc_clean:
compiler_image_collection_make_all: qmake_image_collection.cpp
compiler_image_collection_clean:
	-$(DEL_FILE) qmake_image_collection.cpp
compiler_moc_source_make_all:
compiler_moc_source_clean:
compiler_uic_make_all:
compiler_uic_clean:
compiler_yacc_decl_make_all:
compiler_yacc_decl_clean:
compiler_yacc_impl_make_all:
compiler_yacc_impl_clean:
compiler_lex_make_all:
compiler_lex_clean:
compiler_clean: compiler_moc_header_clean 

####### Compile

tmp/amrosapp.o: src/amrosapp.cpp include/amrosapp.h \
		include/geometer.h \
		include/fiitb/aria_client.h \
		include/fiitb/matrixt.h \
		include/configprog.h \
		include/mainwindow.h \
		include/qtaction.h \
		include/qtwimage.h \
		include/commonvars.h \
		include/wedimage.h \
		include/glbox.h \
		include/globjects.h \
		include/glamros.h \
		include/glsick.h \
		include/glpowerbot.h \
		include/glgeometer.h \
		include/qt3qt4macros.h \
		include/cameramacros.h \
		include/findandbuild.h \
		include/landmark.h \
		include/ivtprintf.h \
		include/mapbuilder.h \
		include/laserlog.h \
		include/laserpoint.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o tmp/amrosapp.o src/amrosapp.cpp

tmp/CNetworkCapture.o: src/CNetworkCapture.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o tmp/CNetworkCapture.o src/CNetworkCapture.cpp

tmp/configprog.o: src/configprog.cpp include/configprog.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o tmp/configprog.o src/configprog.cpp

tmp/findandbuild.o: src/findandbuild.cpp include/findandbuild.h \
		include/landmark.h \
		include/commonvars.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o tmp/findandbuild.o src/findandbuild.cpp

tmp/geometer.o: src/geometer.cpp include/geometer.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o tmp/geometer.o src/geometer.cpp

tmp/glamros.o: src/glamros.cpp include/glamros.h \
		include/glsick.h \
		include/glpowerbot.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o tmp/glamros.o src/glamros.cpp

tmp/glbox.o: src/glbox.cpp include/glbox.h \
		include/globjects.h \
		include/glamros.h \
		include/glsick.h \
		include/glpowerbot.h \
		include/glgeometer.h \
		include/commonvars.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o tmp/glbox.o src/glbox.cpp

tmp/glgeometer.o: src/glgeometer.cpp include/glgeometer.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o tmp/glgeometer.o src/glgeometer.cpp

tmp/globjects.o: src/globjects.cpp include/globjects.h \
		include/glamros.h \
		include/glsick.h \
		include/glpowerbot.h \
		include/glgeometer.h \
		include/commonvars.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o tmp/globjects.o src/globjects.cpp

tmp/glpowerbot.o: src/glpowerbot.cpp include/glpowerbot.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o tmp/glpowerbot.o src/glpowerbot.cpp

tmp/glsick.o: src/glsick.cpp include/glsick.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o tmp/glsick.o src/glsick.cpp

tmp/ivtprintf.o: src/ivtprintf.cpp include/ivtprintf.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o tmp/ivtprintf.o src/ivtprintf.cpp

tmp/landmark.o: src/landmark.cpp include/landmark.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o tmp/landmark.o src/landmark.cpp

tmp/laserlog.o: src/laserlog.cpp include/laserlog.h \
		include/laserpoint.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o tmp/laserlog.o src/laserlog.cpp

tmp/laserpoint.o: src/laserpoint.cpp include/laserpoint.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o tmp/laserpoint.o src/laserpoint.cpp

tmp/main.o: src/main.cpp include/amrosapp.h \
		include/geometer.h \
		include/fiitb/aria_client.h \
		include/fiitb/matrixt.h \
		include/configprog.h \
		include/mainwindow.h \
		include/qtaction.h \
		include/qtwimage.h \
		include/commonvars.h \
		include/wedimage.h \
		include/glbox.h \
		include/globjects.h \
		include/glamros.h \
		include/glsick.h \
		include/glpowerbot.h \
		include/glgeometer.h \
		include/qt3qt4macros.h \
		include/cameramacros.h \
		include/findandbuild.h \
		include/landmark.h \
		include/ivtprintf.h \
		include/mapbuilder.h \
		include/laserlog.h \
		include/laserpoint.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o tmp/main.o src/main.cpp

tmp/mainwindow.o: src/mainwindow.cpp include/mainwindow.h \
		include/qtaction.h \
		include/qtwimage.h \
		include/commonvars.h \
		include/wedimage.h \
		include/glbox.h \
		include/globjects.h \
		include/glamros.h \
		include/glsick.h \
		include/glpowerbot.h \
		include/glgeometer.h \
		include/qt3qt4macros.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o tmp/mainwindow.o src/mainwindow.cpp

tmp/mapbuilder.o: src/mapbuilder.cpp include/mapbuilder.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o tmp/mapbuilder.o src/mapbuilder.cpp

tmp/qtaction.o: src/qtaction.cpp include/qtaction.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o tmp/qtaction.o src/qtaction.cpp

tmp/qtwimage.o: src/qtwimage.cpp include/qtwimage.h \
		include/commonvars.h \
		include/wedimage.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o tmp/qtwimage.o src/qtwimage.cpp

tmp/wedimage.o: src/wedimage.cpp include/wedimage.h \
		include/commonvars.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o tmp/wedimage.o src/wedimage.cpp

tmp/aria_client.o: src/fiitb/aria_client.cpp include/fiitb/aria_client.h \
		include/fiitb/matrixt.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o tmp/aria_client.o src/fiitb/aria_client.cpp

tmp/moc_amrosapp.o: tmp/moc_amrosapp.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o tmp/moc_amrosapp.o tmp/moc_amrosapp.cpp

tmp/moc_glbox.o: tmp/moc_glbox.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o tmp/moc_glbox.o tmp/moc_glbox.cpp

tmp/moc_mainwindow.o: tmp/moc_mainwindow.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o tmp/moc_mainwindow.o tmp/moc_mainwindow.cpp

tmp/moc_qtaction.o: tmp/moc_qtaction.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o tmp/moc_qtaction.o tmp/moc_qtaction.cpp

tmp/moc_qtwimage.o: tmp/moc_qtwimage.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o tmp/moc_qtwimage.o tmp/moc_qtwimage.cpp

tmp/moc_wedimage.o: tmp/moc_wedimage.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o tmp/moc_wedimage.o tmp/moc_wedimage.cpp

####### Install

install:   FORCE

uninstall:   FORCE

FORCE:

