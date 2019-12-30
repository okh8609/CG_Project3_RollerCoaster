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

class ObjLoader
{
public:
	ObjLoader() {}
	ObjLoader(const string &filePath, int size);
	void renderAt(QVector3D pos);

	inline string getFileName() const { return _filePath; }
	inline int numberOfSurface() const { return all_surface.size(); }
	inline int numberOfPoint() const { return all_vertex.size(); }
	inline void setPosition(QVector3D pos) { _origin = pos; }
	void render() const;

	inline float x() {return _origin.x();};
	inline float y() {return _origin.y();};
	inline float z() {return _origin.z();};

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





