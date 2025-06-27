#include <iostream>
#include <windows.h>
#include <GL/glut.h>
#include <stdio.h>
using namespace std;
GLfloat angulo=0.0;
//BOLA POSICION
GLfloat bolaPosX=30;
GLfloat bolaPosY=-90;
GLfloat bolaPosZ=0;

GLfloat angX=0.0,angY=0.0,angZ=0.0,mod_angX=0.0,mod_angY=0.0,mod_angZ=0.0;
void animacion(){
	angX+=mod_angX;
	angY+=mod_angY;
	angZ+=mod_angZ;
	
	if(angX>=360) angX=0;
	if(angY>=360) angY=0;
	if(angZ>=360) angZ=0;
	
	glRotatef(angX,1,0,0);
	glRotatef(angY,0,1,0);
	glRotatef(angZ,0,0,1);
}
typedef struct{
 GLubyte *dibujo;
 GLuint bpp;
 GLuint largo;
 GLuint ancho;
 GLuint ID;
 	
} textura;

textura tyoshi;
textura tfondo;
textura tpersonaje;
textura tcubo;
textura tcarnivora;
textura ttubo;
textura tfantasma;
textura tcarahongo;
textura tcabezahongo;
textura tojoshongo;
textura tfondoverde;
textura ttextomoneda;
textura ttextopuntaje;
textura tmoneda;
textura testrella;
textura tcubo_pregunta;
textura tnube;
textura tlogo;

int cargaTGA(char const*nombre, textura *imagen){
	GLubyte cabeceraTGA[12]={0,0,2,0,0,0,0,0,0,0,0,0};
	GLubyte compararTGA[12];//0,0,2,0,0,0,0,0,0,0,0,0
	GLubyte cabecera[6];// 244,1,244,1,32,40
	GLuint bytesporpunto;
	GLuint tamanoimagen;
	GLuint temp,i;
	GLuint tipo=GL_RGBA;
	
	FILE *archivo= fopen(nombre,"rb");
	
	if(
	archivo==NULL ||
	fread(compararTGA,1,sizeof(compararTGA),archivo)!=sizeof(compararTGA) ||
	memcmp(cabeceraTGA,compararTGA,sizeof(compararTGA))!=0 ||
	fread(cabecera,1,sizeof(cabecera),archivo)!=sizeof(cabecera)
	){
	   if(archivo==NULL){
	   	printf("NO SE ENCONTRO ARCHIVO %s\n", nombre);
	   	return 0;
	   }else{
	   	fclose(archivo);
	   	return 0;
	   }	
	}
	
	
	imagen->largo= 256*cabecera[1] + cabecera[0];
	          // 256 * 1 + 244=256+244=500
	imagen->ancho=256 * cabecera[3]+cabecera[2];
	    //256 * 1 + 244=256+244=500
    if(
	imagen->largo <=0  ||
	imagen->ancho <=0  ||
	(cabecera[4]!=24  && cabecera[4]!=32)// 24 = RGB , 32=RGBA
	) {
		printf("Datos invalidos\n");
	
		fclose(archivo);
		return 0;
	}
	imagen->bpp=cabecera[4];
	bytesporpunto=cabecera[4]/8;// 4 bytes
	
    tamanoimagen=imagen->largo * imagen->ancho * bytesporpunto; 
    //500 * 500 + 4 = 1000000
    //1000000 /1024= 976,56 =977 
    imagen->dibujo =(GLubyte *)malloc(tamanoimagen);
    if(
	imagen->dibujo==NULL ||
	fread(imagen->dibujo,1,tamanoimagen,archivo)!=tamanoimagen
	){
		
		if(imagen->dibujo != NULL){
			printf("Error leyendo imagen\n");
			free(imagen->dibujo);
			
		}else{
			printf("Error asignando memoria\n");
		}
		fclose(archivo);
		return 0;
	}
    // TGA : BGRA                RGBA
    
    for(i=0; i<(int)tamanoimagen;i+=bytesporpunto){
    	temp=imagen->dibujo[i];
    	imagen->dibujo[i]=imagen->dibujo[i+2];
    	imagen->dibujo[i+2]=temp;
    	
	}
	fclose(archivo);
	glGenTextures(1,&imagen[0].ID);
	glBindTexture(GL_TEXTURE_2D,imagen[0].ID);
	if(imagen->bpp==24) tipo=GL_RGB;	
	glTexImage2D(GL_TEXTURE_2D,0,tipo,imagen[0].ancho,imagen[0].largo,0,tipo,GL_UNSIGNED_BYTE,imagen[0].dibujo);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
   glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    return 1;
}

