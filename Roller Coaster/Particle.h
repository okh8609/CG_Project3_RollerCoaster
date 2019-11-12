#pragma once

#include <QtOpenGL>  
#include <GL/GLU.h>
#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib") 
//#include "Utilities/ArcBallCam.H"
//#include "Utilities/3DUtils.H"


typedef struct tag_PARTICLE
{
	GLfloat xpos;//(xpos,ypos,zpos)為particle的position
	GLfloat ypos;
	GLfloat zpos;
	GLfloat xspeed;//(xspeed,yspeed,zspeed)為particle的speed 
	GLfloat yspeed;
	GLfloat zspeed;
	GLfloat r;//(r,g,b)為particle的color
	GLfloat g;
	GLfloat b;
	GLfloat life;// particle的壽命 
	GLfloat fade;// particle的衰減速度
	GLfloat size;// particle的大小  
	GLbyte    bFire;
	GLbyte    nExpl;//哪種particle效果  
	GLbyte    bAddParts;// particle是否含有尾巴
	GLfloat   AddSpeed;//尾巴粒子的加速度  
	GLfloat   AddCount;//尾巴粒子的增加量  
	tag_PARTICLE* pNext;//下一particle 
	tag_PARTICLE* pPrev;//上一particle   
} Particle, *pParticle;



void InitParticle(Particle& ep);
void AddParticle(Particle ex);
void ProcessParticles();
void Explosion1(Particle* par);
void DrawParticles();
void DeleteParticle(pParticle* p);
void DeleteAll(pParticle* Part);