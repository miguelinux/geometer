#include "glsick.h"

#if defined(Q_CC_MSVC)
#pragma warning(disable:4305) // init: truncation from const double to float
#endif

GlSick::GlSick(){
  conf=0;
  laserAng=0;
  scale = 1.0f;
}

GlSick::~GlSick(){
  glDeleteLists( oglList, 1 );
}

void GlSick::draw() {
  glCallList( oglList );
  if(conf & GL_SICK_LASER_ON) {
    glPushMatrix();
      glScalef(scale,scale,scale);
      glColor3f(1,0,0);
      glTranslatef(0, SICK_CENTRE_OF_MIRROR_D, SICK_CENTRE_OF_LIGHT_H);
      glRotatef(laserAng,0,0,1);
      glLineWidth(3);
      glBegin(GL_LINES);
        glVertex3f(0,0,0);
        glVertex3f(0,1000,0);
      glEnd();
      glLineWidth(1);

      if(laserAng == 90) bLDir=false;
      if(laserAng == -90) bLDir=true;
      if(bLDir) laserAng++;
       else laserAng--;
    glPopMatrix();
  }
}

void GlSick::setScale(float s) {
  scale = s;
}

void GlSick::setup() {
  float w,h,d;

  if ( glIsList(oglList) ) {
    glDeleteLists( oglList, 1 );
  }

  oglList = glGenLists(1);
  glNewList( oglList, GL_COMPILE );
  glPushMatrix();

    glScalef(scale,scale,scale);

    // Demensions
    w = SICK_WIDTH  * 0.5;
    h = SICK_HEIGHT;
    d = SICK_DEEP;

    glColor3f  (0,0,1);
    glBegin (GL_TRIANGLE_STRIP);
      glVertex3f (-w, d, 10);
      glVertex3f ( w, d, 10);
      glVertex3f (-w, d,  0);
      glVertex3f ( w, d,  0);
      glVertex3f (-w, 0,  0);
      glVertex3f ( w, 0,  0);
      glVertex3f (-w, 0,  h);
      glVertex3f ( w, 0,  h);
      glVertex3f (-w, d,  h);
      glVertex3f ( w, d,  h);
      glVertex3f (-w, d,  h-70);
      glVertex3f ( w, d,  h-70);
    glEnd();
    glBegin (GL_TRIANGLE_STRIP);
      glVertex3f ( -w, d,  0);
      glVertex3f ( -w, 0,  0);
      glVertex3f ( -w, d, 10);
      glVertex3f ( -w, 0, 10);
    glEnd();
    glBegin (GL_TRIANGLE_STRIP);
      glVertex3f (  w, 0,  0);
      glVertex3f (  w, d,  0);
      glVertex3f (  w, 0, 10);
      glVertex3f (  w, d, 10);
    glEnd();
    glBegin (GL_TRIANGLE_STRIP);
      glVertex3f (  w,  0,  10);
      glVertex3f (  w, 70,  10);
      glVertex3f (  w,  0,  h-70);
      glVertex3f (  w, 70,  h-70);
      glVertex3f (  w,  0,  h);
      glVertex3f (  w,  d,  h-70);
      glVertex3f (  w,  d,  h);
    glEnd();
    glBegin (GL_TRIANGLE_STRIP);
      glVertex3f ( -w,  d,  h);
      glVertex3f ( -w,  d,  h-70);
      glVertex3f ( -w,  0,  h);
      glVertex3f ( -w, 70,  h-70);
      glVertex3f ( -w,  0,  h-70);
      glVertex3f ( -w, 70,  10);
      glVertex3f ( -w,  0,  10);
    glEnd();
    glColor3f(0,0.3f,0);
    glBegin (GL_TRIANGLE_STRIP);
      glVertex3f ( -w,  70,  10);
      glVertex3f ( -w,  70, h-70);
      glVertex3f ( -w+20,  100,  10);
      glVertex3f ( -w+20,  110, h-70);
      glVertex3f ( -w+40,  130,  10);
      glVertex3f ( -w+40,  140, h-70);
      glVertex3f (  0,  d-10,  10);
      glVertex3f (  0,  d, h-70);
      glVertex3f (  w-40,  130,  10);
      glVertex3f (  w-40,  140, h-70);
      glVertex3f (  w-20,  100,  10);
      glVertex3f (  w-20,  110, h-70);
      glVertex3f (  w,  70,  10);
      glVertex3f (  w,  70, h-70);
    glEnd();
  glPopMatrix();
  glEndList();
}

void GlSick::setConfig(int c) {
  conf = c;
}


