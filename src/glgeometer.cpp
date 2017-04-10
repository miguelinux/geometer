#include "glgeometer.h"

GlGeometer::GlGeometer(){
  posX=0;
  posY=0;
  posZ=0;
  posTh=0;

  panTh=0;
  tiltTh=0;

  bLaserOn = false;
  laserAng = -90;
  bLDir = true;

  bSonarOn = false;
  sonarNum = 1;
  bSDir = true;

  GeneralQuadric = gluNewQuadric();
  bLineas=false;
  bSolido=false;
  bPiso=false;
}

GlGeometer::~GlGeometer(){
  gluDeleteQuadric(GeneralQuadric);
}

void GlGeometer::cubo(float lX, float lY, float lZ){
    float lx,ly,lz;

    lx = lX * 0.5;
    ly = lY * 0.5;
    lz = lZ * 0.5;

    // Draw the sides
    glBegin (GL_TRIANGLE_STRIP);
    glNormal3f (-1,0,0);
    glVertex3f (-lx,-ly,-lz);
    glVertex3f (-lx,-ly,lz);
    glVertex3f (-lx,ly,-lz);
    glVertex3f (-lx,ly,lz);
    glNormal3f (0,1,0);
    glVertex3f (lx,ly,-lz);
    glVertex3f (lx,ly,lz);
    glNormal3f (1,0,0);
    glVertex3f (lx,-ly,-lz);
    glVertex3f (lx,-ly,lz);
    glNormal3f (0,-1,0);
    glVertex3f (-lx,-ly,-lz);
    glVertex3f (-lx,-ly,lz);
    glEnd();

    // Draw the top
    glBegin (GL_TRIANGLE_FAN);
    glNormal3f (0,0,1);
    glVertex3f (-lx,-ly,lz);
    glVertex3f (lx,-ly,lz);
    glVertex3f (lx,ly,lz);
    glVertex3f (-lx,ly,lz);
    glEnd();

    // Draw the bottom
    glBegin (GL_TRIANGLE_FAN);
    glNormal3f (0,0,-1);
    glVertex3f (-lx,-ly,-lz);
    glVertex3f (-lx,ly,-lz);
    glVertex3f (lx,ly,-lz);
    glVertex3f (lx,-ly,-lz);
    glEnd();
}

void GlGeometer::cuadro(float lX, float lY){
    float lx,ly;

    lx = lX * 0.5;
    ly = lY * 0.5;

    glBegin (GL_TRIANGLE_FAN);
     //glNormal3f (0,0,1);
     glVertex3f (lx,-ly,0);
     glVertex3f (lx,ly,0);
     glVertex3f (-lx,ly,0);
     glVertex3f (-lx,-ly,0);
    glEnd();
}

void GlGeometer::glTpt(float trX,float trY,float trZ){
  glTranslatef(trX - 20 + 42.5, trY - PIONEER_ALTURA - 10 - 180, trZ + 190 - 55);
}

void GlGeometer::glTl(float lX,float lY,float lZ){
  glTranslatef(lX - 20, lY - PIONEER_ALTURA - 10, lZ + 190 );
}

void GlGeometer::glTo(float oX,float oY,float oZ){
  glTranslatef(oX - 57.5, oY - PIONEER_ALTURA, oZ - 175);
}

void GlGeometer::dibuja(){
  glPushMatrix();
    glTranslatef(posX,posY,posZ);
    glRotatef(posTh,0,1,0);
    if (!bSolido) glPolygonMode(GL_BACK,GL_LINE);
    if (bLineas)  glPolygonMode(GL_FRONT,GL_LINE);

    glRotatef(GEOMETER_INCLINACION,1,0,0);

    dPioneer();
    dLaser();
    dPantTilt();
    dOmni();

    glRotatef(-GEOMETER_INCLINACION,1,0,0);

    if(bPiso){
      glRotatef(90,1,0,0);
      glColor3f(0.75,0.75,0.75);
      cuadro(10000,10000);
    }

    if (bLineas) glPolygonMode(GL_FRONT,GL_FILL);
    glPolygonMode(GL_BACK,GL_FILL);
  glPopMatrix();
}

/*** PIONEER ***/

