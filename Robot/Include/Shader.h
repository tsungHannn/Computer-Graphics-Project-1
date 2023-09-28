#pragma once

#include "Common.h"

class Shader
{
public:
	GLuint getId();
	bool isLoaded();
	void loadShader(char* fileName, int _shaderType);
	void deleteShader();

private:
	GLuint id;
	int type;
	bool loaded;
	char** loadShaderSource(const char* file);
	void freeShaderSource(char** srcp);
};

