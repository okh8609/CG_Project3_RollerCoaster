﻿#include "Particle.h"

Particle * Particles;
UINT nOfFires = 0;

UINT Tick1 = 0, Tick2 = 0;
float DTick;
GLfloat grav = 0.00003; //(0.00003f;)


#define MAX_PARTICLES 100
#define MAX_FIRES 5  //最多幾盞煙火


void InitParticle(Particle& ep)
{
	ep.b = float(rand() % 100) / 60.0f;//顏色隨機
	ep.g = float(rand() % 100) / 60.0f;
	ep.r = float(rand() % 100) / 60.0f;
	ep.life = 1.0f;//初始壽命
	ep.fade = 0.005f + float(rand() % 21) / 10000.0f;//衰减速度
	ep.size = 1;//大小  

	//位置 
	ep.xpos = 75.0f - rand() % 150;
	ep.ypos = 25.0f;
	ep.zpos = 75.0f - rand() % 150;

	if (!int(ep.xpos))//x方向速度(z方向相同)
	{
		ep.xspeed = 0.0f;
	}
	else
	{
		if (ep.xpos < 0)
		{
			ep.xspeed = (rand() % int(-ep.xpos)) / 1500.0f;
		}
		else
		{
			ep.xspeed = -(rand() % int(-ep.xpos)) / 1500.0f;
		}
	}
	ep.yspeed = 0.04f + float(rand() % 11) / 1000.0f;//y方向速度(向上)

	ep.bFire = 1;
	ep.nExpl = 1 + rand() % 7;//粒子效果  
	ep.bAddParts = 1;//設定有尾巴 
	ep.AddCount = 0.0f;
	ep.AddSpeed = 0.2f;
	nOfFires++;//粒子數+1 
	AddParticle(ep);//加入粒子列表    
}

void AddParticle(Particle ex)
{
	Particle * p;
	p = new Particle;//new particle   
	p->pNext = NULL;
	p->pPrev = NULL;
	p->b = ex.b;  p->g = ex.g;  p->r = ex.r;
	p->fade = ex.fade;
	p->life = ex.life;
	p->size = ex.size;
	p->xpos = ex.xpos;
	p->ypos = ex.ypos;
	p->zpos = ex.zpos;
	p->xspeed = ex.xspeed;
	p->yspeed = ex.yspeed;
	p->zspeed = ex.zspeed;
	p->AddCount = ex.AddCount;
	p->AddSpeed = ex.AddSpeed;
	p->bAddParts = ex.bAddParts;
	p->bFire = ex.bFire;
	p->nExpl = ex.nExpl;

	if (!Particles)//當目前的Particle列表為空時
	{
		Particles = p;
		return;
	}
	else
	{
		Particles->pPrev = p;//插入Particle
		p->pNext = Particles;
		Particles = p;
	}
}

void DeleteParticle(Particle ** p)
{
	if (!(*p))//假如Particle列表為空
		return;
	if (!(*p)->pNext && !(*p)->pPrev)//假如只有一個Particle，直接删除  
	{
		delete (*p);
		*p = NULL;
		Particles = *p; //!!!!!
		return;
	}
	Particle * tmp;
	if (!(*p)->pPrev)//假如是首節點  
	{
		tmp = (*p);
		*p = (*p)->pNext;
		Particles = *p;
		(*p)->pPrev = NULL;
		delete tmp;
		return;
	}

	if (!(*p)->pNext)//末節點  
	{
		(*p)->pPrev->pNext = NULL;
		delete (*p);
		*p = NULL;
		return;
	}
	else
	{
		//一般情况  
		tmp = (*p);
		(*p)->pPrev->pNext = (*p)->pNext;
		(*p)->pNext->pPrev = (*p)->pPrev;
		*p = (*p)->pNext;
		delete tmp;
	}
}


void Explosion1(Particle* par)
{
	Particle ep;
	for (int i = 0; i < 75; i++)
	{
		ep.b = float(rand() % 100) / 60.0f;
		ep.g = float(rand() % 100) / 60.0f;
		ep.r = float(rand() % 100) / 60.0f;
		ep.life = 1.0f;
		ep.fade = 0.01f + float(rand() % 31) / 10000.0f;
		ep.size = 0.8f;
		ep.xpos = par->xpos;
		ep.ypos = par->ypos;
		ep.zpos = par->zpos;
		ep.xspeed = 0.02f - float(rand() % 41) / 1000.0f;
		ep.yspeed = 0.02f - float(rand() % 41) / 1000.0f;
		ep.zspeed = 0.02f - float(rand() % 41) / 1000.0f;
		ep.bFire = 0;
		ep.nExpl = 0;
		ep.bAddParts = 0;
		ep.AddCount = 0.0f;
		ep.AddSpeed = 0.0f;
		AddParticle(ep);
	}
}

