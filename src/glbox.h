/*! \file glbox.h
 *   This is the header file where is defined the class GLBox
 */
#ifndef GLBOX_H
#define GLBOX_H

#include <qgl.h>
#include <qevent.h>
#include <cmath>

#include "globjects.h"

//!  GLBox class, the OpenGL widget.
/*!
 * This is where all the figures of the 3D virtual world will be
 * seen using OpenGL.
 */
class GLBox : public QGLWidget
{

   Q_OBJECT

  private:

   //! Angle of Rotation in X axis.
   int xRot;
   //! Angle of Rotation in Y axis.
   int yRot;
   //! Angle of Rotation in Z axis.
   int zRot;

   //!
   /*!
    *
    */
   GLObjects *glObj;

   //!
   /*!
    *
    */
   GLfloat xTrans;
   //!
   /*!
    *
    */
   GLfloat yTrans;
   //!
   /*!
    *
    */
   GLfloat zTrans;

   //! Is the last position clicked in the widget.
   //!
   /*!
    *
    */
   QPoint lastPosClicked;
   
  public:

   //! The constructor of the GLBox class.
   /*!
    *  Here is where the values of variables are initialized.
    *  \param parent is the parent widget of the GLBox.
    *  \param name is the name of the GLBox.
    */
   GLBox( QWidget *parent = 0);
   //! The destructor of the GLBox class.
   /*!
    *  Here we clean and delete all the allocated resources.
    */
   ~GLBox();

   //!
   /*!
    *
    */
   void addGLObjects(GLObjects *glO);

   //!
   /*!
    *
    */
   void reset();

  public slots:

   //! Set the rotation angle around the X axis.
   /*! Set the rotation angle of the object to angle
    *  degrees around the X axis.
    *  \param angle are the degrees to rotate.
    */
   void setXRotation( int angle );
   //! Set the rotation angle around the Y axis.
   /*! Set the rotation angle of the object to angle
    *  degrees around the Y axis.
    *  \param angle are the degrees to rotate.
    */
   void setYRotation( int angle );
   //! Set the rotation angle around the Z axis.
   /*! Set the rotation angle of the object to angle
    *  degrees around the Z axis.
    *  \param angle are the degrees to rotate.
    */
   void setZRotation( int angle );

   //!
   /*!
    *
    */
   void setXTranslation(int xT);
   //!
   /*!
    *
    */
   void setYTranslation(int yT);
   //!
   /*!
    *
    */
   void setZTranslation(int zT);

  signals:

   //! Notify when the rotation of X axis changes.
   /*! Send the signal that the angle of rotation was changed
    *  on the X axis.
    *  \param angle are the rotated degrees.
    */
   void xRotationChanged(int angle);
   //! Notify when the rotation of Y axis changes.
   /*! Send the signal that the angle of rotation was changed
    *  on the Y axis.
    *  \param angle are the rotated degrees.
    */
   void yRotationChanged(int angle);
   //! Notify when the rotation of Z axis changes.
   /*! Send the signal that the angle of rotation was changed
    *  on the Z axis.
    *  \param angle are the rotated degrees.
    */
   void zRotationChanged(int angle);

   //!
   /*!
    *
    */
   void xTranslationChanged(int xT);
   //!
   /*!
    *
    */
   void yTranslationChanged(int yT);
   //!
   /*!
    *
    */
   void zTranslationChanged(int zT);

   //!
   /*!
    *
    */
   void mouseDblClick();

  protected:

   //! Initialices the configuration of OpenGL.
   /*! Set up the OpenGL rendering state, and define display list
    */
   void initializeGL();
   //! Paint the box. 
   /*! The actual openGL commands for drawing the box are
    *  performed here
    */
   void paintGL();
   //! Reconfigure the OpenGL.
   /*! Set up the OpenGL view port, matrix mode, etc.
    *  \param w is the width of the new viewport.
    *  \param h is the height of the new viewport.
    */
   void resizeGL( int w, int h );
   //! Receive mouse press events from the widget. 
   /*! This method is called when the mouse is pressed
    *  inside the widget.
    *  \param event is the mouse event objet.
    */
   void mousePressEvent(QMouseEvent *event);
   //! Receive mouse move events from the widget. 
   /*! This method is called when the mouse is moved
    *  inside the widget.
    *  \param event is the mouse event objet.
    */
   void mouseMoveEvent(QMouseEvent *event);

   //!
   /*!
    *
    */
   void mouseDoubleClickEvent(QMouseEvent *event);

};

#endif