void init(){
	GLfloat light_position[]={0,0,1,0};
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	
	glLightfv(GL_LIGHT0,GL_POSITION,light_position);
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	
	glShadeModel(GL_SMOOTH);//Permite mejorar sombra en tu modelado
	glClearColor(0.0,0.0,0.0,0.0);
	glClearDepth(1.0);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glEnable(GL_TEXTURE_2D);
	//CARGA DE TEXTURA	
	if(!cargaTGA("yoshi2.tga",&tyoshi)){
		printf("Error cargando textura\n");
		exit(0);
	}	
	if(!cargaTGA("fondo.tga",&tfondo)){
		printf("Error cargando textura\n");
		exit(0);
	}
	if(!cargaTGA("TexPersonaje.tga",&tpersonaje)){
		printf("Error cargando textura\n");
		exit(0);
	}
	if(!cargaTGA("TexCubo.tga",&tcubo)){
		printf("Error cargando textura\n");
		exit(0);
	}
	if(!cargaTGA("tubo.tga",&ttubo)){
		printf("Error cargando textura\n");
		exit(0);
	}
	if(!cargaTGA("carnivora.tga",&tcarnivora)){
		printf("Error cargando textura\n");
		exit(0);
	}
	if(!cargaTGA("ghost3.tga",&tfantasma)){
		printf("Error cargando textura\n");
		exit(0);
	}
	if(!cargaTGA("cara_hongo.tga",&tcarahongo)){
		printf("Error cargando textura\n");
		exit(0);
	}
	if(!cargaTGA("TexRojo.tga",&tcabezahongo)){
		printf("Error cargando textura\n");
		exit(0);
	}
	if(!cargaTGA("ojoshongo.tga",&tojoshongo)){
		printf("Error cargando textura\n");
		exit(0);
	}
	/////////////////////////////////////////////
	if(!cargaTGA("fondoVerde.tga",&tfondoverde)){
		printf("Error cargando textura\n");
		exit(0);
	}
	if(!cargaTGA("moneda_texto.tga",&ttextomoneda)){
		printf("Error cargando textura\n");
		exit(0);
	}
	if(!cargaTGA("pntj_texto.tga",&ttextopuntaje)){
		printf("Error cargando textura\n");
		exit(0);
	}
	if(!cargaTGA("moneda.tga",&tmoneda)){
		printf("Error cargando textura\n");
		exit(0);
	}
	if(!cargaTGA("estrella.tga",&testrella)){
		printf("Error cargando textura\n");
		exit(0);
	}
	if(!cargaTGA("TexCaja.tga",&tcubo_pregunta)){
		printf("Error cargando textura\n");
		exit(0);
	}
	if(!cargaTGA("nube.tga",&tnube)){
		printf("Error cargando textura\n");
		exit(0);
	}
	if(!cargaTGA("logoMario.tga",&tlogo)){
		printf("Error cargando textura\n");
		exit(0);
	}	
}

void yoshi(){
	glDisable(GL_LIGHTING);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_NOTEQUAL, 0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,tyoshi.ID);
	glTranslatef(350,-730,0);
	glRotatef(angulo*1.3,0,1,0);	
	int t=150;
	glBegin(GL_QUADS);
		glTexCoord2f(0,0);glVertex3d(t,t,-t);
		glTexCoord2f(1,0);glVertex3d(-t,t,-t);
	  	glTexCoord2f(1,1);glVertex3d(-t,-t,-t);
	  	glTexCoord2f(0,1);glVertex3d(t,-t,-t);
	glEnd();
	glLoadIdentity();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_ALPHA_TEST);
	glEnable(GL_LIGHTING);	
}

