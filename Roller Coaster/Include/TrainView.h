#ifndef TRAINVIEW_H  
#define TRAINVIEW_H  
#include <QGLWidget> 
#include <QtGui>  
#include <QtOpenGL>
#include <QVector3D>
#include <GL/GLU.h>
#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib") 
#include "Utilities/ArcBallCam.H"
#include "Utilities/3DUtils.H"
#include "Track.H"
#include "Triangle.h"
#include "Square.h"
#include "Model.h"
#include <iostream>
#include <tuple>
#include "Particle.h"
#include "Shader.h"

#include "ObjLoader.h"
#include "ObjLoader_ForShader.h"
#include "ObjLoader_ForShader_withNormal.h"

class AppMain;
class CTrack;

//#######################################################################
// TODO
// You might change the TrainView in order to add different objects to
// be drawn, or adjust the lighting, or ...
//#######################################################################


class TrainView : public QGLWidget, protected QOpenGLFunctions_4_3_Core
{
	Q_OBJECT
public:
	explicit TrainView(QWidget *parent = 0);
	~TrainView();

public:
	// overrides of important window things
	//virtual int handle(int);
	virtual void paintGL();

	// all of the actual drawing happens in this routine
	// it has to be encapsulated, since we draw differently if
	// we're drawing shadows (no colors, for example)
	// �e�y�D
	void drawStuff(bool doingShadows = false);

	// �e�伵�W
	void drawPillar();

	// �e����
	void drawTrain(QVector3D trainPos, QVector3D trainUp, QVector3D trainDir);
	//void drawTrain();

	// setup the projection - assuming that the projection stack has been
	// cleared for you
	void setProjection();

	// Reset the Arc ball control
	void resetArcball();

	// pick a point (for when the mouse goes down)
	void doPick(int mx, int my);

	void initializeGL();
	void initializeTexture();


public:
	ArcBallCam		arcball;			// keep an ArcBall for the UI
	int				selectedCube;  // simple - just remember which cube is selected

	CTrack*			m_pTrack;		// The track of the entire scene �y�D�x�s
	int camera; //�۾�����
	int curve; //���u���A  0:"Linear"�B1:"Cardinal"�B2:"Cubic" 
	int track; //�y�D���A  0:"Line"  1:"Track"  2:"Road"  
	bool isrun;
	unsigned long lastRedraw;
	float trainSpeed; //0~99 �Ψӵ����������t��
	float tens = 0.5; // �x�}���Ѽ� tension = 0.01~1 (�y�D�s���{��)
	Triangle* triangle;
	Square* square;
	GLfloat ProjectionMatrex[16];
	//GLfloat ProjectionMatrex_[4][4];
	GLfloat ModelViewMatrex[16];
	//GLfloat ModelViewMatrex_[4][4];
	QVector<QOpenGLTexture*> Textures;

	Model *m;
	ObjLoader poepleObj;
	ObjLoader tunnelObj;

	//mountain
	ObjLoader_ForShader_withNormal mountain;
	GLuint mountainVAO;
	GLuint mountainVBO;
	GLuint mountainTexture;
	int mountainTextureWidth, mountainTextureHeight, mountainTextureChannels;
	uchar *mountainTextureData;

	//Skybox
	float *skyboxVertices;
	GLuint skyboxVAO;
	GLuint skyboxVBO;
	GLuint skyboxTexture;
	vector<std::string> skyboxTexturePath{
		"./Textures/skybox/right.jpg",
		"./Textures/skybox/left.jpg",
		"./Textures/skybox/top.jpg",
		"./Textures/skybox/bottom.jpg",
		"./Textures/skybox/front.jpg",
		"./Textures/skybox/back.jpg"
	};
	vector<tuple<uchar*, int, int, int>> skyboxTextureData; //data,W,H,Channel


	QVector3D trainPos; //��������m
	QVector3D trainUp;  //�����W��
	QVector3D trainDire;  //�������e��


//private:
	inline void glVertexQVector3D(const QVector3D& v);
	inline void glNormalQVector3D(const QVector3D& v);

	void drawBox(QVector3D pos, float size);

	float arcLength; //�y�D�`����


	float t_time;
	unsigned int DIVIDE_LINE;


	const int NumIntegralDiv = 25; //Number of Integral Divide - �u�q�n�� �`�@���X��
	const int trackWidth = 2;
	vector<QVector3D> trackMiddle; //�K�y�������y�D�I
	vector<QVector3D> trackLeft; //���y������y�D�I
	vector<QVector3D> trackRight; //���y���k��y�D�I
	UINT64 trainNextPositionIndex = 1;
	float trainCurrSpacing = 1; //�Z���U�@�B�A�٦��h�ֶZ��

};




#endif // TRAINVIEW_H  