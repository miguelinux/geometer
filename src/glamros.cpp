#include "glamros.h"


GlAmros::GlAmros(){

  setScale(1.0f);

  posX=0;
  posY=0;
  posZ=0;
  posTh=0;

}

GlAmros::GlAmros(float scale){

  setScale(scale);

  posX=0;
  posY=0;
  posZ=0;
  posTh=0;

}

void GlAmros::setScale(float s){
  scale = s;
  //sick.setScale(1.0f);
  //pbot.setScale(1.0f);
}

GlAmros::~GlAmros(){
}

void GlAmros::moveTo(float nX, float nY, float nZ, float nTh){
  posX  = nX;
  posY  = nY;
  posZ  = nZ;
  posTh = nTh;
}

void GlAmros::moveRel(float nX, float nY, float nZ, float nTh){
  posX  += nX;
  posY  += nY;
  posZ  += nZ;
  posTh += nTh;

  while (posTh<0)   posTh+=360;
  while (posTh>360) posTh-=360;
}

float GlAmros::getX(){
  return (posX);
}

float GlAmros::getY(){
  return (posY);
}

float GlAmros::getZ(){
  return (posZ);
}

float GlAmros::getTh(){
  return (posTh);
}

void GlAmros::draw(){

  glPolygonMode(GL_BACK,GL_LINE);

  glPushMatrix();
    glTranslatef(posX,posY,posZ);
    glRotatef(posTh,0,0,1);

    pbot.draw();

    glTranslatef( 0,  150, 476);

    sick.setConfig(GL_SICK_LASER_ON);
    sick.draw();

  glPopMatrix();
}

void GlAmros::setup(){
  sick.setup();
  pbot.setup();
}