void fantasma(){
	glDisable(GL_LIGHTING);
	//glDisable(GL_DEPTH_TEST);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_NOTEQUAL, 0);	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,tfantasma.ID);
	glTranslatef(-400,-600,0);
	glRotatef(angulo,0,1,0);
	
		
	int t=130;
	glBegin(GL_QUADS);
		glTexCoord2f(0,0);glVertex3d(t,t,0);
		glTexCoord2f(1,0);glVertex3d(-t,t,0);
	  	glTexCoord2f(1,1);glVertex3d(-t,-t,0);
	  	glTexCoord2f(0,1);glVertex3d(t,-t,0);
	glEnd();
	glLoadIdentity();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_ALPHA_TEST);
	//glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);	
}

void fondo(){	
	glDisable(GL_LIGHTING);
	//glDisable(GL_DEPTH_TEST);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_NOTEQUAL, 0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,tfondo.ID);	
	int t=999;
	glBegin(GL_QUADS);
		
		glTexCoord2f(0,0);glVertex3d(t,t,-t);
		glTexCoord2f(1,0);glVertex3d(-t,t,-t);
	  	glTexCoord2f(1,1);glVertex3d(-t,-t,-t);
	  	glTexCoord2f(0,1);glVertex3d(t,-t,-t);
	glEnd();
	glLoadIdentity();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_ALPHA_TEST);
	//glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);	
}

void hongo(){	
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glTranslatef(220,10,0);
	glScalef(1.20,1,1);
	glBindTexture(GL_TEXTURE_2D,tcarahongo.ID);	
	int c=40;
	glBegin(GL_QUADS);
	
	  glTexCoord2f(0,0); glVertex3d(-c,c,c);
	  glTexCoord2f(1,0); glVertex3d(c,c,c);
	  glTexCoord2f(1,1); glVertex3d(30,-c,c);
	  glTexCoord2f(0,1); glVertex3d(-30,-c,c);
	  
	  glTexCoord2f(0,0); glVertex3d(c,c,c);
	  glTexCoord2f(1,0); glVertex3d(c,c,-c);
	  glTexCoord2f(1,1); glVertex3d(30,-c,-c);
	  glTexCoord2f(0,1); glVertex3d(30,-c,c);
	  
	  glTexCoord2f(0,0); glVertex3d(c,c,-c);
	  glTexCoord2f(1,0); glVertex3d(-c,c,-c);
	  glTexCoord2f(1,1); glVertex3d(-30,-c,-c);
	  glTexCoord2f(0,1); glVertex3d(30,-c,-c);
	  
	  glTexCoord2f(0,0); glVertex3d(-30,-c,c);
	  glTexCoord2f(1,0); glVertex3d(30,-c,c);
	  glTexCoord2f(1,1); glVertex3d(30,-c,-c);
	  glTexCoord2f(0,1); glVertex3d(-30,-c,-c);
	  
	  glTexCoord2f(0,0); glVertex3d(-c,c,-c);
	  glTexCoord2f(1,0); glVertex3d(-c,c,c);
	  glTexCoord2f(1,1); glVertex3d(-30,-c,c);
	  glTexCoord2f(0,1); glVertex3d(-30,-c,-c);
	  
	  glTexCoord2f(0,0); glVertex3d(-c,c,-c);
	  glTexCoord2f(1,0); glVertex3d(c,c,-c);
	  glTexCoord2f(1,1); glVertex3d(c,c,c);
	  glTexCoord2f(0,1); glVertex3d(-c,c,c);
	  
	glEnd();
	glLoadIdentity();
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	animacion();
	glTranslatef(220,65,0);
	glRotatef(90,1,0,0);
	
	
	GLfloat mat_ambient[]={0.75f,0.24f,0.20f,1};
	GLfloat mat_diffuse[]={1.0f,0.0f,0.0f,1.0f};
	GLfloat mat_specular[]={1.0f,1.0f,1.0f,1.0f};
	GLfloat mat_shininess[]={30.0f};
	
	glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,mat_ambient);
	glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,mat_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,mat_specular);
	glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,mat_shininess);
	
	
	glutSolidTorus(20,50,50,50);
	glLoadIdentity();
	animacion();
	glTranslatef(220,95,0);
	glRotatef(90,1,0,1);
	
	glDisable(GL_LIGHTING);	
	glEnable(GL_TEXTURE_2D);
		
	GLUquadric *qobj= gluNewQuadric();
	gluQuadricTexture(qobj,GL_TRUE);
	
	glBindTexture(GL_TEXTURE_2D,tcabezahongo.ID);
	gluSphere(qobj,55,50,50);
	
	gluDeleteQuadric(qobj);
	glLoadIdentity();
	glDisable(GL_TEXTURE_2D);
	///////////////////
	glDisable(GL_LIGHTING);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_NOTEQUAL, 0);	
	glEnable(GL_TEXTURE_2D);
	animacion();
	glTranslatef(220,15,1);
	glScalef(1.20,1,1);
	glBindTexture(GL_TEXTURE_2D,tojoshongo.ID);	
	int b=50;
	glBegin(GL_QUADS);
	
	  glTexCoord2f(0,0); glVertex3d(-40,b,b);
	  glTexCoord2f(1,0); glVertex3d(40,b,b);
	  glTexCoord2f(1,1); glVertex3d(40,-b,b);
	  glTexCoord2f(0,1); glVertex3d(-40,-b,b);
	glEnd();
	glLoadIdentity();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_ALPHA_TEST);
	glEnable(GL_LIGHTING);	
}

