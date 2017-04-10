/*! \file globjects.cpp
 *   This is where the GLObjects class is implemented.
 */
#include "globjects.h"

//#include <GL/glut.h>

#if defined(Q_CC_MSVC)
#pragma warning(disable:4305) // init: truncation from const double to float
#endif

float colors[][3] ={
  {0.5f, 0.0f, 0.0f},
  {1.0f, 0.0f, 0.0f},
  {0.0f, 0.5f, 0.0f},
  {0.0f, 1.0f, 0.0f},
  {0.0f, 0.0f, 0.5f},
  {0.0f, 0.0f, 1.0f},
  {0.5f, 0.5f, 0.0f},
  {1.0f, 1.0f, 0.0f},
  {0.5f, 0.0f, 0.5f},
  {1.0f, 0.0f, 1.0f},
  {0.0f, 0.5f, 0.5f},
  {0.0f, 1.0f, 1.0f},
};

float colorsP[][3] ={
  {1.0f, 0.0f, 0.0f},
  {0.0f, 1.0f, 0.0f},
  {0.0f, 0.0f, 1.0f},
  {1.0f, 1.0f, 0.0f},
  {1.0f, 0.0f, 1.0f},
  {0.0f, 1.0f, 1.0f},
};


GLObjects::GLObjects(){
  conf = 0;
  numLandmarks = 0;
  generalQuadric = gluNewQuadric();
  posX = posY = posZ = posTh = 0.0f;
  //for(short i=0; i<MAX_LANDMARKS_NUM; i++) {
    //Math3d::SetVec(vLandmarks[i], 0.0f,0.0f,0.0f);
  //}
  rgbdI = 0;
  yMax= 0.0f;
  yMin= 0.0f;
  zMin= -10000.0f;
}

GLObjects::~GLObjects(){
  glDeleteLists( objAxes, 1 );
  glDeleteLists( objGrid, 1 );
  gluDeleteQuadric(generalQuadric);
}

void GLObjects::drawV3D() {
  glColor3f(1.0f,1.0f,1.0f);
  glPointSize(3);

  glBegin(GL_POINTS);
    for(Vec3dList::iterator it =vV3D.begin(); it != vV3D.end(); it++){
      glVertex3d( it->x, it->y, it->z );
    }
  glEnd();
  glPointSize(1);
  /**
  glLineWidth( 2.0f );
  glBegin(GL_LINE_LOOP);
  //glBegin(GL_POINTS);
    for(Vec3dList::iterator it =vV3D.begin(); it != vV3D.end(); it++){
      glVertex3d( it->x, it->y, it->z );
    }
  glEnd();
  **/
}

void GLObjects::drawLaserPoints() {
  glColor3f(0.0f,0.0f,0.0f);
  glPointSize(3);
  glBegin(GL_POINTS);
    for(Vec3dList::iterator it =vActualLaserPoints.begin(); it != vActualLaserPoints.end(); it++){
      glVertex3d( it->x, it->y, it->z );
    }
  glEnd();
  glPointSize(1);

  glColor3f(0.9f,0.9f,0.0f);
  //glLineWidth( 2.0f );
  glBegin(GL_LINE_STRIP);
    for(Vec3dList::iterator it =vActualLaserPoints.begin(); it != vActualLaserPoints.end(); it++){
      glVertex3d( it->x, it->y, it->z );
    }
  glEnd();

  glBegin(GL_POINTS);
  glColor3f(0.0f, 0.5f, 0.5f);
    for(Vec3dList::iterator it =vAllLaserPoints.begin(); it != vAllLaserPoints.end(); it++){
      glVertex3d( it->x, it->y, it->z );
    }
  glEnd();

}

void GLObjects::drawCameraPoints() {
  Vec3dList::iterator it;
  short pointPrint=0, color=0, color2=0;

  glColor3f(1.0f,0.0f,0.0f);
  glPointSize(3);
  glBegin(GL_POINTS);
    for(it =vCameraPoints.begin(); it != vCameraPoints.end(); it++){
      glColor3f(colorsP[color2][0],colorsP[color2][1],colorsP[color2][2]);
      color2++;
      if (color2>3) color2=0;
      // To visualice the 3D points in OpenGL acording to the robot
      // we interchange the coordinates (x,y,z) -> (x,z,-y)
      glVertex3d( it->x, it->z, -it->y );
    }
  glEnd();
  glPointSize(1);
  color=0;
  //glLineWidth( 2.0f );
  //glBegin(GL_LINE_LOOP);
  //glBegin(GL_POINTS);
    //for(Vec3dList::iterator it =vCamera.begin(); it != vCamera.end(); it++){
      //glVertex3d( it->x, it->y, it->z );
    //}
  //glEnd();

  glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
  glBegin(GL_QUADS);
  for(it =vCameraPoints.begin(); it != vCameraPoints.end(); it++){
     if (!(pointPrint % 4))
     {
       glColor4f(colors[color][0],colors[color][1],colors[color][2],0.5f);
       color++;
     }
    // To visualice the 3D points in OpenGL acording to the robot
    // we interchange the coordinates (x,y,z) -> (x,z,-y)
    glVertex3d( it->x, it->z, -it->y );
    if (color == 12) color = 0;
    pointPrint++;
  }
  glEnd();
  glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
}

