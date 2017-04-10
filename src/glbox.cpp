/*! \file glbox.cpp
 *   This is where the GLBox class is implemented.
 */
#include "glbox.h"

#if defined(Q_CC_MSVC)
#pragma warning(disable:4305) // init: truncation from const double to float
#endif

GLBox::GLBox( QWidget* parent )
    : QGLWidget( parent )
{
    // default object rotation
    xRot = -65;
    yRot = zRot = 0;
    xTrans = 0.0f;
    yTrans = -10.0f;
    zTrans = -50.0f;
    glObj = 0;
}

GLBox::~GLBox() {
    makeCurrent();
}

void GLBox::addGLObjects(GLObjects *glO){
  glObj = glO;
}

void GLBox::reset() {
    setXRotation(-65);
    setYRotation(0);
    setZRotation(0);
    setXTranslation(0);
    setYTranslation(-10);
    setZTranslation(-50);
}

void GLBox::paintGL()
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    //glScalef( scale, scale, scale );

    glTranslatef( xTrans, yTrans, zTrans );
    glRotatef( (GLfloat) xRot, 1.0f, 0.0f, 0.0f ); 
    glRotatef( (GLfloat) yRot, 0.0f, 1.0f, 0.0f ); 
    glRotatef( (GLfloat) zRot, 0.0f, 0.0f, 1.0f );

    glObj->draw();
}

void GLBox::initializeGL()
{

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0f,
		 (GLfloat) this->width() /(GLfloat)  this->height(),
		 //1.0f, 1000.0f);
		 1.0f, 2000.0f);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glClearColor(0.83f, 0.81f, 0.78f, 1.0f);

  glClearDepth(1.0f);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  // para las transparencias
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  //glBlendFunc(GL_SRC_ALPHA, GL_ONE);

  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

  glShadeModel( GL_FLAT );

  glObj->setup(0.01f);

}

void GLBox::resizeGL( int w, int h )
{
    glViewport( 0, 0, (GLint)w, (GLint)h );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    //glFrustum( -1.0, 1.0, -1.0, 1.0, 5.0, 15.0 );
    gluPerspective(45.0f,
		 (GLfloat) this->width() /(GLfloat)  this->height(),
		 1.0f, 2000.0f);
    glMatrixMode( GL_MODELVIEW );
}

void GLBox::setXRotation( int angle ) {
    // If is the same angle then return (do nothing)
    if ((angle %= 360) == xRot) return;

    xRot = angle;
    // emiting the xRotationChanged signal
    emit xRotationChanged(angle);
    updateGL();
}


void GLBox::setYRotation( int angle ) {
    // If is the same angle then return (do nothing)
    if ((angle %= 360) == yRot) return;

    yRot = angle;
    // emiting the yRotationChanged signal
    emit yRotationChanged(angle);
    updateGL();
}

void GLBox::setZRotation( int angle ) {
    // If is the same angle then return (do nothing)
    if ((angle %= 360) == zRot) return;

    zRot = angle;
    // emiting the zRotationChanged signal
    emit zRotationChanged(angle);
    updateGL();
}

void GLBox::setXTranslation(int xT){
    xTrans = xT;
    // emiting the xTranslationChanged signal
    emit xTranslationChanged(xT);
    updateGL();
}

void GLBox::setYTranslation(int yT){
    yTrans = yT;
    // emiting the yTranslationChanged signal
    emit yTranslationChanged(yT);
    updateGL();
}

void GLBox::setZTranslation(int zT){
    zTrans = zT;
    // emiting the zTranslationChanged signal
    emit zTranslationChanged(zT);
    updateGL();
}

void GLBox::mousePressEvent(QMouseEvent *event)
{
    lastPosClicked = event->pos();
}

void GLBox::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - lastPosClicked.x();
    int dy = event->y() - lastPosClicked.y();

    #if QT_VERSION >= 0x040000
    if (event->buttons() & Qt::LeftButton) 
    #else
    if (event->state() & Qt::LeftButton) 
    #endif
    {
        setXRotation(xRot + dy);
        //setYRotation(yRot + dx);
        setZRotation(zRot + dx);
    }
    #if QT_VERSION >= 0x040000
    else if (event->buttons() & Qt::RightButton)
    #else
    else if (event->state() & Qt::RightButton) 
    #endif
    {
        //setXTranslation(xTrans + (dx/8.0f)*std::cos(float(xRot)));
        //setZTranslation(zTrans + (dy/16.0f)*std::sin(float(zRot)));
        //setXTranslation(int(xTrans + dx/8.0f));
        setXTranslation(int(xTrans + dx));
        //setZTranslation(int(zTrans + dy/6.0f));
        setZTranslation(int(zTrans + dy));

    }
    lastPosClicked = event->pos();
}

void GLBox::mouseDoubleClickEvent(QMouseEvent *event) {
    emit mouseDblClick();
}