void GlGeometer::dPioneer(){
  float lx, ly, lz;

  lx= 135;
  ly=  85;
  lz= 175;

  // Cuerpo
  glPushMatrix();
   glTranslatef(0,-152,0);
   glColor3f(0.5,0,0);
   glBegin (GL_TRIANGLE_STRIP);
    glVertex3f(80,-ly,lz);
   //glColor3f(0.875,0,0);
    glVertex3f(80,ly,lz);
   //glColor3f(0.5,0,0);
    glVertex3f(-80,-ly,lz);
   //glColor3f(0.875,0,0);
    glVertex3f(-80,ly,lz);
   //glColor3f(0.5,0,0);
    glVertex3f(-lx,-ly,140);
   //glColor3f(0.875,0,0);
    glVertex3f(-lx,ly,140);
   //glColor3f(0.5,0,0);
    glVertex3f(-lx,-ly,-140);
   //glColor3f(0.875,0,0);
    glVertex3f(-lx,ly,-140);
   //glColor3f(0.5,0,0);
    glVertex3f(-102.5,-ly,-lz);
   //glColor3f(0.875,0,0);
    glVertex3f(-102.5,ly,-lz);
   //glColor3f(0.5,0,0);
    glVertex3f(102.5,-ly,-lz);
   //glColor3f(0.875,0,0);
    glVertex3f(102.5,ly,-lz);
   //glColor3f(0.5,0,0);
    glVertex3f(lx,-ly,-140);
   //glColor3f(0.875,0,0);
    glVertex3f(lx,ly,-140);
   //glColor3f(0.5,0,0);
    glVertex3f(lx,-ly,140);
   //glColor3f(0.875,0,0);
    glVertex3f(lx,ly,140);
   //glColor3f(0.5,0,0);
    glVertex3f(80,-ly,lz);
   //glColor3f(0.875,0,0);
    glVertex3f(80,ly,lz);
   glEnd();
  glPopMatrix();

  
  // Sonares
  glPushMatrix();
   glTranslatef(0,-152,0);
   lz+=26;
   glTranslatef(0,-ly+25,0);
   glRotatef(-45+7.5,0,1,0);
   glColor3f(0.875,0.875,0);

   for (lx=2;lx<8;lx++){
     glTranslatef(0,0,lz+1);
     gluDisk(GeneralQuadric,0,17.5,20,1);
     glTranslatef(0,0,-1);
     //glColor3f(0.875,0,0);
     glColor3f(0.5f,0,0);
     cuadro(53,50);
     glColor3f(0.875,0.875,0);

     if(bSonarOn && (sonarNum == lx)){
       glLineWidth(3);
       glBegin(GL_LINES);
         glVertex3f(0,0,0);
         glVertex3f(0,0,800);
       glEnd();
       glLineWidth(1);
     }

     glTranslatef(0,0,-lz);
     glRotatef(15,0,1,0);
   }

   glRotatef(45-7.5,0,1,0);
   glTranslatef(-117,0,144.5);
   gluDisk(GeneralQuadric,0,17.5,20,1);
   glTranslatef(0,0,-1);
   //glColor3f(0.875,0,0);
   glColor3f(0.5f,0,0);
   cuadro(53,50);
   glColor3f(0.875,0.875,0);
   if(bSonarOn && (sonarNum == 8)){
     glBegin(GL_LINES);
       glVertex3f(0,0,0);
       glVertex3f(0,0,800);
     glEnd();
   }

   glRotatef(180,0,1,0);
   glTranslatef(0,0,287.75);
   gluDisk(GeneralQuadric,0,17.5,20,1);
   glTranslatef(0,0,-1);
   //glColor3f(0.875,0,0);
   glColor3f(0.5f,0,0);
   cuadro(53,50);
   glColor3f(0.875,0.875,0);
   if(bSonarOn && (sonarNum == 1)){
     glBegin(GL_LINES);
       glVertex3f(0,0,0);
       glVertex3f(0,0,800);
     glEnd();
   }
   if(bSonarOn){ 
     if(sonarNum == 8) bSDir=false;
     if(sonarNum == 1) bSDir=true;
     if(bSDir) sonarNum++;
       else sonarNum--;
   }

  glPopMatrix();

  lx= 135;
  ly=  85;
  lz= 175;

  // Ruedas
  glPushMatrix();
   glTranslatef(0,-95,40);
   // Rueda Derecha
   glRotatef(90,0,1,0);
   glTranslatef(0,0,lx+15);
   glColor3f(0,0,0);
   gluCylinder(GeneralQuadric,95,95,50,20,2);
   glTranslatef(0,0,50);
   gluDisk(GeneralQuadric,50,95,20,1);
   glColor3f(0.75,0.75,0.25);
   gluDisk(GeneralQuadric,0,50,20,1);

   // Rueda Izquierda
   glRotatef(180,0,1,0);
   glTranslatef(0,0,lx+lx+80);
   glColor3f(0,0,0);
   gluCylinder(GeneralQuadric,95,95,50,20,2);
   glTranslatef(0,0,50);
   gluDisk(GeneralQuadric,50,95,20,1);
   glColor3f(0.75,0.75,0.25);
   gluDisk(GeneralQuadric,0,50,20,1);

   // Rueda tracera
   glColor3f(0,0,0);
   glTranslatef(-lz-5,65,-lx-50);
   gluDisk(GeneralQuadric,0,35,20,1);
   glTranslatef(0,0,-30);
   gluCylinder(GeneralQuadric,35,35,30,20,2);
   glRotatef(180,0,1,0);
   gluDisk(GeneralQuadric,0,35,20,1);
  glPopMatrix();

  // Base negra
  glPushMatrix();
   glTranslatef(0,-152,0);
   glTranslatef(0,-ly-3,5);
   glRotatef(90,1,0,0);
   //cuadro(400,440);
   gluPartialDisk(GeneralQuadric,0,220,10,1,129,102);
   glTranslatef(0,45,0);
   cuadro(380,170);
   glTranslatef(0,-75,0);
   gluPartialDisk(GeneralQuadric,0,250,10,1,-50,100);
   glTranslatef(0,-58,0);
   cuadro(340,100);
  glPopMatrix();

}

