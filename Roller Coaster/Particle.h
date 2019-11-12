#pragma once

#include <QtOpenGL>  
#include <GL/GLU.h>
#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib") 
//#include "Utilities/ArcBallCam.H"
//#include "Utilities/3DUtils.H"


typedef struct tag_PARTICLE
{
	GLfloat xpos;//(xpos,ypos,zpos)��particle��position
	GLfloat ypos;
	GLfloat zpos;
	GLfloat xspeed;//(xspeed,yspeed,zspeed)��particle��speed 
	GLfloat yspeed;
	GLfloat zspeed;
	GLfloat r;//(r,g,b)��particle��color
	GLfloat g;
	GLfloat b;
	GLfloat life;// particle���ةR 
	GLfloat fade;// particle���I��t��
	GLfloat size;// particle���j�p  
	GLbyte    bFire;
	GLbyte    nExpl;//����particle�ĪG  
	GLbyte    bAddParts;// particle�O�_�t������
	GLfloat   AddSpeed;//���ڲɤl���[�t��  
	GLfloat   AddCount;//���ڲɤl���W�[�q  
	tag_PARTICLE* pNext;//�U�@particle 
	tag_PARTICLE* pPrev;//�W�@particle   
} Particle, *pParticle;



void InitParticle(Particle& ep);
void AddParticle(Particle ex);
void ProcessParticles();
void Explosion1(Particle* par);
void DrawParticles();
void DeleteParticle(pParticle* p);
void DeleteAll(pParticle* Part);