void personaje(){	
	glDisable(GL_LIGHTING);
	glRotatef(90,1,0,0);
	glRotatef(angulo,0,1,0);
	glEnable(GL_TEXTURE_2D);
		
	GLUquadric *qobj= gluNewQuadric();
	gluQuadricTexture(qobj,GL_TRUE);
	
	glBindTexture(GL_TEXTURE_2D,tpersonaje.ID);
	gluSphere(qobj,70,50,50);
	
	gluDeleteQuadric(qobj);
	glLoadIdentity();
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);	
}

void cubo(float posX, float posY){	
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glTranslated(posX,posY,0);
	//glRotatef(angulo,1,0,0);
	glBindTexture(GL_TEXTURE_2D,tcubo.ID);		
	int c=70;
	glBegin(GL_QUADS);
	  glTexCoord2f(0,0); glVertex3d(-c,c,c);
	  glTexCoord2f(1,0); glVertex3d(c,c,c);
	  glTexCoord2f(1,1); glVertex3d(c,-c,c);
	  glTexCoord2f(0,1); glVertex3d(-c,-c,c);
	  
	  glTexCoord2f(0,0); glVertex3d(c,c,c);
	  glTexCoord2f(1,0); glVertex3d(c,c,-c);
	  glTexCoord2f(1,1); glVertex3d(c,-c,-c);
	  glTexCoord2f(0,1); glVertex3d(c,-c,c);
	  
	  glTexCoord2f(0,0); glVertex3d(c,c,-c);
	  glTexCoord2f(1,0); glVertex3d(-c,c,-c);
	  glTexCoord2f(1,1); glVertex3d(-c,-c,-c);
	  glTexCoord2f(0,1); glVertex3d(c,-c,-c);
	  
	  glTexCoord2f(0,0); glVertex3d(-c,-c,c);
	  glTexCoord2f(1,0); glVertex3d(c,-c,c);
	  glTexCoord2f(1,1); glVertex3d(c,-c,-c);
	  glTexCoord2f(0,1); glVertex3d(-c,-c,-c);
	  
	  glTexCoord2f(0,0); glVertex3d(-c,-c,c);
	  glTexCoord2f(1,0); glVertex3d(c,-c,c);
	  glTexCoord2f(1,1); glVertex3d(c,-c,-c);
	  glTexCoord2f(0,1); glVertex3d(-c,-c,-c);
	  
	  glTexCoord2f(0,0); glVertex3d(-c,c,-c);
	  glTexCoord2f(1,0); glVertex3d(-c,c,c);
	  glTexCoord2f(1,1); glVertex3d(-c,-c,c);
	  glTexCoord2f(0,1); glVertex3d(-c,-c,-c);
	  
	  glTexCoord2f(0,0); glVertex3d(-c,c,-c);
	  glTexCoord2f(1,0); glVertex3d(c,c,-c);
	  glTexCoord2f(1,1); glVertex3d(c,c,c);
	  glTexCoord2f(0,1); glVertex3d(-c,c,c);
	glEnd();
	glLoadIdentity();
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);	
}