void GlGeometer::sonarOn(bool bSON){
  bSonarOn = bSON;
}

void GlGeometer::moveTo(float nX, float nY, float nZ, float nTh){
  posX  = nX;
  posY  = nY;
  posZ  = nZ;
  posTh = nTh;
}

void GlGeometer::moveRel(float nX, float nY, float nZ, float nTh){
  posX  += nX;
  posY  += nY;
  posZ  += nZ;
  posTh += nTh;

  while (posTh<0)   posTh+=360;
  while (posTh>360) posTh-=360;
}

float GlGeometer::getX(){
  return (posX);
}

float GlGeometer::getY(){
  return (posY);
}

float GlGeometer::getZ(){
  return (posZ);
}

float GlGeometer::getTh(){
  return (posTh);
}

/*** PAN TILT ***/

void GlGeometer::dPantTilt(){
  // Motor Pan
  glPushMatrix();
   glTpt(35,-22.5,-45);
   glColor3f(0,0,0);
   cubo(70,45,90);
  glPopMatrix();
  // Motor Tilt
  glPushMatrix();
   glTpt(45,-77.5,-22.5);
   glRotatef(panTh,0,1,0);
   glTranslatef(0,0,-22.5);
   glColor3f(0.125,0.125,0.125);
   cubo(40,65,90);
  glPopMatrix();
  // Cabezal
  glPushMatrix();
   glTpt(45,-87.5,-22.5);
   glRotatef(panTh,0,1,0);
   glRotatef(tiltTh,1,0,0);
   glTranslatef(-25,-12.5,0);
   glColor3f(0,0,0);
   cubo(10,50,25);  // Cuello Izq
   glTranslatef(50,0,0);
   cubo(10,50,25);  // Cuello Der
   glTranslatef(-25,-47.5,0);
   glColor3f(0.125,0.125,0.125);
   cubo(130,45,27); // Cabeza
   glTranslatef(0,-20,-23.5);
   glColor3f(0,0,0);
   cubo(40,85,20);  // Nuca
   glTranslatef(45,20,37);
   gluCylinder(GeneralQuadric,20,20,41.5,20,2); // Ojo Der
   glTranslatef(0,0,41.5);
   gluDisk(GeneralQuadric,0,20,20,1); // Iris Der
   glTranslatef(-90,0,0);
   gluDisk(GeneralQuadric,0,20,20,1); // Iris Izq
   glTranslatef(0,0,-41.5);
   gluCylinder(GeneralQuadric,20,20,41.5,20,2); // Ojo Izq
  glPopMatrix();
}

void GlGeometer::panTilt(float nP,float nT){
  panTh = nP;
  tiltTh = nT;
  
  while (panTh<0)   panTh+=360;
  while (panTh>360) panTh-=360;

  while (tiltTh<0)   tiltTh+=360;
  while (tiltTh>360) tiltTh-=360;
}

void GlGeometer::pan(float nP){
  panTh = nP;

  while (panTh<0)   panTh+=360;
  while (panTh>360) panTh-=360;
}

void GlGeometer::panRel(float nP){
  panTh += nP;

  while (panTh<0)   panTh+=360;
  while (panTh>360) panTh-=360;
}

void GlGeometer::tilt(float nT){
  tiltTh = nT;
  
  while (tiltTh<0)   tiltTh+=360;
  while (tiltTh>360) tiltTh-=360;
}

