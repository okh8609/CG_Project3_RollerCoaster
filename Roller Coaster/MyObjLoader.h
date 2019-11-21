#pragma once

#include <string>
#include <cmath>
#include <vector>
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

class MyObjLoader
{
public:
	MyObjLoader() {}
	MyObjLoader(const string &filePath, int size, QVector3D origin);
	void render() const;

	inline string getFileName() const { return _filePath; }
	inline int numberOfSurface() const { return all_surface.size(); }
	inline int numberOfPoint() const { return all_vertex.size(); }

private:
	string _filePath;
	QVector3D _origin;

	vector<QVector3D> all_vertex;
	vector<vector<int>> all_surface;

	float scale;

	inline void glVertexVec3(const QVector3D& v) const { glVertex3f(v.x(), v.y(), v.z()); }

	template<typename T>
	inline T maximum(T a, T b) { return a > b ? a : b; }

	template<typename T>
	inline T maximum(T a, T b, T c) { return maximum(maximum(a, b), c); }
};