void tubo(){	
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glTranslated(680,-750,0);
	glScalef(1,1.6,1);
	glBindTexture(GL_TEXTURE_2D,ttubo.ID);		
	int c=70;
	glBegin(GL_QUADS);
	  glTexCoord2f(0,0); glVertex3d(-c,c,c);
	  glTexCoord2f(1,0); glVertex3d(c,c,c);
	  glTexCoord2f(1,1); glVertex3d(c,-c,c);
	  glTexCoord2f(0,1); glVertex3d(-c,-c,c);
	  
	  glTexCoord2f(0,0); glVertex3d(c,c,c);
	  glTexCoord2f(1,0); glVertex3d(c,c,-c);
	  glTexCoord2f(1,1); glVertex3d(c,-c,-c);
	  glTexCoord2f(0,1); glVertex3d(c,-c,c);
	  
	  glTexCoord2f(0,0); glVertex3d(c,c,-c);
	  glTexCoord2f(1,0); glVertex3d(-c,c,-c);
	  glTexCoord2f(1,1); glVertex3d(-c,-c,-c);
	  glTexCoord2f(0,1); glVertex3d(c,-c,-c);
	  
	  glTexCoord2f(0,0); glVertex3d(-c,-c,c);
	  glTexCoord2f(1,0); glVertex3d(c,-c,c);
	  glTexCoord2f(1,1); glVertex3d(c,-c,-c);
	  glTexCoord2f(0,1); glVertex3d(-c,-c,-c);
	  
	  glTexCoord2f(0,0); glVertex3d(-c,-c,c);
	  glTexCoord2f(1,0); glVertex3d(c,-c,c);
	  glTexCoord2f(1,1); glVertex3d(c,-c,-c);
	  glTexCoord2f(0,1); glVertex3d(-c,-c,-c);
	  
	  glTexCoord2f(0,0); glVertex3d(-c,c,-c);
	  glTexCoord2f(1,0); glVertex3d(-c,c,c);
	  glTexCoord2f(1,1); glVertex3d(-c,-c,c);
	  glTexCoord2f(0,1); glVertex3d(-c,-c,-c);
	  
	  glTexCoord2f(0,0); glVertex3d(-c,c,-c);
	  glTexCoord2f(1,0); glVertex3d(c,c,-c);
	  glTexCoord2f(1,1); glVertex3d(c,c,c);
	  glTexCoord2f(0,1); glVertex3d(-c,c,c);
	glEnd();
	glLoadIdentity();
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);	
}

void carnivora(){
	
	glDisable(GL_LIGHTING);
	glTranslated(680,-650,0);
	glEnable(GL_TEXTURE_2D);
		
	GLUquadric *qobj= gluNewQuadric();
	gluQuadricTexture(qobj,GL_TRUE);
	
	glBindTexture(GL_TEXTURE_2D,tcarnivora.ID);
	gluSphere(qobj,50,50,50);
	
	gluDeleteQuadric(qobj);
	glLoadIdentity();
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);	
}

