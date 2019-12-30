#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <QOpenGLFunctions>

class Shader : protected QOpenGLFunctions
{
public:
	unsigned int ID;
	// constructor generates the shader on the fly
	// ------------------------------------------------------------------------
	Shader(const char* vertexPath, const char* fragmentPath);
	// activate the shader
	// ------------------------------------------------------------------------
	void use();

	static void Matrix16to4_4(GLfloat source[], GLfloat target[][4]);
private:
	// utility function for checking shader compilation/linking errors.
	// ------------------------------------------------------------------------
	void checkCompileErrors(unsigned int shader, std::string type);
};


