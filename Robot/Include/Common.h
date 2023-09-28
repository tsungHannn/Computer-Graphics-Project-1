#pragma once

#include "GLEW/glew.h"
#include "FreeGLUT/freeglut.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "../Include/TinyOBJ/tiny_obj_loader.h"

#define GLM_SWIZZLE
#include "../Include/GLM/glm.hpp"
#include "../Include/GLM/gtc/matrix_transform.hpp"
#include "../Include/GLM/gtc/type_ptr.hpp"
#include "../Include/GLM/gtx/rotate_vector.hpp"

#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>
#include <algorithm>

#define deg2rad(x) ((x)*((3.1415926f)/(180.0f)))
#define rad2deg(x) ((180.0f) / ((x)*(3.1415926f)))

typedef struct _TextureData
{
	_TextureData() : width(0), height(0), data(0) {}
	int width;
	int height;
	unsigned char* data;
} TextureData;

class Common
{
public:
	static char** LoadShaderSource(const char* file);
	static void FreeShaderSource(char** srcp);
	static void dumpInfo(void);
	static void shaderLog(GLuint shader);
	static void printGLError();
	static bool CheckShaderCompiled(GLuint shader);
	static bool CheckProgramLinked(GLuint program);
	static bool Common::CheckGLError();
	static TextureData loadPng(const char* path);
};