void fondoverde(){
	glDisable(GL_LIGHTING);
	//glDisable(GL_DEPTH_TEST);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_NOTEQUAL, 0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,tfondoverde.ID);	
	int t=999;
	glBegin(GL_QUADS);
		
		glTexCoord2f(0,0);glVertex3d(1050,t,-t);
		glTexCoord2f(1,0);glVertex3d(-t,t,-t);
	  	glTexCoord2f(1,1);glVertex3d(-t,-t,-t);
	  	glTexCoord2f(0,1);glVertex3d(1050,-t,-t);
	glEnd();
	glLoadIdentity();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_ALPHA_TEST);
	//glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);	
}

void textopuntaje(){
	glDisable(GL_LIGHTING);	
	//glDisable(GL_DEPTH_TEST);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_NOTEQUAL, 0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,ttextopuntaje.ID);	
	glTranslated(-800,840,0);
	int t=120;
	glBegin(GL_QUADS);	
		glTexCoord2f(0,0);glVertex3d(-t,t,-t);
		glTexCoord2f(1,0);glVertex3d(t,t,-t);
	  	glTexCoord2f(1,1);glVertex3d(t,-t,-t);
	  	glTexCoord2f(0,1);glVertex3d(-t,-t,-t);
	glEnd();
	glLoadIdentity();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_ALPHA_TEST);
	//glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);	
	
}

void textomoneda(){
	glDisable(GL_LIGHTING);	
	//glDisable(GL_DEPTH_TEST);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_NOTEQUAL, 0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,ttextomoneda.ID);	
	glTranslated(-530,840,0);
	int t=120;
	glBegin(GL_QUADS);	
		glTexCoord2f(0,0);glVertex3d(-t,t,-t);
		glTexCoord2f(1,0);glVertex3d(t,t,-t);
	  	glTexCoord2f(1,1);glVertex3d(t,-t,-t);
	  	glTexCoord2f(0,1);glVertex3d(-t,-t,-t);
	glEnd();
	glLoadIdentity();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_ALPHA_TEST);
	//glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);	
	
}

void moneda(float posX, float posY){
	glDisable(GL_LIGHTING);
	//glDisable(GL_DEPTH_TEST);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_NOTEQUAL, 0);	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,tmoneda.ID);
	glTranslatef(posX,posY,0);
	glRotatef(angulo,0,1,0);	
	int t=10;
	int z=60;
	glBegin(GL_QUADS);
		glTexCoord2f(0,0);glVertex3d(z,z,-t);
		glTexCoord2f(1,0);glVertex3d(-z,z,-t);
	  	glTexCoord2f(1,1);glVertex3d(-z,-z,-t);
	  	glTexCoord2f(0,1);glVertex3d(z,-z,-t);
	glEnd();
	glLoadIdentity();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_ALPHA_TEST);
	//glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);	
}

void estrella(){
	glDisable(GL_LIGHTING);
	//glDisable(GL_DEPTH_TEST);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_NOTEQUAL, 0);	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,testrella.ID);
	glTranslatef(440,58,0);
	glRotatef(angulo,0,1,0);	
	int t=10;
	int z=75;
	glBegin(GL_QUADS);
		glTexCoord2f(0,0);glVertex3d(z,z,-t);
		glTexCoord2f(1,0);glVertex3d(-z,z,-t);
	  	glTexCoord2f(1,1);glVertex3d(-z,-z,-t);
	  	glTexCoord2f(0,1);glVertex3d(z,-z,-t);
	glEnd();
	glLoadIdentity();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_ALPHA_TEST);
	//glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);	
}

