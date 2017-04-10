#ifndef GLGEOMETER_H
#define GLGEOMETER_H 

#ifdef WIN32
 #include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>
//#include <GL/glut.h>

#define PIONEER_ALTURA 240
//#define GEOMETER_INCLINACION -1.4
#define GEOMETER_INCLINACION 0

#define GLGEOMETER_VERLINEAS	0x0001
#define GLGEOMETER_VERPISO	0x0002
#define GLGEOMETER_LASERON	0x0004
#define GLGEOMETER_SONARON	0x0008
#define GLGEOMETER_SOLIDO	0x0010

class GlGeometer {
  private:
	float posX;   // Posición en X
	float posY;   // Posición en Y
	float posZ;   // Posición en Z
	float posTh;  // Angulo  en grados

	float panTh;  // Angulo del Pan
	float tiltTh; // Angulo del Tilt

        // Quadrics are used to draw many different shapes.
	GLUquadricObj *GeneralQuadric;
	bool  bLineas; // ver solo las lineas de contorno
	bool  bSolido; // ver solido las figuras
	bool  bPiso;   // ver Piso donde esta geometer

	bool  bLaserOn; // activar animación de laser
	int   laserAng; // angulo actual del laser animado
	bool  bLDir; // direccion del laser

	bool  bSonarOn; // activa animación del sonar
	int   sonarNum; // numero actual del sonar animado
	bool  bSDir; // direccion del sonar
	
	/*** Funciones miselaneas ***/

	// Dibuja un cubo, largo en X, largo en Y, largo Z
	void cubo(float lX, float lY, float lZ);
	// Dibuja un cuadro, largo en X, largo en Y
	void cuadro(float lX, float lY);

	// coordenadas de Pan-Tilt
	void glTpt(float trX=0,float trY=0,float trZ=0);
	// coordenadas del Laser
	void glTl(float lX=0,float lY=0,float lZ=0);
	// coordenadas del Omnidireccional
	void glTo(float oX=0,float oY=0,float oZ=0);
	void punto();

  public:   

	GlGeometer();
	~GlGeometer();

	// Función principal que dibuja a Geometer
	void dibuja();

	/*** PIONEER ***/
	// dibuja el pionerr
	void dPioneer();
	// Habilita animación del Sonar
	void sonarOn(bool bSON = true);
	
	// Se mueve a la posicion (nX, nY, nZ)
	void moveTo(float nX, float nY, float nZ, float nTh);
	// Se mueve a la posicion relativa (nX, nY, nZ)
	void moveRel(float nX=0, float nY=0, float nZ=0, float nTh=0);

	// Debuelve su posición en X
	float getX();
	// Debuelve su posición en Y
	float getY();
	// Debuelve su posición en Z
	float getZ();
	// Debuelve su Angulo en grados
	float getTh();


	/*** PAN TILT ***/
	// dibuja el Pan-Tilt
	void dPantTilt();

	// Pans and tilts to the given degrees.
	void panTilt(float nP,float nT);
	// Pans to the given degrees.
	void pan(float nP);
	// Pans relative to current position by given degrees.
	void panRel(float nP);
	// Tilts to the given degrees.
	void tilt(float nT);
	// Tilts relative to the current position by given degrees.
	void tiltRel(float nT);
	// Pans and tilts relatives to the current position by the given degrees.
	void panTiltRel(float nP,float nT);
	
	// The angle the camera was last told to pan to.
	float getPan();
	// The angle the camera was last told to tilt to.
	float getTilt();

	// Debuelve las coordenadas del ojo Derecho
	void gotoEyeR();
	// Debuelve las coordenadas del ojo Izquierdo
	void gotoEyeL();
	// Debuelve las coordenadas absolutas del ojo Izquierdo con pan=0,tilt=0
	void getEyeL(float *coord);

	/*** LASER ***/
	// dibuja el Laser
	void dLaser();
	// Habilita animación del Laser
	void laserOn(bool bLON = true);

	// Debuelve las coordenadas del centro del Laser
	void gotoLaser();
	// Debuelve las coordenadas del centro y altura del Laser en (0,altura,0)
	void gotoLaserZero();
	// Debuelve las coordenadas de la altura del centro del Laser
	void gotoLaserHeight();
	// Debuelve las coordenadas de la altura del centro del Laser sin desplazamiento
	void gotoLaserHeightZero();

	/*** OMNI ***/
	// dibuja el omnidireccional
	void dOmni();

	// Debuelve las coordenadas del ojo del Obmni
	void gotoOEye();


	/*** Funciones miselaneas ***/

	// Habilitar ver lineas de contorno
	void verLineas(bool bl=true);
	// Habilita ver piso debajo de Geometer
	void verPiso(bool bp=true);
	// Habilita opciones del robot con un solo argumento
	void opciones(int opcion);
};

#endif