void Explosion2(Particle* par)
{
	Particle ep;
	for (int i = 0; i < 100; i++)
	{
		ep.b = par->b;
		ep.g = par->g;
		ep.r = par->r;
		ep.life = 1.0f;
		ep.fade = 0.01f + float(rand() % 31) / 10000.0f;
		ep.size = 0.8f;
		ep.xpos = par->xpos;
		ep.ypos = par->ypos;
		ep.zpos = par->zpos;
		ep.xspeed = 0.02f - float(rand() % 41) / 1000.0f;
		ep.yspeed = 0.02f - float(rand() % 41) / 1000.0f;
		ep.zspeed = 0.02f - float(rand() % 41) / 1000.0f;
		ep.bFire = 0;
		ep.nExpl = 0;
		ep.bAddParts = 0;
		ep.AddCount = 0.0f;
		ep.AddSpeed = 0.0f;
		AddParticle(ep);
	}
}

void Explosion3(Particle*par)
{
	Particle ep;
	float PIAsp = 3.1415926 / 180;
	for (int i = 0; i < 30; i++) {
		float angle = float(rand() % 360)*PIAsp;
		ep.b = par->b;
		ep.g = par->g;
		ep.r = par->r;
		ep.life = 1.5f;
		ep.fade = 0.01f + float(rand() % 31) / 10000.0f;
		ep.size = 0.8f;
		ep.xpos = par->xpos;
		ep.ypos = par->ypos;
		ep.zpos = par->zpos;
		ep.xspeed = (float)sin(angle)*0.01f;
		ep.yspeed = 0.01f + float(rand() % 11) / 1000.0f;
		ep.zspeed = (float)cos(angle)*0.01f;
		ep.bFire = 0;
		ep.nExpl = 0;
		ep.bAddParts = 1;
		ep.AddCount = 0.0f;
		ep.AddSpeed = 0.2f;
		AddParticle(ep);
	}
}

void Explosion4(Particle* par) {
	Particle ep;
	float PIAsp = 3.1415926 / 180;
	for (int i = 0; i < 30; i++) {
		float angle = float(rand() % 360)*PIAsp;
		ep.b = float(rand() % 100) / 60.0f;
		ep.g = float(rand() % 100) / 60.0f;
		ep.r = float(rand() % 100) / 60.0f;
		ep.life = 1.5f;
		ep.fade = 0.01f + float(rand() % 31) / 10000.0f;
		ep.size = 0.8f;
		ep.xpos = par->xpos;
		ep.ypos = par->ypos;
		ep.zpos = par->zpos;
		ep.xspeed = (float)sin(angle)*0.01f;
		ep.yspeed = 0.01f + float(rand() % 11) / 1000.0f;
		ep.zspeed = (float)cos(angle)*0.01f;
		ep.bFire = 0;
		ep.nExpl = 0;
		ep.bAddParts = 1;
		ep.AddCount = 0.0f;
		ep.AddSpeed = 0.2f;
		AddParticle(ep);
	}
}

void Explosion5(Particle* par) {
	Particle ep;
	for (int i = 0; i < 30; i++) {
		ep.b = par->b;
		ep.g = par->g;
		ep.r = par->r;
		ep.life = 0.8f;
		ep.fade = 0.01f + float(rand() % 31) / 10000.0f;
		ep.size = 0.8f;
		ep.xpos = par->xpos;
		ep.ypos = par->ypos;
		ep.zpos = par->zpos;
		ep.xspeed = 0.01f - float(rand() % 21) / 1000.0f;
		ep.yspeed = 0.01f - float(rand() % 21) / 1000.0f;
		ep.zspeed = 0.01f - float(rand() % 21) / 1000.0f;
		ep.bFire = 0;
		ep.nExpl = 7;
		ep.bAddParts = 0;
		ep.AddCount = 0.0f;
		ep.AddSpeed = 0.0f;
		AddParticle(ep);
	}
}