void cubo_pregunta(){
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glTranslated(400,400,0);
	glRotatef(angulo*0.7,0,1,0);
	glBindTexture(GL_TEXTURE_2D,tcubo_pregunta.ID);		
	int c=80;
	glBegin(GL_QUADS);
	  glTexCoord2f(0,0); glVertex3d(-c,c,c);
	  glTexCoord2f(1,0); glVertex3d(c,c,c);
	  glTexCoord2f(1,1); glVertex3d(c,-c,c);
	  glTexCoord2f(0,1); glVertex3d(-c,-c,c);
	  
	  glTexCoord2f(0,0); glVertex3d(c,c,c);
	  glTexCoord2f(1,0); glVertex3d(c,c,-c);
	  glTexCoord2f(1,1); glVertex3d(c,-c,-c);
	  glTexCoord2f(0,1); glVertex3d(c,-c,c);
	  
	  glTexCoord2f(0,0); glVertex3d(c,c,-c);
	  glTexCoord2f(1,0); glVertex3d(-c,c,-c);
	  glTexCoord2f(1,1); glVertex3d(-c,-c,-c);
	  glTexCoord2f(0,1); glVertex3d(c,-c,-c);
	  
	  glTexCoord2f(0,0); glVertex3d(-c,-c,c);
	  glTexCoord2f(1,0); glVertex3d(c,-c,c);
	  glTexCoord2f(1,1); glVertex3d(c,-c,-c);
	  glTexCoord2f(0,1); glVertex3d(-c,-c,-c);
	  
	  glTexCoord2f(0,0); glVertex3d(-c,-c,c);
	  glTexCoord2f(1,0); glVertex3d(c,-c,c);
	  glTexCoord2f(1,1); glVertex3d(c,-c,-c);
	  glTexCoord2f(0,1); glVertex3d(-c,-c,-c);
	  
	  glTexCoord2f(0,0); glVertex3d(-c,c,-c);
	  glTexCoord2f(1,0); glVertex3d(-c,c,c);
	  glTexCoord2f(1,1); glVertex3d(-c,-c,c);
	  glTexCoord2f(0,1); glVertex3d(-c,-c,-c);
	  
	  glTexCoord2f(0,0); glVertex3d(-c,c,-c);
	  glTexCoord2f(1,0); glVertex3d(c,c,-c);
	  glTexCoord2f(1,1); glVertex3d(c,c,c);
	  glTexCoord2f(0,1); glVertex3d(-c,c,c);
	glEnd();
	glLoadIdentity();
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);	
}

void nube(float posX, float posY){
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glTranslated(posX,posY,-800);
	glRotatef(angulo*0.5,0,1,0);
	glBindTexture(GL_TEXTURE_2D,tnube.ID);		
	int c=175;
	glBegin(GL_QUADS);
	  glTexCoord2f(0,0); glVertex3d(-c,c,c);
	  glTexCoord2f(1,0); glVertex3d(c,c,c);
	  glTexCoord2f(1,1); glVertex3d(c,-c,c);
	  glTexCoord2f(0,1); glVertex3d(-c,-c,c);
	  
	  glTexCoord2f(0,0); glVertex3d(c,c,c);
	  glTexCoord2f(1,0); glVertex3d(c,c,-c);
	  glTexCoord2f(1,1); glVertex3d(c,-c,-c);
	  glTexCoord2f(0,1); glVertex3d(c,-c,c);
	  
	  glTexCoord2f(0,0); glVertex3d(c,c,-c);
	  glTexCoord2f(1,0); glVertex3d(-c,c,-c);
	  glTexCoord2f(1,1); glVertex3d(-c,-c,-c);
	  glTexCoord2f(0,1); glVertex3d(c,-c,-c);
	  
	  glTexCoord2f(0,0); glVertex3d(-c,c,-c);
	  glTexCoord2f(1,0); glVertex3d(-c,c,c);
	  glTexCoord2f(1,1); glVertex3d(-c,-c,c);
	  glTexCoord2f(0,1); glVertex3d(-c,-c,-c);
	glEnd();
	glLoadIdentity();
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);	
}

