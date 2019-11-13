﻿#include "TrainView.h"  


TrainView::TrainView(QWidget *parent) :
	QGLWidget(parent)
{

	resetArcball();

	m = new Model("C:/Users/KaiHao/Desktop/Computer Graphics/DGMM-Lab/P3/arrow.obj", 100, Point3d(0, 0, 0));

	//string path_02("C:/Users/KaiHao/Desktop/實驗室/3D身體調變/3D model obj file/obj_2/20190624_064424_262Y1Q6D.obj");
	//MyObjLoader model_02(path_02, 9, QVector3D(-1.5, 1, 0));
	//model = model_02;
}
TrainView::~TrainView()
{}
void TrainView::initializeGL()
{
	initializeOpenGLFunctions();
	//Create a triangle object
	triangle = new Triangle();
	//Initialize the triangle object
	triangle->Init();
	//Create a square object
	square = new Square();
	//Initialize the square object
	square->Init();
	//Initialize texture 
	initializeTexture();

}
void TrainView::initializeTexture()
{
	//Load and create a texture for square;'stexture
	QOpenGLTexture* texture = new QOpenGLTexture(QImage("./Textures/Tupi.bmp"));
	Textures.push_back(texture);
}

inline void TrainView::glVertexQVector3D(QVector3D v)
{
	glVertex3f(v.x(), v.y(), v.z());
}

void TrainView::resetArcball()
//========================================================================
{
	// Set up the camera to look at the world
	// these parameters might seem magical, and they kindof are
	// a little trial and error goes a long way
	arcball.setup(this, 40, 250, .2f, .4f, 0);
}

void TrainView::paintGL()
{


	//*********************************************************************
	//
	// * Set up basic opengl informaiton
	//
	//**********************************************************************
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	// Set up the view port
	glViewport(0, 0, width(), height());
	// clear the window, be sure to clear the Z-Buffer too
	glClearColor(0, 0, 0.3f, 0);

	// we need to clear out the stencil buffer since we'll use
	// it for shadows
	glClearStencil(0);
	glEnable(GL_DEPTH);

	// Blayne prefers GL_DIFFUSE
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

	// prepare for projection
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	setProjection();		// put the code to set up matrices here

	//######################################################################
	// TODO: 
	// you might want to set the lighting up differently. if you do, 
	// we need to set up the lights AFTER setting up the projection
	//######################################################################
	// enable the lighting
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	// top view only needs one light
	if (this->camera == 1) {
		glDisable(GL_LIGHT1);
		glDisable(GL_LIGHT2);
	}
	else {
		glEnable(GL_LIGHT1);
		glEnable(GL_LIGHT2);
	}

	//*********************************************************************
	//
	// * set the light parameters
	//
	//**********************************************************************
	GLfloat lightPosition1[] = { 0,1,1,0 }; // {50, 200.0, 50, 1.0};
	GLfloat lightPosition2[] = { 1, 0, 0, 0 };
	GLfloat lightPosition3[] = { 0, -1, 0, 0 };
	GLfloat yellowLight[] = { 0.5f, 0.5f, .1f, 1.0 };
	GLfloat whiteLight[] = { 1.0f, 1.0f, 1.0f, 1.0 };
	GLfloat blueLight[] = { .1f,.1f,.3f,1.0 };
	GLfloat grayLight[] = { .3f, .3f, .3f, 1.0 };

	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition1);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, whiteLight);
	glLightfv(GL_LIGHT0, GL_AMBIENT, grayLight);

	glLightfv(GL_LIGHT1, GL_POSITION, lightPosition2);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, yellowLight);

	glLightfv(GL_LIGHT2, GL_POSITION, lightPosition3);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, blueLight);



	//*********************************************************************
	// now draw the ground plane
	//*********************************************************************
	setupFloor();
	glDisable(GL_LIGHTING);
	drawFloor(200, 10);


	//*********************************************************************
	// now draw the object and we need to do it twice
	// once for real, and then once for shadows
	//*********************************************************************
	glEnable(GL_LIGHTING);
	setupObjects();

	drawStuff();

	// this time drawing is for shadows (except for top view)
	if (this->camera != 1) {
		setupShadows();
		drawStuff(true);
		unsetupShadows();
	}

	//Get modelview matrix
	glGetFloatv(GL_MODELVIEW_MATRIX, ModelViewMatrex);
	//Get projection matrix
	glGetFloatv(GL_PROJECTION_MATRIX, ProjectionMatrex);

	//Call triangle's render function, pass ModelViewMatrex and ProjectionMatrex
	triangle->Paint(ProjectionMatrex, ModelViewMatrex);

	//we manage textures by Trainview class, so we modify square's render function
	square->Begin();
	//Active Texture
	glActiveTexture(GL_TEXTURE0);
	//Bind square's texture
	Textures[0]->bind();
	//pass texture to shader
	square->shaderProgram->setUniformValue("Texture", 0);
	//Call square's render function, pass ModelViewMatrex and ProjectionMatrex
	square->Paint(ProjectionMatrex, ModelViewMatrex);
	square->End();




	ProcessParticles();
	DrawParticles();

	glColor4f(1, 0, 0, 1);
	m->render(false, false);

	//model.render();

	drawTrain();
}