void Explosion6(Particle* par) {
	Particle ep;
	for (int i = 0; i < 50; i++) {
		ep.b = float(rand() % 100) / 60.0f;
		ep.g = float(rand() % 100) / 60.0f;
		ep.r = float(rand() % 100) / 60.0f;
		ep.life = 0.8f;
		ep.fade = 0.01f + float(rand() % 31) / 10000.0f;
		ep.size = 0.8f;
		ep.xpos = par->xpos;
		ep.ypos = par->ypos;
		ep.zpos = par->zpos;
		ep.xspeed = 0.01f - float(rand() % 21) / 1000.0f;
		ep.yspeed = 0.01f - float(rand() % 21) / 1000.0f;
		ep.zspeed = 0.01f - float(rand() % 21) / 1000.0f;
		ep.bFire = 0;
		ep.nExpl = 7;
		ep.bAddParts = 0;
		ep.AddCount = 0.0f;
		ep.AddSpeed = 0.0f;
		AddParticle(ep);
	}
}

void Explosion7(Particle* par) {
	Particle ep;
	for (int i = 0; i < 20; i++) {
		ep.b = par->b;
		ep.g = par->g;
		ep.r = par->r;
		ep.life = 0.5f;
		ep.fade = 0.01f + float(rand() % 31) / 10000.0f;
		ep.size = 0.6f;
		ep.xpos = par->xpos;
		ep.ypos = par->ypos;
		ep.zpos = par->zpos;
		ep.xspeed = 0.02f - float(rand() % 41) / 1000.0f;
		ep.yspeed = 0.02f - float(rand() % 41) / 1000.0f;
		ep.zspeed = 0.02f - float(rand() % 41) / 1000.0f;
		ep.bFire = 0;
		ep.nExpl = 0;
		ep.bAddParts = 0;
		ep.AddCount = 0.0f;
		ep.AddSpeed = 0.0f;
		AddParticle(ep);
	}
}


void ProcessParticles()
{
	Tick1 = Tick2;
	Tick2 = GetTickCount();
	DTick = float(Tick2 - Tick1);
	DTick *= 0.5f;
	Particle ep;
	if (nOfFires < MAX_FIRES)
	{
		InitParticle(ep);
	}
	Particle * par;
	par = Particles; //迭代 link list 先把頭取出來

	while (par) {
		par->life -= par->fade*(float(DTick)*0.1f);//Particle壽命衰減 
		if (par->life <= 0.05f) {//當壽命小於一定值 

			//爆炸效果
			switch (par->nExpl) {
			case 1:
				Explosion1(par);
				break;
			case 2:
				Explosion2(par);
				break;
			case 3:
				Explosion3(par);
				break;
			case 4:
				Explosion4(par);
				break;
			case 5:
				Explosion5(par);
				break;
			case 6:
				Explosion6(par);
				break;
			case 7:
				Explosion7(par);
				break;
			default:
				break;
			}

			if (par->bFire)
				nOfFires--;
			DeleteParticle(&par);
		}
		else {
			par->xpos += par->xspeed*DTick;
			par->ypos += par->yspeed*DTick;
			par->zpos += par->zspeed*DTick;
			par->yspeed -= grav * DTick;
			if (par->bAddParts) {//假如有尾巴  
				par->AddCount += 0.01f*DTick;//AddCount變化愈慢，尾巴粒子愈小  
				if (par->AddCount > par->AddSpeed) {//AddSpeed愈大，尾巴粒子愈小  
					par->AddCount = 0;
					ep.b = par->b;  ep.g = par->g;  ep.r = par->r;
					ep.life = par->life*0.9f;//壽命變短  
					ep.fade = par->fade*7.0f;//衰减快一些  
					ep.size = 0.6f;//粒子尺寸小一些  
					ep.xpos = par->xpos;
					ep.ypos = par->ypos;
					ep.zpos = par->zpos;
					ep.xspeed = 0.0f;
					ep.yspeed = 0.0f;
					ep.zspeed = 0.0f;
					ep.bFire = 0;
					ep.nExpl = 0;
					ep.bAddParts = 0;//尾巴粒子没有尾巴  
					ep.AddCount = 0.0f;
					ep.AddSpeed = 0.0f;
					AddParticle(ep);
				}
			}   par = par->pNext;//更新下一粒子   
		}
	}
}

void DrawParticles() {
	Particle * par;
	par = Particles;
	while (par)
	{
		glColor4f(par->r, par->g, par->b, par->life);
		glBegin(GL_TRIANGLE_STRIP);
		glTexCoord2d(1, 1);
		glVertex3f(par->xpos + par->size, par->ypos + par->size, par->zpos);
		glTexCoord2d(0, 1);
		glVertex3f(par->xpos - par->size, par->ypos + par->size, par->zpos);
		glTexCoord2d(1, 0);
		glVertex3f(par->xpos + par->size, par->ypos - par->size, par->zpos);
		glTexCoord2d(0, 0);
		glVertex3f(par->xpos - par->size, par->ypos - par->size, par->zpos);
		glEnd();
		par = par->pNext;
	}
}