void logo(){
	glDisable(GL_LIGHTING);
	//glDisable(GL_DEPTH_TEST);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_NOTEQUAL, 0);	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,tlogo.ID);
	glTranslatef(780,800,0);
	int z=200;
	glBegin(GL_QUADS);
		glTexCoord2f(0,0);glVertex3d(-z,z,-z);
		glTexCoord2f(1,0);glVertex3d(z,z,-z);
	  	glTexCoord2f(1,1);glVertex3d(z,-z,-z);
	  	glTexCoord2f(0,1);glVertex3d(-z,-z,-z);
	glEnd();
	glLoadIdentity();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_ALPHA_TEST);
	//glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);	
}

void display(){
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	fondo();
	logo();
	fondoverde();
	//nube derecha
	nube(770,110);
	//nube izquierda
	nube(-240,270);	
	textopuntaje();
	textomoneda();
	animacion();
	cubo_pregunta();
	animacion();
	yoshi();
	animacion();
	fantasma();
	animacion();
	moneda(-640,40);
	animacion();
	moneda(-490,40);
	animacion();
	moneda(-340,40);
	animacion();
	estrella();
	//plataforma de abajo
	animacion();
	cubo(-930,-930);
	animacion();
	cubo(-790,-930);
	animacion();
	cubo(-650,-930);
	animacion();
	cubo(-510,-930);
	animacion();
	cubo(-370,-930);
	animacion();
	cubo(-230,-930);
	animacion();
	cubo(-90,-930);
	animacion();
	cubo(190,-930);
	animacion();
	cubo(330,-930);
	animacion();
	cubo(470,-930);
	animacion();
	cubo(610,-930);
	animacion();
	cubo(750,-930);
	animacion();	
	cubo(890,-930);
	animacion();
	cubo(1030,-930);
	animacion();
	//plataforma de arriba, derecha
	cubo(190,-100);
	animacion();
	cubo(330,-100);
	animacion();
	cubo(470,-100);
	animacion();
	//plataforma de arriba, izquierda
	cubo(-640,-100);
	animacion();
	cubo(-500,-100);
	animacion();
	cubo(-360,-100);
	animacion();			
	//anillo(600);	
	tubo();
	animacion();
	carnivora();
	animacion();
	hongo();
	animacion();
	glTranslatef(bolaPosX,bolaPosY,bolaPosZ);
	personaje();	
	glLoadIdentity();
	glutSwapBuffers();
		
}

void reshape(int largo, int ancho){
	glViewport(0,0,(GLsizei)largo,(GLsizei)ancho);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	int t=1000;
	glOrtho(-t,t,-t,t,-t,t);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void keyboard( unsigned char key ,int x , int y ){
	switch(key){
		case ' ':
			mod_angX=0;
			mod_angY=0;
			mod_angZ=0;
			break;
		case 'a':
			 bolaPosX-=10;
			break;
		case 'd':
			 bolaPosX+=10;
			break;
		case 'w':
			 bolaPosY+=10;
			break;
		case 's':
			 bolaPosY-=10;
			break;				
	}
}

void keyboard_s(int key , int x, int y){
	switch(key){
		case GLUT_KEY_UP:
			mod_angX-=0.0005;
			break;
		case GLUT_KEY_DOWN:
			mod_angX+=0.0005;
			break;
		case GLUT_KEY_LEFT:
			mod_angY-=0.0005;
			break;
		case GLUT_KEY_RIGHT:
			mod_angY+=0.0005;
			break;
		case GLUT_KEY_HOME:
		    mod_angZ-=0.0005;
		    break;
		case GLUT_KEY_END:
		    mod_angZ+=0.0005;
			break;	
	}	
	
}

void idle(){
	angulo+=0.05;	
	display();
}

int main(int argc, char** argv) {
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(900,600);
	glutInitWindowPosition(100,100);
	glutCreateWindow("Examen Final");
	init();
	char soundfile[] = "C:\sonidoMario.wav";
    cout<<PlaySound((LPCSTR)soundfile,NULL,SND_FILENAME | SND_ASYNC | SND_LOOP);
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(keyboard_s);
	
	glutIdleFunc(idle);
	glutMainLoop();
	
	return 0;
}








