#include "glpowerbot.h"
#include <cmath>

#if defined(Q_CC_MSVC)
#pragma warning(disable:4305) // init: truncation from const double to float
#endif

GlPowerBot::GlPowerBot(){
  conf=0;
  laserAng=0;
  scale = 1.0f;
}

GlPowerBot::~GlPowerBot(){
  glDeleteLists( oglList, 1 );
}

void GlPowerBot::draw() {
  glCallList( oglList );
}

void GlPowerBot::setScale(float s) {
  scale = s;
}

void GlPowerBot::setup() {

  float x,y;
  float angle;

  if (glIsList(oglList)) {
    glDeleteLists( oglList, 1 );
  } 

  oglList = glGenLists(1);
  glNewList( oglList, GL_COMPILE );

  glPushMatrix();

    glScalef(scale,scale,scale);

    /*glVertex3f ( w, d, h);
    glBegin (GL_TRIANGLE_STRIP);
      glVertex3f ( , , );
    glEnd();
    ******************************/
    glColor3f  (1.0f,0.9f,0.0f);
    glBegin (GL_TRIANGLE_STRIP);
      glVertex3f (-267,  158, 476);
      glVertex3f ( 267,  158, 476);
      glVertex3f (-267,  158,  74);
      glVertex3f ( 267,  158,  74);
    glEnd();
    glBegin (GL_TRIANGLE_STRIP);
      glVertex3f ( 267,  158, 476);
      glVertex3f ( 267, -330, 476);
      glVertex3f ( 267,  158, 282);
      glVertex3f ( 267, -330, 282);
    glEnd();
    glBegin (GL_TRIANGLE_STRIP);
      glVertex3f ( 267,  158, 476);
      glVertex3f ( 267, -330, 476);
      glVertex3f ( 267,  158, 282);
      glVertex3f ( 267, -330, 282);
    glEnd();
    glBegin (GL_TRIANGLE_STRIP);
      glVertex3f ( 267, 142,  74);
      glVertex3f ( 267, 158,  74);
      glVertex3f ( 267, 142, 146);
      glVertex3f ( 267, 158, 146);
      glVertex3f ( 267, 134, 176);
      glVertex3f ( 267, 158, 282);
      glVertex3f ( 267, 122, 212);
      glVertex3f ( 267,  92, 282);
      glVertex3f ( 267,  92, 246);
      glVertex3f ( 267,  56, 282);
      glVertex3f ( 267,  56, 270);
      glVertex3f ( 267,   0, 282);
    glEnd();
    glBegin (GL_TRIANGLE_STRIP);
      glVertex3f ( 267,-330, 282);
      glVertex3f ( 267,-330, 218);
      glVertex3f ( 267,-300, 282);
      glVertex3f ( 267,-300, 218);
      glVertex3f ( 267,-260, 282);
      glVertex3f ( 267,-260, 210);
      glVertex3f ( 267,-228, 282);
      glVertex3f ( 267,-228, 188);
      glVertex3f ( 267,-202, 282);
      glVertex3f ( 267,-202, 160);
      glVertex3f ( 267,-176, 282);
      glVertex3f ( 267,-176, 106);
      glVertex3f ( 267,-162, 282);
      glVertex3f ( 267,-162, 98);
      glVertex3f ( 267,-154, 282);
      glVertex3f ( 267,-154, 106);
      glVertex3f ( 267,-126, 282);
      glVertex3f ( 267,-126, 202);
      glVertex3f ( 267,-102, 282);
      glVertex3f ( 267,-102, 236);
      glVertex3f ( 267, -52, 282);
      glVertex3f ( 267, -52, 274);
      glVertex3f ( 267,   0, 282);
    glEnd();
    glBegin (GL_TRIANGLE_STRIP);
      glVertex3f ( 267,-330, 476);
      glVertex3f ( 125,-441, 476);
      glVertex3f ( 267,-330, 218);
      glVertex3f ( 125,-441, 218);
    glEnd();
    glBegin (GL_TRIANGLE_STRIP);
      glVertex3f ( 228.6f,-360, 218);
      glVertex3f ( 125,-441, 218);
      glVertex3f ( 190.2f,-390, 196);
      glVertex3f ( 125,-441, 196);
      glVertex3f ( 164.6f,-410, 158);
      glVertex3f ( 125,-441, 158);
      glVertex3f ( 151.8f,-420,  94);
      glVertex3f ( 125,-441,  94);
      glVertex3f ( 151.8f,-420,  74);
      glVertex3f ( 125,-441,  74);
    glEnd();
    glBegin (GL_TRIANGLE_STRIP);
      glVertex3f ( 125,-441, 476);
      glVertex3f (-125,-441, 476);
      glVertex3f ( 125,-441,  74);
      glVertex3f (-125,-441,  74);
    glEnd();
    glBegin (GL_TRIANGLE_STRIP);
      glVertex3f (-267, -330, 476);
      glVertex3f (-267,  158, 476);
      glVertex3f (-267, -330, 282);
      glVertex3f (-267,  158, 282);
    glEnd();
    glBegin (GL_TRIANGLE_STRIP);
      glVertex3f (-267, 158, 74);
      glVertex3f (-267, 142, 74);
      glVertex3f (-267, 158, 146);
      glVertex3f (-267, 142, 146);
      glVertex3f (-267, 158, 176);
      glVertex3f (-267, 134, 176);
      glVertex3f (-267, 158, 282);
      glVertex3f (-267, 122, 212);
      glVertex3f (-267,  92, 282);
      glVertex3f (-267,  92, 246);
      glVertex3f (-267,  56, 282);
      glVertex3f (-267,  56, 270);
      glVertex3f (-267,   0, 282);
    glEnd();
    glBegin (GL_TRIANGLE_STRIP);
      glVertex3f (-267,-330, 218);
      glVertex3f (-267,-330, 282);
      glVertex3f (-267,-300, 218);
      glVertex3f (-267,-300, 282);
      glVertex3f (-267,-260, 210);
      glVertex3f (-267,-260, 282);
      glVertex3f (-267,-228, 188);
      glVertex3f (-267,-228, 282);
      glVertex3f (-267,-202, 160);
      glVertex3f (-267,-202, 282);
      glVertex3f (-267,-176, 106);
      glVertex3f (-267,-176, 282);
      glVertex3f (-267,-162, 98);
      glVertex3f (-267,-162, 282);
      glVertex3f (-267,-154, 106);
      glVertex3f (-267,-154, 282);
      glVertex3f (-267,-126, 202);
      glVertex3f (-267,-126, 282);
      glVertex3f (-267,-102, 236);
      glVertex3f (-267,-102, 282);
      glVertex3f (-267, -52, 274);
      glVertex3f (-267, -52, 282);
      glVertex3f (-267,   0, 282);
    glEnd();
    glBegin (GL_TRIANGLE_STRIP);
      glVertex3f (-125,-441, 476);
      glVertex3f (-267,-330, 476);
      glVertex3f (-125,-441, 218);
      glVertex3f (-267,-330, 218);
    glEnd();
    glBegin (GL_TRIANGLE_STRIP);
      glVertex3f (-125,-441, 218);
      glVertex3f (-228.6f,-360, 218);
      glVertex3f (-125,-441, 196);
      glVertex3f (-190.2f,-390, 196);
      glVertex3f (-125,-441, 158);
      glVertex3f (-164.6f,-410, 158);
      glVertex3f (-125,-441,  94);
      glVertex3f (-151.8f,-420,  94);
      glVertex3f (-125,-441,  74);
      glVertex3f (-151.8f,-420,  74);
    glEnd();

    glColor3f  (0.0f,0.0f,0.0f);
    glBegin (GL_TRIANGLE_STRIP);
      glVertex3f ( 316, -71, 476);
      glVertex3f (-316, -71, 476);
      glVertex3f ( 316,-134, 476);
      glVertex3f (-316,-134, 476);
      glVertex3f ( 288,-335, 476);
      glVertex3f (-288,-335, 476);
      glVertex3f ( 235,-398, 476);
      glVertex3f (-235,-398, 476);
      glVertex3f ( 168,-443, 476);
      glVertex3f (-168,-443, 476);
      glVertex3f (  44,-477, 476);
      glVertex3f ( -44,-477, 476);
    glEnd();
    glBegin (GL_TRIANGLE_STRIP);
      glVertex3f (-316, -66, 476);
      glVertex3f ( 316, -66, 476);
      glVertex3f (-316, 159, 476);
      glVertex3f ( 316, 159, 476);
      glVertex3f (-280, 218, 476);
      glVertex3f ( 280, 218, 476);
      glVertex3f (-242, 258, 476);
      glVertex3f ( 242, 258, 476);
      glVertex3f (-194, 293, 476);
      glVertex3f ( 194, 293, 476);
      glVertex3f (-131, 323, 476);
      glVertex3f ( 131, 323, 476);
      glVertex3f ( -50, 342, 476);
      glVertex3f (  50, 342, 476);
    glEnd();
    
    // Big right wheel 
    glBegin (GL_TRIANGLE_FAN);
      glVertex3f ( 315, 0.0f, 130.0f);
      for (angle=0.0f; angle < 6.2832f; angle += 0.62832f) {
	x=std::cos(angle) * 130.0f;
	y=std::sin(angle) * 130.0f;
        glVertex3f ( 315, x+0.0f, y+130.0f);
      }
    glEnd();
    glBegin (GL_TRIANGLE_STRIP);
      for (angle=0.0f; angle < 6.2832f; angle += 0.62832f) {
	x=std::cos(angle) * 130.0f;
	y=std::sin(angle) * 130.0f;
        glVertex3f ( 315, x+0.0f, y+130.0f);
        glVertex3f ( 265, x+0.0f, y+130.0f);
      }
    glEnd();
    // Small right wheel 
    glBegin (GL_TRIANGLE_FAN);
      glVertex3f ( 230,-330.0f, 80.0f);
      for (angle=0.0f; angle < 6.2832f; angle += 0.62832f) {
	x=std::cos(angle) * 80.0f;
	y=std::sin(angle) * 80.0f;
        glVertex3f ( 230, x-330.0f, y+80.0f);
      }
    glEnd();
    glBegin (GL_TRIANGLE_STRIP);
      for (angle=0.0f; angle < 6.2832f; angle += 0.62832f) {
	x=std::cos(angle) * 80.0f;
	y=std::sin(angle) * 80.0f;
        glVertex3f ( 230, x-330.0f, y+80.0f);
        glVertex3f ( 200, x-330.0f, y+80.0f);
      }
    glEnd();
    // Big left wheel 
    glBegin (GL_TRIANGLE_FAN);
      glVertex3f (-315, 0.0f, 130.0f);
      for (angle=0.0f; angle >-6.2832f; angle -= 0.62832f) {
	x=std::cos(angle) * 130.0f;
	y=std::sin(angle) * 130.0f;
        glVertex3f (-315, x+0.0f, y+130.0f);
      }
    glEnd();
    glBegin (GL_TRIANGLE_STRIP);
      for (angle=0.0f; angle >-6.2832f; angle -= 0.62832f) {
	x=std::cos(angle) * 130.0f;
	y=std::sin(angle) * 130.0f;
        glVertex3f (-315, x+0.0f, y+130.0f);
        glVertex3f (-265, x+0.0f, y+130.0f);
      }
    glEnd();
    // Small left wheel 
    glBegin (GL_TRIANGLE_FAN);
      glVertex3f (-230,-330.0f, 80.0f);
      for (angle=0.0f; angle >-6.2832f; angle -= 0.62832f) {
	x=std::cos(angle) * 80.0f;
	y=std::sin(angle) * 80.0f;
        glVertex3f (-230, x-330.0f, y+80.0f);
      }
    glEnd();
    glBegin (GL_TRIANGLE_STRIP);
      for (angle=0.0f; angle >-6.2832f; angle -= 0.62832f) {
	x=std::cos(angle) * 80.0f;
	y=std::sin(angle) * 80.0f;
        glVertex3f (-230, x-330.0f, y+80.0f);
        glVertex3f (-200, x-330.0f, y+80.0f);
      }
    glEnd();

    glColor3f  (0.5f,0.5f,0.5f);
    glBegin (GL_TRIANGLE_STRIP);
      glVertex3f ( 240,-258, 204);
      glVertex3f ( 240,-310, 204);
      glVertex3f ( 240,-240, 182);
      glVertex3f ( 240,-310, 182);
      glVertex3f ( 240,-240, 138);
      glVertex3f ( 240,-310, 138);
      glVertex3f ( 240,-308,  74);
      glVertex3f ( 240,-340,  78);
      glVertex3f ( 240,-328,  62);
    glEnd();
    glBegin (GL_TRIANGLE_STRIP);
      glVertex3f (-240,-310, 204);
      glVertex3f (-240,-258, 204);
      glVertex3f (-240,-310, 182);
      glVertex3f (-240,-240, 182);
      glVertex3f (-240,-310, 138);
      glVertex3f (-240,-240, 138);
      glVertex3f (-240,-340,  78);
      glVertex3f (-240,-308,  74);
      glVertex3f (-240,-328,  62);
    glEnd();

    /*glVertex3f ( w, d, h);
    glBegin (GL_TRIANGLE_STRIP);
      glVertex3f ( , , );
    glEnd();
    ******************************/


  glPopMatrix();
  glEndList();
}

void GlPowerBot::setConfig(int c) {
  conf = c;
}