//************************************************************************
//
// * This sets up both the Projection and the ModelView matrices
//   HOWEVER: it doesn't clear the projection first (the caller handles
//   that) - its important for picking
//========================================================================
void TrainView::
setProjection()
//========================================================================
{
	// Compute the aspect ratio (we'll need it)
	float aspect = static_cast<float>(width()) / static_cast<float>(height());

	//std::cout << static_cast<float>(width()) << " " << static_cast<float>(height()) << std::endl;

	// Check whether we use the world camp
	if (this->camera == 0) {
		arcball.setProjection(false);
		update();
		// Or we use the top cam
	}
	else if (this->camera == 1) {
		float wi, he;
		if (aspect >= 1) {
			//wi = 110;
			wi = 300;
			he = wi / aspect;
		}
		//else {
		//	he = 110;
		//	wi = he * aspect;
		//}

		// Set up the top camera drop mode to be orthogonal and set
		// up proper projection matrix
		glMatrixMode(GL_PROJECTION);
		glOrtho(-wi, wi, -he, he, 200, -200);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glRotatef(-90, 1, 0, 0);
		update();
	}
	// Or do the train view or other view here
	//####################################################################
	// TODO: 
	// put code for train view projection here!	
	else if (this->camera == 2) {
		float eyeX = 15, eyeY = 15, eyeZ = 15;

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(60, aspect, .1, 1000);

		// Put the camera where we want it to be
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glTranslatef(-eyeX, -eyeY, -eyeZ);
		//glRotatef(-45, 1, 0, 0);
		glRotatef(-45, 0, 1, 0); //向左看45度
		//glRotatef(-45, 0, 1, 0); //向左看45度


		update();
	}
	//####################################################################
	else {
#ifdef EXAMPLE_SOLUTION
		trainCamView(this, aspect);
#endif
		update();
	}
}

