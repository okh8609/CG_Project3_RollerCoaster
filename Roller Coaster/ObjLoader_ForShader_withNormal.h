
#pragma once

#include <string>
#include <cmath>
#include <vector>
#include <utility>
#include <fstream>
#include <sstream>
#include <cassert>
#include <algorithm>
#include <iostream>
#include <stdexcept>

#include <QtOpenGL>  
#include <GL/GLU.h>
#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib") 

#include <QVector3D>

using namespace std;

class ObjLoader_ForShader_withNormal
{
public:
	ObjLoader_ForShader_withNormal();
	ObjLoader_ForShader_withNormal(const string &filePath, int size);
	float *all_data;
	int getDataSize();
	int getVertexCount();


private:
	string _filePath;
	int _size;

	QVector3D boundsMin = QVector3D(1e9, 1e9, 1e9);
	QVector3D boundsMax = QVector3D(-1e9, -1e9, -1e9);

	vector<QVector3D> all_v; //編號的頂點位置
	vector<QVector3D> all_vn; //編號的頂點位置
	vector<vector<pair<int,int>>> all_f; // 位置編號 法向量編號

	template<typename T>
	inline T maximum(T a, T b) { return a > b ? a : b; }
	template<typename T>
	inline T maximum(T a, T b, T c) { return maximum(maximum(a, b), c); }
};







