#include "TrainView.h"  


TrainView::TrainView(QWidget *parent) :
	QGLWidget(parent)
{

	resetArcball();

	this->m = new Model("C:/Users/KaiHao/Desktop/Computer Graphics/DGMM-Lab/P3/arrow.obj", 50, Point3d(0, 0, 0));
	//m = new Model("C:/Users/KaiHao/Desktop/實驗室/3D身體調變/3D model obj file/obj_2/20190624_064424_262Y1Q6D.obj", 100, Point3d(0, 0, 0));


	string path("C:/Users/KaiHao/Desktop/實驗室/3D身體調變/3D model obj file/obj_2/20190624_064424_262Y1Q6D.obj");
	this->model = MyObjLoader(path, 50, QVector3D(0, 35, 0));


	trainPos = QVector3D(50, 5, 0); //火車的位置
	trainUp = QVector3D(0, 1, 0);  //火車上方
	trainDire = QVector3D(1, 0, 1);  //火車的前方
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

inline void TrainView::glVertexQVector3D(const QVector3D& v)
{
	glVertex3f(v.x(), v.y(), v.z());
}

inline void TrainView::glNormalQVector3D(const QVector3D& v)
{
	glNormal3d(v.x(), v.y(), v.z());
}

void TrainView::drawBox(QVector3D pos, float size)
{
	glColor3f(0.8, 0.1, 0.1);

	size /= 2;

	//上
	glBegin(GL_QUADS);
	glNormal3d(0, 1, 0);
	glVertex3f(pos.x() + size, pos.y() + size, pos.z() + size);
	glVertex3f(pos.x() + size, pos.y() + size, pos.z() - size);
	glVertex3f(pos.x() - size, pos.y() + size, pos.z() - size);
	glVertex3f(pos.x() - size, pos.y() + size, pos.z() + size);
	glEnd();
	//下
	glBegin(GL_QUADS);
	glNormal3d(0, -1, 0);
	glVertex3f(pos.x() + size, pos.y() - size, pos.z() + size);
	glVertex3f(pos.x() + size, pos.y() - size, pos.z() - size);
	glVertex3f(pos.x() - size, pos.y() - size, pos.z() - size);
	glVertex3f(pos.x() - size, pos.y() - size, pos.z() + size);
	glEnd();
	//右
	glBegin(GL_QUADS);
	glNormal3d(1, 0, 0);
	glVertex3f(pos.x() + size, pos.y() + size, pos.z() + size);
	glVertex3f(pos.x() + size, pos.y() + size, pos.z() - size);
	glVertex3f(pos.x() + size, pos.y() - size, pos.z() - size);
	glVertex3f(pos.x() + size, pos.y() - size, pos.z() + size);
	glEnd();
	//左
	glBegin(GL_QUADS);
	glNormal3d(-1, 0, 0);
	glVertex3f(pos.x() - size, pos.y() + size, pos.z() + size);
	glVertex3f(pos.x() - size, pos.y() + size, pos.z() - size);
	glVertex3f(pos.x() - size, pos.y() - size, pos.z() - size);
	glVertex3f(pos.x() - size, pos.y() - size, pos.z() + size);
	glEnd();
	//前
	glBegin(GL_QUADS);
	glNormal3d(0, 0, 1);
	glVertex3f(pos.x() + size, pos.y() + size, pos.z() + size);
	glVertex3f(pos.x() + size, pos.y() - size, pos.z() + size);
	glVertex3f(pos.x() - size, pos.y() - size, pos.z() + size);
	glVertex3f(pos.x() - size, pos.y() + size, pos.z() + size);
	glEnd();
	//後
	glBegin(GL_QUADS);
	glNormal3d(0, 0, -1);
	glVertex3f(pos.x() + size, pos.y() + size, pos.z() - size);
	glVertex3f(pos.x() + size, pos.y() - size, pos.z() - size);
	glVertex3f(pos.x() - size, pos.y() - size, pos.z() - size);
	glVertex3f(pos.x() - size, pos.y() + size, pos.z() - size);
	glEnd();
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

#pragma region 火車跑跑跑
	trainNextPositionIndex %= trackMiddle.size();
	QVector3D nextPos = trackMiddle.at(trainNextPositionIndex);

	trainDire = (nextPos - trainPos).normalized();
	trainUp = QVector3D::crossProduct((trackRight.at(trainNextPositionIndex) - trackLeft.at(trainNextPositionIndex)), trainDire).normalized();
	if (this->camera != 2)
		drawTrain(trainPos, trainUp, trainDire);

	cout << trainSpeed << endl;
	//畫四角椎當火車頭 
	if (this->isrun)
	{
		if ((clock() - lastRedraw)*trainSpeed > CLOCKS_PER_SEC / 120)
		{
			lastRedraw = clock();

			float distance = (nextPos - trainPos).length(); //目前的點 走到下一個點的距離
			if (trainCurrSpacing - distance > 0) //走下一步，會跨過去
			{
				trainCurrSpacing -= distance;
				trainPos = nextPos;
				trainNextPositionIndex += 2;
			}
			else //走下一步，不會跨過去
			{
				trainPos += (nextPos - trainPos).normalized()*trainCurrSpacing;
				trainCurrSpacing = 1;
			}
		}
	}
#pragma endregion


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
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(120, aspect, .1, 1000);


		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		auto trainCent = trainPos + trainDire + trainUp * 3;
		auto trainPos2 = trainPos + trainUp * 3;

		gluLookAt(trainPos2.x(), trainPos2.y(), trainPos2.z(),
			trainCent.x(), trainCent.y(), trainCent.z(),
			trainUp.x(), trainUp.y(), trainUp.z());

		//float eyeX = 15, eyeY = 15, eyeZ = 15;


		//// Put the camera where we want it to be
		//glMatrixMode(GL_MODELVIEW);
		//glLoadIdentity();
		//glTranslatef(-eyeX, -eyeY, -eyeZ);
		////glRotatef(-45, 1, 0, 0);
		//glRotatef(-45, 0, 1, 0); //向左看45度
		////glRotatef(-45, 0, 1, 0); //向左看45度


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
	// Draw the control points
	// don't draw the control points if you're driving 
	// (otherwise you get sea-sick as you drive through them)
	bool ctp = true;
	if (ctp&&this->camera != 2) {
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
	//畫支撐柱
	glColor3ub(77, 28, 17);
	drawPillar();



	//####################################################################
	// TODO: 
	//	KaiHao's Code
	//####################################################################

	trackMiddle.clear(); //鐵軌的中間軌道點
	trackLeft.clear(); //雙軌的左邊軌道點
	trackRight.clear(); //雙軌的右邊軌道點

	this->arcLength = 0; //總長度先設為0

#pragma region 畫軌道線
	//曲線型態  0:"Linear"、1:"Cardinal"、2:"Cubic"
	QMatrix4x4 M;

	if (curve == 0) // Linear
	{
		M = QMatrix4x4(0, 0, 0, 0,
			0, 0, -1, 1,
			0, 0, 1, 0,
			0, 0, 0, 0);
	}
	else if (curve == 1) // Cardinal
	{

		M = QMatrix4x4(-1, 2, -1, 0,
			2 / tens - 1, 1 - 3 / tens, 0, 1 / tens,
			1 - 2 / tens, 3 / tens - 2, 1, 0,
			1, -1, 0, 0);
		M *= tens;
	}
	else if (curve == 2) // Cubic
	{
		M = QMatrix4x4(-1, 3, -3, 1,
			3, -6, 0, 4,
			-3, 3, 3, 1,
			1, 0, 0, 0);
		M /= 6.0;
	}

	for (size_t i = 0; i < m_pTrack->points.size(); ++i)
	{
		vector<QVector3D> subTrackMiddle; //鐵軌的中間軌道點

		// 控制點的位置
		Point3f _p0 = m_pTrack->points[(i - 1) % m_pTrack->points.size()].pos;
		Point3f _p1 = m_pTrack->points[i].pos; //
		Point3f _p2 = m_pTrack->points[(i + 1) % m_pTrack->points.size()].pos; //
		Point3f _p3 = m_pTrack->points[(i + 2) % m_pTrack->points.size()].pos;
		QMatrix4x4 G(_p0.x, _p1.x, _p2.x, _p3.x,
			_p0.y, _p1.y, _p2.y, _p3.y,
			_p0.z, _p1.z, _p2.z, _p3.z,
			0, 0, 0, 0);
		QMatrix4x4 GxM = G * M;

		// 線段 (中)
		QVector3D preQ(_p1.x, _p1.y, _p1.z); //---算總長度用的---

		for (size_t i = 0; i < NumIntegralDiv; ++i)  //  <=  ???
		{

			float t = (float)i / (float)NumIntegralDiv; //整體進度 t = 0 ~ 1
			QVector4D T(t*t*t, t*t, t, 1);

			QVector4D _q = G * M * T;
			QVector3D Q(_q.x(), _q.y(), _q.z());
			subTrackMiddle.push_back(Q);


			arcLength += (Q - preQ).length(); //---算總長度用的---
			preQ = Q; //---算總長度用的---
		}
		trackMiddle.insert(trackMiddle.end(), subTrackMiddle.begin(), subTrackMiddle.end());
	}

	// 線段 (左) (右)
	for (size_t i = 0; i < trackMiddle.size(); i++)
	{
		// 兩控制點指向的方向
		int di = i / NumIntegralDiv;
		Point3f _d1 = m_pTrack->points[di].orient;
		Point3f _d2 = m_pTrack->points[(di + 1) % m_pTrack->points.size()].orient;
		QVector3D d1(_d1.x, _d1.y, _d1.z); d1.normalize();
		QVector3D d2(_d2.x, _d2.y, _d2.z); d2.normalize();

		// 角度 沒特別講，就用線性的吧
		float t = (float)(i% NumIntegralDiv) / (float)(NumIntegralDiv);
		QVector3D dd = d1 + (d2 - d1) * t;
		dd.normalize();

		const QVector3D& pa = trackMiddle.at(i);
		const QVector3D& pb = trackMiddle.at((i + 1) % trackMiddle.size());
		QVector3D d_ab = pb - pa; //a向b的方向

		//算左延伸的點
		trackLeft.push_back(pa + QVector3D::crossProduct(dd, d_ab).normalized() * trackWidth);
		//算右延伸的點
		trackRight.push_back(pa - QVector3D::crossProduct(dd, d_ab).normalized() * trackWidth);

	}


	//軌道型態  0:"Line"  1:"Track"  2:"Road"
	if (track == 0) //單一條線的軌道
	{
		glLineWidth(3);
		//glColor3ub(32, 32, 64);
		glColor4f(0.05, 0.2, 0.3, 0);
		glBegin(GL_LINE_STRIP);
		for (auto p : trackMiddle)
			glVertexQVector3D(p);
		glVertexQVector3D(trackMiddle.front());
		glEnd();
	}
	else if (track == 1) //兩條線的軌道
	{
		glLineWidth(3);
		//glColor3ub(32, 32, 64);
		glColor4f(0.05, 0.1, 0.3, 0);

		glBegin(GL_LINE_STRIP);
		for (auto p : trackLeft)
			glVertexQVector3D(p);
		glVertexQVector3D(trackLeft.front());
		glEnd();

		//glColor4f(0.5, 0.1, 0.3, 0);

		glBegin(GL_LINE_STRIP);
		for (auto p : trackRight)
			glVertexQVector3D(p);
		glVertexQVector3D(trackRight.front());
		glEnd();
	}
	else if (track == 2) //鋪設軌道平面
	{
		vector<QVector3D> roadUp; //路面上方 要連續畫的點
		vector<QVector3D> roadDn; //路面下方 要連續畫的點
		vector<QVector3D> roadLf; //路面左方 要連續畫的點
		vector<QVector3D> roadRi; //路面右方 要連續畫的點

		for (size_t i = 0; i < trackMiddle.size(); i++)
		{
			const float roadThickness = 0.35;

			/*     馬路的後面
				s4 ________  s3
				  /        /|
				 /        / |          |
			 s1	/_______ /s2|          ↓ dd
				|        |  / s7
				|  s8    | /
				|_______ |/
			 s5          s6
				馬路的前面
			*/


			QVector3D s1 = trackLeft.at(i);
			QVector3D s2 = trackRight.at(i);
			QVector3D s3 = trackRight.at((i + 1) % trackRight.size());
			QVector3D s4 = trackLeft.at((i + 1) % trackLeft.size());
			QVector3D dd = QVector3D::crossProduct(s3 - s2, s1 - s2).normalized(); //往上的方向
			QVector3D s5 = s1 + dd * roadThickness;
			QVector3D s6 = s2 + dd * roadThickness;
			QVector3D s7 = s3 + dd * roadThickness;
			QVector3D s8 = s4 + dd * roadThickness;

			roadUp.push_back(s1);
			roadUp.push_back(s2);

			roadDn.push_back(s5);
			roadDn.push_back(s6);

			roadLf.push_back(s1);
			roadLf.push_back(s5);

			roadRi.push_back(s2);
			roadRi.push_back(s6);
		}

		glColor4f(0.05, 0.1, 0.3, 0);

		glBegin(GL_QUAD_STRIP); //連續畫
		for (auto s : roadUp)
			glVertexQVector3D(s);
		glVertexQVector3D(roadUp.at(0));
		glVertexQVector3D(roadUp.at(1));
		glEnd();

		glBegin(GL_QUAD_STRIP); //連續畫
		for (auto s : roadDn)
			glVertexQVector3D(s);
		glVertexQVector3D(roadDn.at(0));
		glVertexQVector3D(roadDn.at(1));
		glEnd();

		glBegin(GL_QUAD_STRIP); //連續畫
		for (auto s : roadLf)
			glVertexQVector3D(s);
		glVertexQVector3D(roadLf.at(0));
		glVertexQVector3D(roadLf.at(1));
		glEnd();

		glBegin(GL_QUAD_STRIP); //連續畫
		for (auto s : roadRi)
			glVertexQVector3D(s);
		glVertexQVector3D(roadRi.at(0));
		glVertexQVector3D(roadRi.at(1));
		glEnd();
	}
#pragma endregion

#pragma region 鋪設木棧道
	const float eachSpacing = 8.0; // 每片木頭間，相隔多少長度
	float currSpacing = eachSpacing; //距離鋪下一面木頭，還有多少距離

	QVector3D curr = trackMiddle.at(0); //目前走到的位置
	vector<QVector3D>::iterator next = ++trackMiddle.begin(); //下一個將會走到的點

	while (next != trackMiddle.end())
	{
		float distance = (*next - curr).length(); //目前的點 走到縣段終點的距離

		if (currSpacing - distance > 0) //走下一步，會跨過去
		{
			currSpacing -= distance;
			curr = *next;
			++next;
		}
		else //走下一步，不會跨過去
		{


			curr += (*next - curr).normalized()*currSpacing;

			{
				//drawBox(curr, 2);

				//--------------------------------------------

				const int woodWidth = 3;
				const int woodHight = 2;
				const float woodThickness = 0.4;
				const float downTrack = 0.05;

				/*     木棧道的前面
					s4 ________  s3
					  /        /|
					 /        / |		   ↓ uu : 下方
				 s1	/_______ /s2|          ← ll : 左方
					|        |  / s7       / ff : 前方
					|  s8    | /
					|_______ |/
				 s5          s6
				   木棧道的前面
				*/

				int index = (next - trackMiddle.begin()) - 1;
				QVector3D ll = (trackRight.at(index) - trackLeft.at(index)).normalized();
				QVector3D ff = (*next - curr).normalized();
				QVector3D uu = QVector3D::crossProduct(ff, ll).normalized();

				QVector3D s5 = curr + ll * woodWidth + uu * downTrack;
				QVector3D s6 = curr - ll * woodWidth + uu * downTrack;
				QVector3D s7 = curr + ll * woodWidth + ff * woodHight + uu * downTrack;
				QVector3D s8 = curr - ll * woodWidth + ff * woodHight + uu * downTrack;
				QVector3D s1 = curr + ll * woodWidth + uu * woodThickness;
				QVector3D s2 = curr - ll * woodWidth + uu * woodThickness;
				QVector3D s3 = curr + ll * woodWidth + ff * woodHight + uu * woodThickness;
				QVector3D s4 = curr - ll * woodWidth + ff * woodHight + uu * woodThickness;

				glColor4f(0.8, 0.8, 0.8, 0);

				glBegin(GL_QUAD_STRIP); //連續畫
				glVertexQVector3D(s1);
				glVertexQVector3D(s2);
				glVertexQVector3D(s3);
				glVertexQVector3D(s4);
				glVertexQVector3D(s8);
				glVertexQVector3D(s7);
				glVertexQVector3D(s6);
				glVertexQVector3D(s5);
				glVertexQVector3D(s1);
				glVertexQVector3D(s2);
				glEnd();

				glBegin(GL_QUADS); //單獨畫
				glVertexQVector3D(s1);
				glVertexQVector3D(s4);
				glVertexQVector3D(s8);
				glVertexQVector3D(s5);

				glVertexQVector3D(s2);
				glVertexQVector3D(s3);
				glVertexQVector3D(s7);
				glVertexQVector3D(s6);
				glEnd();
			}

			currSpacing = eachSpacing;
		}
	}
#pragma endregion
}

void TrainView::drawPillar()
{
	QVector2D cy[19] = { QVector2D(1.000, 0.000),QVector2D(0.940, 0.342),QVector2D(0.766, 0.643),QVector2D(0.500, 0.866),
		QVector2D(0.174, 0.985),QVector2D(-0.173, 0.985),QVector2D(-0.499, 0.867),QVector2D(-0.765, 0.644),QVector2D(-0.939, 0.343),
		QVector2D(-1.000, 0.002),QVector2D(-0.940, -0.340),QVector2D(-0.767, -0.641),QVector2D(-0.502, -0.865),QVector2D(-0.176, -0.984),
		QVector2D(0.171, -0.985),QVector2D(0.498, -0.867),QVector2D(0.764, -0.645),QVector2D(0.939, -0.345),QVector2D(1.000, -0.003) };


	const int r = 2;
	for (size_t i = 0; i < 19; ++i)
		cy[i] *= r;


	for (ControlPoint cp : m_pTrack->points)
	{
		Point3f pos = cp.pos;

		glBegin(GL_QUAD_STRIP); //連續畫
		for (size_t i = 0; i < 19; ++i)
		{
			glVertex3f(pos.x + cy[i].x(), pos.y, pos.z + cy[i].y());
			glVertex3f(pos.x + cy[i].x(), 0.0f, pos.z + cy[i].y());
		}
		glEnd();
	}
}

void TrainView::drawTrain(QVector3D trainPos, QVector3D trainUp, QVector3D trainDire)
//void TrainView::drawTrain()
{
	//我自己定義的畫四角椎的function

	//debug////
	//QVector3D trainPos(0, 4, 6); //火車的位置
	//QVector3D trainUp(0, 1, 0);  //火車上方
	//QVector3D trainDire(1, 0, 1);  //火車的前方
	///////////
	QVector3D trainLeft(QVector3D::crossProduct(trainUp, trainDire));  //火車的左方

	const float trainHeight = 4;
	const float trainBack = 4;
	const float trainFront = 6;
	const float trainWidth = 5.0 / 2;

	trainUp.normalize();
	trainDire.normalize();
	trainLeft.normalize();

	/*     木棧道的前面
		s4 ________  s3
		  /        /|
		 /        / |		   ↓ uu : 下方
	 s1	/_______ /s2|          ← ll : 左方
		/       /   / s7       / ff : 前方
	   / s8    /  /
	  /	______/ /
	 s5          s6
	   木棧道的前面
	*/

	QVector3D s1 = trainPos + trainUp * trainHeight - trainLeft * trainWidth;
	QVector3D s2 = trainPos + trainUp * trainHeight + trainLeft * trainWidth;
	QVector3D s3 = trainPos + trainUp * trainHeight + trainLeft * trainWidth - trainDire * trainBack;
	QVector3D s4 = trainPos + trainUp * trainHeight - trainLeft * trainWidth - trainDire * trainBack;
	QVector3D s5 = trainPos - trainLeft * trainWidth + trainDire * trainFront;
	QVector3D s6 = trainPos + trainLeft * trainWidth + trainDire * trainFront;
	QVector3D s7 = trainPos + trainLeft * trainWidth - trainDire * trainBack;
	QVector3D s8 = trainPos - trainLeft * trainWidth - trainDire * trainBack;

	//s1 s2 s3 s4
	//s8 s7 s6 s5
	//s3 s2 s6 s7
	//s1 s4 s8 s5
	//s2 s1 s5 s6
	//s4 s3 s7 s8

	glColor4f(232.0 / 255.0, 117.0 / 255.0, 17.0 / 255.0, 0.5);

	glBegin(GL_POLYGON);
	glNormalQVector3D(QVector3D::normal(s2 - s1, s3 - s1));
	glVertexQVector3D(s1); //a
	glVertexQVector3D(s2); //b
	glVertexQVector3D(s3); //c
	glVertexQVector3D(s4);
	glEnd();
	glBegin(GL_POLYGON);
	glNormalQVector3D(QVector3D::normal(s7 - s8, s6 - s8));
	glVertexQVector3D(s8); //a
	glVertexQVector3D(s7); //b
	glVertexQVector3D(s6); //c
	glVertexQVector3D(s5);
	glEnd();
	glBegin(GL_POLYGON);
	glNormalQVector3D(QVector3D::normal(s2 - s3, s6 - s3));
	glVertexQVector3D(s3); //a
	glVertexQVector3D(s2); //b
	glVertexQVector3D(s6); //c
	glVertexQVector3D(s7);
	glEnd();
	glBegin(GL_POLYGON);
	glNormalQVector3D(QVector3D::normal(s4 - s1, s8 - s1));
	glVertexQVector3D(s1); //a
	glVertexQVector3D(s4); //b
	glVertexQVector3D(s8); //c
	glVertexQVector3D(s5);
	glEnd();
	glBegin(GL_POLYGON);
	glNormalQVector3D(QVector3D::normal(s1 - s2, s5 - s2));
	glVertexQVector3D(s2); //a
	glVertexQVector3D(s1); //b
	glVertexQVector3D(s5); //c
	glVertexQVector3D(s6);
	glEnd();
	glBegin(GL_POLYGON);
	glNormalQVector3D(QVector3D::normal(s3 - s4, s7 - s4));
	glVertexQVector3D(s4); //a
	glVertexQVector3D(s3); //b
	glVertexQVector3D(s7); //c
	glVertexQVector3D(s8);
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