void GLObjects::drawLandmarks() {
  float r=0.0f, g=0.0f, b=0.5f;
  for(unsigned short i=0; i<numLandmarks; i++) {
    if ( vLandmarks[i].empty() ) continue;
    for (std::vector<CvPoint3D64f>::iterator it = vLandmarks[i].begin();
	 it != vLandmarks[i].end(); it++)
    {
      glColor3f(r,g,b);
      glPushMatrix();
        // To visualice the 3D points in OpenGL acording to the robot
        // we interchange the coordinates (x,y,z) -> (x,z,-y)
        glTranslated( it->x, it->z, -it->y );
        gluSphere(generalQuadric, 100, 12, 8);
      glPopMatrix();
      b+=0.5f;
      if (b==1.5f) { b=0.0f; g+=0.5f; }
      if (g==1.5f) { g=0.0f; r+=0.5f; }
      if (r==1.5f) { r=0.0f; }
    } 
  } 
}


void GLObjects::draw(){
  //TODO uncoment
  //glCallList( objAxes );
  glCallList( objGrid );

  /*
  glColor3f(1.0f,0.0f,0.0f);
  glutSolidCone( 1.0,//GLdouble base
              2.0,// GLdouble height
              10,// GLint slices
              5// GLint stacks
              );
  */
  
  glPushMatrix();
    glScalef(scale,scale,scale);
    if (conf & GL_VIEW_POINTS)  drawV3D();
    if (conf & GL_VIEW_LASER_P) {
      //glPushMatrix();
        //glTranslatef(posX,posY,posZ);
        //glRotatef(posTh,0,0,1);
        drawLaserPoints();
      //glPopMatrix();
    }

    if (conf & GL_VIEW_LANDMARKS)  drawLandmarks();

    glTranslated(posX-20, posY+340, posZ+670);
    glRotated(posTh,0,0,1);
    if (conf & GL_VIEW_CAMERA_P)  drawCameraPoints();
    
  glPopMatrix();

  //glAmros.draw();
  
  /** TODO geometer  ***
  glPushMatrix();
    glScalef(scale,scale,scale);
    glRotatef(-90.0f, 1.0f,0.0f,0.0f);
    glGeometer.dibuja();
  glPopMatrix();
  ***/

  if (conf & _GL_KINECT) {
    glPushMatrix();
      glScalef(scale,scale,scale);
      glRotatef(90.0f, 1.0f,0.0f,0.0f);
      //glTranslatef(0.0f, 0.0f, yMin-yMax);
      //  y = altura de kinect 80cm aprox
      //  z = 30cm distancia minima
      glTranslatef(0.0f, 800.0f, 1000 );
      //dKinect();
      dKinect1();
    glPopMatrix();
  }

}

void GLObjects::addV3D(Vec3d point) {
  vV3D.push_back( point );
}

void GLObjects::setupAxes() {
  // Generating the Axes X,Y,Z
  objAxes = glGenLists( 1 );
  glNewList( objAxes, GL_COMPILE );
   glLineWidth( 2.0f );
   glBegin(GL_LINES);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(2000.0f, 0.0f, 0.0f);

    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 2000.0f, 0.0f);

    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 2000.0f);
   glEnd();
   glLineWidth( 1.0f );
  glEndList();
}

