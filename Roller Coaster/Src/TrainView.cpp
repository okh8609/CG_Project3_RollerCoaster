#include "TrainView.h"  


TrainView::TrainView(QWidget *parent) :
	QGLWidget(parent)
{

	resetArcball();

	this->m = new Model("C:/Users/KaiHao/Desktop/Computer Graphics/DGMM-Lab/P3/arrow.obj", 100, Point3d(0, 0, 0));
	//m = new Model("C:/Users/KaiHao/Desktop/實驗室/3D身體調變/3D model obj file/obj_2/20190624_064424_262Y1Q6D.obj", 100, Point3d(0, 0, 0));


	string path("C:/Users/KaiHao/Desktop/實驗室/3D身體調變/3D model obj file/obj_2/20190624_064424_262Y1Q6D.obj");
	this->model = MyObjLoader(path, 50, QVector3D(0, 35, 0));
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

	//畫人像OBJ
	glColor3f(205.0 / 255.0, 149.0 / 255.0, 124.0 / 255.0);
	glColor4f(0, 0.2, 0.5, 1);
	model.render();

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

	//畫粒子
	ProcessParticles();
	DrawParticles();

	//畫箭頭的OBJ
	glColor4f(0, 0.5, 0.5, 1);
	m->render(false, false);


	//畫四角椎當火車頭   	 
	drawTrain();
}