void GlGeometer::tiltRel(float nT){
  tiltTh += nT;
  
  while (tiltTh<0)   tiltTh+=360;
  while (tiltTh>360) tiltTh-=360;
}

void GlGeometer::panTiltRel(float nP,float nT){
  panTh += nP;
  tiltTh += nT;

  while (panTh<0)   panTh+=360;
  while (panTh>360) panTh-=360;
  
  while (tiltTh<0)   tiltTh+=360;
  while (tiltTh>360) tiltTh-=360;
}
	
float GlGeometer::getPan(){
  return (panTh);
}

float GlGeometer::getTilt(){
  return (tiltTh);
}


void GlGeometer::gotoEyeR(){
   glTranslatef(posX,posY,posZ);
   glRotatef(posTh,0,1,0);
   glRotatef(GEOMETER_INCLINACION,1,0,0);
   glTpt(45,-87.5,-22.5);
   glRotatef(panTh,0,1,0);
   glRotatef(tiltTh,1,0,0);
   glTranslatef(45,-60,55);
}

void GlGeometer::gotoEyeL(){
   glTranslatef(posX,posY,posZ);
   glRotatef(posTh,0,1,0);
   glRotatef(GEOMETER_INCLINACION,1,0,0);
   glTpt(45,-87.5,-22.5);
   glRotatef(panTh,0,1,0);
   glRotatef(tiltTh,1,0,0);
   glTranslatef(-45,-60,55);
}

void GlGeometer::getEyeL(float *coord){
  /* coordenadas tomadas de gotoEeyeL 
   * ojo=(22.5,-577.5,167.5) y rotadas
   * en el eje x=(1,0,0) usando GEOMETER_INCLINACION
   * como angulo, esto en Conformal.
   */
  coord[0]=22.5f;
  coord[1]=-573.235f;
  coord[2]=181.56f;
}

/*** LASER ***/

void GlGeometer::dLaser(){
  float lx,ly,lz;
  glPushMatrix();
    glTl(75,-90,-100);

    lx = 150 * 0.5;
    ly = 180 * 0.5;
    lz = 210 * 0.5;

    glBegin (GL_TRIANGLE_STRIP);
      glColor3f(0,0,1);
      //glColor3f(0,0,0);
      glNormal3f (0,0,-1);
      glVertex3f (-lx,ly,-lz);
      glVertex3f (lx,ly,-lz);
      glVertex3f (-lx,-ly,-lz);
      glVertex3f (lx,-ly,-lz);
      //glColor3f(0,0,1);
      glNormal3f (0,-1,0);
      glVertex3f (-lx,-ly,lz);
      glVertex3f (lx,-ly,lz);
    glEnd();
    glBegin (GL_QUADS);
      glNormal3f (1,0,0);
      glVertex3f (lx,ly,0);
      glVertex3f (lx,-ly,0);
      //glColor3f(0,0,0);
      glVertex3f (lx,-ly,-lz);
      glVertex3f (lx,ly,-lz);
    glEnd();
    glBegin (GL_QUADS);
      glNormal3f (-1,0,0);
      glVertex3f (-lx,ly,-lz);
      glVertex3f (-lx,-ly,-lz);
      //glColor3f(0,0,1);
      glVertex3f (-lx,-ly,0);
      glVertex3f (-lx,ly,0);
    glEnd();
    glBegin (GL_TRIANGLE_STRIP);
      glNormal3f (-1,0,0);
      glVertex3f (-lx,-ly+70,0);
      glVertex3f (-lx,-ly,0);
      glVertex3f (-lx,-ly+70,lz);
      glVertex3f (-lx,-ly,lz);
      glNormal3f (0,0,1);
      glVertex3f (lx,-ly+70,lz);
      glVertex3f (lx,-ly,lz);
      glNormal3f (1,0,0);
      glVertex3f (lx,-ly+70,0);
      glVertex3f (lx,-ly,0);
    glEnd();
    glBegin (GL_TRIANGLE_STRIP);
      glNormal3f (-1,0,0);
      glVertex3f (-lx,ly,0);
      glVertex3f (-lx,ly-10,0);
      glVertex3f (-lx,ly,lz);
      glVertex3f (-lx,ly-10,lz);
      glNormal3f (0,0,1);
      glVertex3f (lx,ly,lz);
      glVertex3f (lx,ly-10,lz);
      glNormal3f (1,0,0);
      glVertex3f (lx,ly,0);
      glVertex3f (lx,ly-10,0);
    glEnd();
   glTranslatef(0,80,22);
   glRotatef(90,1,0,0);
   glColor3f(0,0,0);
   gluCylinder(GeneralQuadric,80,80,100,10,2);
  glPopMatrix();
  if(bLaserOn) {
    glPushMatrix();
      glTl(75,-60,-60);
      glColor3f(1,0,0);
      glRotatef(-90,0,1,0);
      glRotatef(laserAng,0,1,0);
      glLineWidth(3);
      glBegin(GL_LINES);
        glVertex3f(0,0,0);
        glVertex3f(1000,0,0);
      glEnd();
      glLineWidth(1);

      if(laserAng == 90) bLDir=false;
      if(laserAng == -90) bLDir=true;
      if(bLDir) laserAng++;
       else laserAng--;
    glPopMatrix();
  }
}