void GLObjects::setupGrid() {
  // Generating the Axes X,Y,Z
  objGrid = glGenLists( 1 );
  glNewList( objGrid, GL_COMPILE );
   glBegin(GL_LINES);
    glColor3f(0.9f, 0.9f, 0.9f);
    for(float line=0; line<40.0f; line+=1.0f) {
      glVertex3f(-400.0f,  line*10.0f, 0.0f);
      glVertex3f( 400.0f,  line*10.0f, 0.0f);
      glVertex3f(-400.0f, -line*10.0f, 0.0f);
      glVertex3f( 400.0f, -line*10.0f, 0.0f);

      glVertex3f( line*10.0f, -400.0f, 0.0f);
      glVertex3f( line*10.0f,  400.0f, 0.0f);
      glVertex3f(-line*10.0f, -400.0f, 0.0f);
      glVertex3f(-line*10.0f,  400.0f, 0.0f);
    }
   glEnd();
  glEndList();
}

void GLObjects::setup(float fScale) {
  scale = fScale;
  glAmros.setScale(fScale);
  setupAxes();
  setupGrid();
  glAmros.setup();
}

void GLObjects::setConfig(unsigned int c){
  if (conf & c) conf &= ~c;
  else conf |= c;
}

void GLObjects::moveTo(double nX, double nY, double nZ, double nTh){
  posX  = nX;
  posY  = nY;
  posZ  = nZ;
  posTh = nTh;
}

void GLObjects::moveRel(double nX, double nY, double nZ, double nTh){
  posX  += nX;
  posY  += nY;
  posZ  += nZ;
  posTh += nTh;

  while (posTh<0)   posTh+=360;
  while (posTh>360) posTh-=360;
}

void GLObjects::dKinect()
{
  const uint8_t* crgb;
  uint8_t* rgb;
  GLbyte* color;
  GLbyte vColor[3];
  if (rgbdI) {
    crgb = rgbdI->bufferRGB();
	rgb = const_cast<uint8_t*>(crgb);
    color = reinterpret_cast<GLbyte*>(rgb);
  }
  glPointSize(3);
  glBegin(GL_POINTS);
   //glColor3f(1.0f, 0.0f, 0.0f);
   for (std::vector<CvPoint3D32f>::iterator it = v3Dkp.begin();
	 it != v3Dkp.end(); it++)
   {
     // To visualice the 3D points in OpenGL acording to the robot
     // we interchange the coordinates (x,y,z) -> (x,-z,y)
     if (it->z == 0) {
       color+=3;
       continue;
     }
     vColor[0] = *color++;
     vColor[1] = *color++;
     vColor[2] = *color++;

     if (vColor[0] < 0.01f ||
         vColor[1] < 0.01f ||
         vColor[2] < 0.01f) {
       continue;
     } 

     glColor3bv(vColor);
     //glVertex3d( it->x, -it->z, it->y );
     glVertex3d( it->x, it->y, it->z );

     if (zMin < it->z) zMin = it->z;

     if (yMin < it->y) yMin = it->y;
     if (yMax > it->y) yMax = it->y;
     
   }
  glEnd();
  glPointSize(1);
}

void GLObjects::dKinect1()
{
  const uint8_t* crgb;
  uint8_t* rgb;
  GLbyte* color;
  GLbyte vColor[3];
  if (rgbdI) {
    crgb = rgbdI->bufferRGB();
	rgb = const_cast<uint8_t*>(crgb);
    color = reinterpret_cast<GLbyte*>(rgb);
  }
  //glPointSize(3);
  glPointSize(1);
  glBegin(GL_POINTS);
   //glColor3f(1.0f, 0.0f, 0.0f);
   for (std::vector<CvPoint3D32f>::iterator it = v3Dkp.begin();
	 it != v3Dkp.end(); it++)
   {
     // To visualice the 3D points in OpenGL acording to the robot
     // we interchange the coordinates (x,y,z) -> (x,-z,y)
     if (it->z == 0) {
       color+=3;
       continue;
     }
     vColor[0] = *color++;
     vColor[1] = *color++;
     vColor[2] = *color++;

     /*
     if (vColor[0] < 0.01f ||
         vColor[1] < 0.01f ||
         vColor[2] < 0.01f) {
       continue;
     } 
     */

     glColor3bv(vColor);
     //glColor3f(0.0f, 0.0f, 0.0f);
     //glVertex3d( it->x, -it->z, it->y );
     glVertex3d( it->x, it->y, it->z );
     //glVertex3d( it->x, 0.0f, it->z );

     /*
     if (zMin < it->z) zMin = it->z;
     if (yMin < it->y) yMin = it->y;
     if (yMax > it->y) yMax = it->y;
     */
     
   }
  glEnd();
  glPointSize(1);
}

void GLObjects::getCloud(short version)
{
   if (rgbdI) {
     rgbdI->getCloud3DPoints(v3Dkp, version);
   }
}