//************************************************************************
//
// * this draws all of the stuff in the world
//
//	NOTE: if you're drawing shadows, DO NOT set colors (otherwise, you get 
//       colored shadows). this gets called twice per draw 
//       -- once for the objects, once for the shadows
//########################################################################
// TODO: 
// if you have other objects in the world, make sure to draw them
//########################################################################
//========================================================================
void TrainView::drawStuff(bool doingShadows)
{
	// Draw the control points
	// don't draw the control points if you're driving 
	// (otherwise you get sea-sick as you drive through them)
	if (this->camera != 2) {
		for (size_t i = 0; i < this->m_pTrack->points.size(); ++i) {
			if (!doingShadows) {
				if (((int)i) != selectedCube)
					glColor3ub(240, 60, 60);
				else
					glColor3ub(240, 240, 30);
			}
			this->m_pTrack->points[i].draw();
		}
		update();
	}
	// draw the track
	//####################################################################
	// TODO: 
	// call your own track drawing code
	//####################################################################
/*
	//float t_time;
	unsigned int DIVIDE_LINE = 2; //有幾條線


	for (size_t i = 0; i < m_pTrack->points.size(); ++i)
	{
		// 位置
		Point3f cp_pos_p1 = m_pTrack->points[i].pos;
		Point3f cp_pos_p2 = m_pTrack->points[(i + 1) % m_pTrack->points.size()].pos;
		// 方向
		Point3f cp_orient_p1 = m_pTrack->points[i].orient;
		Point3f cp_orient_p2 = m_pTrack->points[(i + 1) % m_pTrack->points.size()].orient;

		Point3f qt, qt0, qt1, orient_t;

		float percent = 1.0f / DIVIDE_LINE;
		float t = 0;

		switch (curve)
		{
		case 0: // "Linear"
			qt = (1 - t) * cp_pos_p1 + t * cp_pos_p2;
			break;
		}

		for (size_t j = 0; j < DIVIDE_LINE; j++) {
			qt0 = qt;

			switch (curve) {
			case  0: // "Linear"
				orient_t = (1 - t) * cp_orient_p1 + t * cp_orient_p2;
				break;
			}

			t += percent;

			switch (curve) {
			case 0: // "Linear"
				qt = (1 - t) * cp_pos_p1 + t * cp_pos_p2;
				break;
			}
			qt1 = qt;
		}

		glLineWidth(3);
		glBegin(GL_LINES);
		if (!doingShadows) {
			glColor3ub(32, 32, 64);
		}
		glVertex3f(qt0.x, qt0.y, qt0.z);
		glVertex3f(qt1.x, qt1.y, qt1.z);

		glEnd();
		glLineWidth(1);

	}
*/


#ifdef EXAMPLE_SOLUTION
	drawTrack(this, doingShadows);
#endif

	// draw the train
	//####################################################################
	// TODO: 
	//	call your own train drawing code
	//####################################################################



#ifdef EXAMPLE_SOLUTION
	// don't draw the train if you're looking out the front window
	if (!tw->trainCam->value())
		drawTrain(this, doingShadows);
#endif

	//####################################################################
	// TODO: 
	//	KaiHao's Code
	//####################################################################

	vector<Point3f> trackMiddle; //鐵軌的中間軌道點
	vector<Point3f> trackLeft; //雙軌的左邊軌道點
	vector<Point3f> trackRight; //雙軌的右邊軌道點

	if (curve == 0) // "Linear"
	{
		for (size_t i = 0; i < m_pTrack->points.size(); ++i)
		{
			// 控制點的位置
			Point3f _p1 = m_pTrack->points[i].pos;
			Point3f _p2 = m_pTrack->points[(i + 1) % m_pTrack->points.size()].pos;
			QVector3D p1(_p1.x, _p1.y, _p1.z);
			QVector3D p2(_p2.x, _p2.y, _p2.z);
			QVector3D p1_p2(p2 - p1); //p1指向p2的向量
			p1_p2.normalize();

			// 控制點指向的方向
			Point3f _d1 = m_pTrack->points[i].orient;
			Point3f _d2 = m_pTrack->points[(i + 1) % m_pTrack->points.size()].orient;
			QVector3D d1(_d1.x, _d1.y, _d1.z); d1.normalize();
			QVector3D d2(_d2.x, _d2.y, _d2.z); d2.normalize();

			//算左右延伸的兩點
			const int trackWidth = 3;
			QVector3D v1 = p1 + QVector3D::crossProduct(p1_p2, d1) * trackWidth;
			QVector3D v2 = p1 - QVector3D::crossProduct(p1_p2, d1) * trackWidth;
			QVector3D v3 = p2 + QVector3D::crossProduct(p1_p2, d2) * trackWidth;
			QVector3D v4 = p2 - QVector3D::crossProduct(p1_p2, d2) * trackWidth;


			if (track == 0)
			{
				glLineWidth(3);
				glColor3ub(32, 32, 64);
				glBegin(GL_LINES);
				glVertexQVector3D(p1);
				glVertexQVector3D(p2);
				glEnd();
			}
			else if (track == 1)
			{
				glLineWidth(3);
				glColor3ub(32, 32, 64);
				glBegin(GL_LINES);
				glVertexQVector3D(v1);
				glVertexQVector3D(v3);
				glVertexQVector3D(v2);
				glVertexQVector3D(v4);
				glEnd();
			}
			else if (track == 2)
			{
				const float roadThickness = 0.35;

				glColor4f(0.05, 0.1, 0.3, 0.2);

				glBegin(GL_QUADS); //連續畫
				glVertexQVector3D(v1 + d1 * roadThickness);
				glVertexQVector3D(v2 + d1 * roadThickness);
				glVertexQVector3D(v4 + d2 * roadThickness);
				glVertexQVector3D(v3 + d2 * roadThickness);
				glVertexQVector3D(v3 - d2 * roadThickness);
				glVertexQVector3D(v4 - d2 * roadThickness);
				glVertexQVector3D(v2 - d1 * roadThickness);
				glVertexQVector3D(v1 - d1 * roadThickness);
				glVertexQVector3D(v1 + d1 * roadThickness);
				glVertexQVector3D(v2 + d1 * roadThickness);
				glEnd();

				glBegin(GL_QUADS); //單獨畫
				glVertexQVector3D(v1 + d1 * roadThickness);
				glVertexQVector3D(v1 - d1 * roadThickness);
				glVertexQVector3D(v3 - d2 * roadThickness);
				glVertexQVector3D(v3 + d2 * roadThickness);

				glVertexQVector3D(v2 + d1 * roadThickness);
				glVertexQVector3D(v2 - d1 * roadThickness);
				glVertexQVector3D(v4 - d2 * roadThickness);
				glVertexQVector3D(v4 + d2 * roadThickness);
				glEnd();
			}
		}
	}
}