void GlGeometer::laserOn(bool bLON){
  bLaserOn = bLON;
}

void GlGeometer::gotoLaser(){
   glTranslatef(posX,posY,posZ);
   glRotatef(posTh,0,1,0);
   glRotatef(GEOMETER_INCLINACION,1,0,0);
   glTl(75,-60,-60);
}

void GlGeometer::gotoLaserZero(){
   glTranslatef(posX,posY,posZ);
   glRotatef(posTh,0,1,0);
   glRotatef(GEOMETER_INCLINACION,1,0,0);
   glTl(20,-60,-190);
}
void GlGeometer::gotoLaserHeight(){
   glTranslatef(0,posY,0);
   glTl(75,-60,-60);
}

void GlGeometer::gotoLaserHeightZero(){
   glTranslatef(0,posY,0);
   glTl(20,-60,-190);
}

/*** OMNI ***/

void GlGeometer::dOmni(){
  glPushMatrix();
   glTo(10,-365,-10);
   glColor3f(0,0,0);
   cubo(20,730,20);
   glTranslatef(0,-355,75);
   cubo(20,20,130);
   glTranslatef(0,180,-42.5);
   cubo(20,20,45);
   glTranslatef(0,0,27.5);
   glColor3f(0.75,0.75,0.25);
   cubo(10,10,10);  
   glTranslatef(0,0,30);
   glColor3f(0.875,0.875,0.875);
   cubo(50,40,50);  // Base de Camara
   glTranslatef(0,-20,0);
   glRotatef(90,1,0,0);
   glColor3f(0,0,0);
   gluCylinder(GeneralQuadric,17.5,17.5,45,20,2); // Camara
   glTranslatef(0,0,45);
   gluDisk(GeneralQuadric,0,17.5,20,1); // Ojo de Camara
   glTranslatef(0,0,95);
   glColor3f(0.75,0.75,0.25);
   cubo(10,10,20);
   glTranslatef(0,0,-10);
   glColor3f(1,1,1);
   gluDisk(GeneralQuadric,0,31,20,1); // Lente parte superior
   glTranslatef(0,0,-25);
   gluCylinder(GeneralQuadric,0,31,25,20,2); // Lente
  glPopMatrix();
}

void GlGeometer::gotoOEye(){
   glTranslatef(posX,posY,posZ);
   glRotatef(posTh,0,1,0);
   glRotatef(GEOMETER_INCLINACION,1,0,0);
   glTo(10,-365,-10);
   glTranslatef(0,-195,90);
   glRotatef(90,1,0,0);
   glTranslatef(0,0,45);
}

/*** Funciones miselaneas ***/

void GlGeometer::verLineas(bool bl){
  bLineas=bl;
}

void GlGeometer::verPiso(bool bp){
  bPiso=bp;
}

void GlGeometer::opciones(int opcion){
   bLineas  = (bool)(opcion & GLGEOMETER_VERLINEAS); 
   bPiso    = (bool)(opcion & GLGEOMETER_VERPISO);
   bLaserOn = (bool)(opcion & GLGEOMETER_LASERON);
   bSonarOn = (bool)(opcion & GLGEOMETER_SONARON);
   bSolido  = (bool)(opcion & GLGEOMETER_SOLIDO	);
}

void GlGeometer::punto(){
  glPointSize(3);
  glBegin(GL_POINTS);
    glVertex3f(0,0,0);
  glEnd();
  glPointSize(1);
}

/*
  glPushMatrix();
    glColor3f(1, 1, 1);
    glTranslatef(posX,posY,posZ);
    glRotatef(posTh,0,1,0);
    glBegin(GL_LINES); 
      glVertex3f(0,0,-1000);
      glVertex3f(0,0,2000);
    glEnd();
    gluSphere(GeneralQuadric, 500, 12, 8);
  glPopMatrix();
  */