//************************************************************************
//
// * This sets up both the Projection and the ModelView matrices
//   HOWEVER: it doesn't clear the projection first (the caller handles
//   that) - its important for picking
//========================================================================
void TrainView::setProjection()
//========================================================================
{
	// Compute the aspect ratio (we'll need it)
	float aspect = static_cast<float>(width()) / static_cast<float>(height());

	//std::cout << static_cast<float>(width()) << " " << static_cast<float>(height()) << std::endl;

	// Check whether we use the world camp
	if (this->camera == 0) {
		//上帝視角

		arcball.setProjection(false);
		update();
		// Or we use the top cam
	}
	else if (this->camera == 1) {
		//普通視角

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
		//火車視角 (未完成!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!)
		//火車視角 (未完成!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!)
		//火車視角 (未完成!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!)
		//火車視角 (未完成!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!)
		//火車視角 (未完成!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!)


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
	else
	{
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
	// 畫軌道


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

	//vector<Point3f> trackMiddle; //鐵軌的中間軌道點
	//vector<Point3f> trackLeft; //雙軌的左邊軌道點
	//vector<Point3f> trackRight; //雙軌的右邊軌道點

	this->arcLength = 0; //總長度先設為0


	//畫軌道線
	//曲線型態  0:"Linear"、1:"Cardinal"、2:"Cubic"
	if (curve == 0) //軌道是線性的
	{
		for (size_t i = 0; i < m_pTrack->points.size(); ++i)
		{
			// 控制點的位置
			Point3f _p1 = m_pTrack->points[i].pos;
			Point3f _p2 = m_pTrack->points[(i + 1) % m_pTrack->points.size()].pos;
			QVector3D p1(_p1.x, _p1.y, _p1.z);
			QVector3D p2(_p2.x, _p2.y, _p2.z);
			QVector3D p1_p2(p2 - p1); //p1指向p2的向量
			this->arcLength += p1_p2.length(); //加長度進去
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

			//軌道型態  0:"Line"  1:"Track"  2:"Road"
			if (track == 0) //單一條線的軌道
			{
				glLineWidth(3);
				glColor3ub(32, 32, 64);
				glBegin(GL_LINES);
				glVertexQVector3D(p1);
				glVertexQVector3D(p2);
				glEnd();
			}
			else if (track == 1) //兩條線的軌道
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
			else if (track == 2) //鋪設軌道平面
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
	else if (curve == 1) // Cardinal
	{
		const int NumIntegralDiv = 100; //Number of Integral Divide - 線段積分 總共切幾份
		float tens = 0.5; //tension 矩陣的參數 0~1
		QMatrix4x4 M(-1, 2, -1, 0,
			2 / tens - 1, 1 - 3 / tens, 0, 1 / tens,
			1 - 2 / tens, 3 / tens - 2, 1, 0,
			1, -1, 0, 0);


		for (size_t i = 0; i < m_pTrack->points.size(); ++i)
		{
			// 控制點的位置
			Point3f _p0 = m_pTrack->points[(i - 1) % m_pTrack->points.size()].pos;
			Point3f _p1 = m_pTrack->points[i].pos; //
			Point3f _p2 = m_pTrack->points[(i + 1) % m_pTrack->points.size()].pos; //
			Point3f _p3 = m_pTrack->points[(i + 2) % m_pTrack->points.size()].pos;
			QMatrix4x4 G(QMatrix4x4(_p0.x, _p0.y, _p0.z, 0,
				_p1.x, _p1.y, _p1.z, 0,
				_p2.x, _p2.y, _p2.z, 0,
				_p3.x, _p3.y, _p3.z, 0).transposed());
			QMatrix4x4 GxM = G * M;

			// 控制點指向的方向
			Point3f _d1 = m_pTrack->points[i].orient;
			Point3f _d2 = m_pTrack->points[(i + 1) % m_pTrack->points.size()].orient;
			QVector3D d1(_d1.x, _d1.y, _d1.z); d1.normalize();
			QVector3D d2(_d2.x, _d2.y, _d2.z); d2.normalize();

			//取各線段

			glLineWidth(3);
			glColor3ub(32, 32, 64);
			glBegin(GL_LINE_STRIP);
			for (size_t i = 0; i <= NumIntegralDiv; ++i)
			{
				float t = (float)i / (float)NumIntegralDiv; //整體進度 t = 0 ~ 1

				QVector4D T(t*t*t, t*t, t, 1);

				QVector4D _q = G * M * T;
				QVector3D Q(_q.x(), _q.y(), _q.z());

				glVertexQVector3D(Q);
			}
			glEnd();



			////算左右延伸的兩點
			//const int trackWidth = 3;
			//QVector3D v1 = p1 + QVector3D::crossProduct(p1_p2, d1) * trackWidth;
			//QVector3D v2 = p1 - QVector3D::crossProduct(p1_p2, d1) * trackWidth;
			//QVector3D v3 = p2 + QVector3D::crossProduct(p1_p2, d2) * trackWidth;
			//QVector3D v4 = p2 - QVector3D::crossProduct(p1_p2, d2) * trackWidth;

			////軌道型態  0:"Line"  1:"Track"  2:"Road"
			//if (track == 0) //單一條線的軌道
			//{
			//	glLineWidth(3);
			//	glColor3ub(32, 32, 64);
			//	glBegin(GL_LINES);
			//	glVertexQVector3D(p1);
			//	glVertexQVector3D(p2);
			//	glEnd();
			//}
			//else if (track == 1) //兩條線的軌道
			//{
			//	glLineWidth(3);
			//	glColor3ub(32, 32, 64);
			//	glBegin(GL_LINES);
			//	glVertexQVector3D(v1);
			//	glVertexQVector3D(v3);
			//	glVertexQVector3D(v2);
			//	glVertexQVector3D(v4);
			//	glEnd();
			//}
			//else if (track == 2) //鋪設軌道平面
			//{
			//	const float roadThickness = 0.35;

			//	glColor4f(0.05, 0.1, 0.3, 0.2);

			//	glBegin(GL_QUADS); //連續畫
			//	glVertexQVector3D(v1 + d1 * roadThickness);
			//	glVertexQVector3D(v2 + d1 * roadThickness);
			//	glVertexQVector3D(v4 + d2 * roadThickness);
			//	glVertexQVector3D(v3 + d2 * roadThickness);
			//	glVertexQVector3D(v3 - d2 * roadThickness);
			//	glVertexQVector3D(v4 - d2 * roadThickness);
			//	glVertexQVector3D(v2 - d1 * roadThickness);
			//	glVertexQVector3D(v1 - d1 * roadThickness);
			//	glVertexQVector3D(v1 + d1 * roadThickness);
			//	glVertexQVector3D(v2 + d1 * roadThickness);
			//	glEnd();

			//	glBegin(GL_QUADS); //單獨畫
			//	glVertexQVector3D(v1 + d1 * roadThickness);
			//	glVertexQVector3D(v1 - d1 * roadThickness);
			//	glVertexQVector3D(v3 - d2 * roadThickness);
			//	glVertexQVector3D(v3 + d2 * roadThickness);

			//	glVertexQVector3D(v2 + d1 * roadThickness);
			//	glVertexQVector3D(v2 - d1 * roadThickness);
			//	glVertexQVector3D(v4 - d2 * roadThickness);
			//	glVertexQVector3D(v4 + d2 * roadThickness);
			//	glEnd();
			//}
		}
	}
	else if (curve == 2) // Cubic
	{
		for (size_t i = 0; i < m_pTrack->points.size(); ++i)
		{

		}
	}

	// 鋪設木棧道
	{





	}



	/* 助教給的垃圾code

	DIVIDE_LINE = 100;
	t_time = 0.5;


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

		// 一些咚咚
		QVector3D qt, qt0, qt1, orient_t;

		glLineWidth(4);
		//glBegin(GL_LINES);
		float percent = 1.0f / DIVIDE_LINE;
		float t = 0;


		if (curve == 0) //曲線型態  0:"Linear"、1:"Cardinal"、2:"Cubic"
		{
			qt = (1 - t) * p1 + t * p2;
		}
		else if (curve == 1)
		{

		}
		else if (curve == 2)
		{

		}

		for (size_t j = 0; j < DIVIDE_LINE; j++) {
			qt0 = qt;
			if (curve == 0) //曲線型態  0:"Linear"、1:"Cardinal"、2:"Cubic"
			{
				orient_t = (1 - t) * d1 + t * d2;
			}
			else if (curve == 1)
			{

			}
			else if (curve == 2)
			{

			}
			t += percent;
			if (curve == 0) //曲線型態  0:"Linear"、1:"Cardinal"、2:"Cubic"
			{
				qt = (1 - t) * p1 + t * p2;
			}
			else if (curve == 1)
			{


			}
			else if (curve == 2)
			{

			}
			qt1 = qt;
		}

		glLineWidth(3);
		glBegin(GL_LINES);
		if (!doingShadows) {
			glColor3ub(32, 32, 64);
		}
		glVertex3f(qt0.x(), qt0.y(), qt0.z());
		glVertex3f(qt1.x(), qt1.y(), qt1.z());

		glEnd();
		glLineWidth(1);
	}

	*/




}

//void TrainView::drawTrain(QVector3D trainPos, QVector3D trainUp, QVector3D trainDir)
void TrainView::drawTrain()
{
	//我自己定義的畫四角椎的function

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
	glVertexQVector3D(trainPos + trainUp * trainSize + trainLeft * trainSize);
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

void TrainView::doPick(int mx, int my)
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