//void TrainView::drawTrain(QVector3D trainPos, QVector3D trainUp, QVector3D trainDir)
void TrainView::drawTrain()
{
	//debug
	QVector3D trainPos(0, 0, 0); //火車的位置
	QVector3D trainUp(0, 1, 0);  //火車上方
	QVector3D trainDire(1, 0, 0);  //火車的前方
	QVector3D trainLeft(QVector3D::crossProduct(trainUp, trainDire));  //火車的左方
	////////

	const float trainSize = 5;
	
	trainUp.normalize();
	trainDire.normalize(); 
	trainLeft.normalize();
	
	trainPos += trainUp * trainSize;


	//畫四角錐作為車頭
	glColor3f(1, 1, 1);

	glBegin(GL_TRIANGLE_FAN);
	glVertexQVector3D(trainPos + trainDire * trainSize * 2);
	glVertexQVector3D(trainPos+ trainUp * trainSize + trainLeft * trainSize);
	glVertexQVector3D(trainPos + trainUp * trainSize - trainLeft * trainSize);
	glVertexQVector3D(trainPos - trainUp * trainSize - trainLeft * trainSize);
	glVertexQVector3D(trainPos - trainUp * trainSize + trainLeft * trainSize);
	glVertexQVector3D(trainPos + trainUp * trainSize + trainLeft * trainSize);
	glEnd();
	glBegin(GL_TRIANGLE_FAN);
	glVertexQVector3D(trainPos);
	glVertexQVector3D(trainPos + trainUp * trainSize + trainLeft * trainSize);
	glVertexQVector3D(trainPos + trainUp * trainSize - trainLeft * trainSize);
	glVertexQVector3D(trainPos - trainUp * trainSize - trainLeft * trainSize);
	glVertexQVector3D(trainPos - trainUp * trainSize + trainLeft * trainSize);
	glVertexQVector3D(trainPos + trainUp * trainSize + trainLeft * trainSize);
	glEnd();
}

void TrainView::
doPick(int mx, int my)
//========================================================================
{
	// since we'll need to do some GL stuff so we make this window as 
	// active window
	makeCurrent();

	// get the viewport - most reliable way to turn mouse coords into GL coords
	int viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPickMatrix((double)mx, (double)(viewport[3] - my),
		5, 5, viewport);

	// now set up the projection
	setProjection();

	// now draw the objects - but really only see what we hit
	GLuint buf[100];
	glSelectBuffer(100, buf);
	glRenderMode(GL_SELECT);
	glInitNames();
	glPushName(0);


	// draw the cubes, loading the names as we go
	for (size_t i = 0; i < m_pTrack->points.size(); ++i) {
		glLoadName((GLuint)(i + 1));
		m_pTrack->points[i].draw();
	}

	// go back to drawing mode, and see how picking did
	int hits = glRenderMode(GL_RENDER);
	if (hits) {
		// warning; this just grabs the first object hit - if there
		// are multiple objects, you really want to pick the closest
		// one - see the OpenGL manual 
		// remember: we load names that are one more than the index
		selectedCube = buf[3] - 1;
	}
	else // nothing hit, nothing selected
		selectedCube = -1;
}
