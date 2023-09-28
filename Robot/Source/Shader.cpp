#include "../Include/Shader.h"

GLuint Shader::getId()
{
	return id;
}

bool Shader::isLoaded()
{
	return loaded;
}

void Shader::loadShader(char* fileName, int shaderType)
{
	GLuint s = glCreateShader(shaderType);
	char** source = loadShaderSource(fileName);
	glShaderSource(s, 1, source, NULL);
	freeShaderSource(source);
	glCompileShader(s);

	id = s;
	type = shaderType;
	loaded = true;
}

void Shader::deleteShader() {
	if (!loaded)return;
	loaded = false;
	glDeleteShader(id);
}

char** Shader::loadShaderSource(const char* file)
{
	FILE* fp = fopen(file, "rb");
	fseek(fp, 0, SEEK_END);
	long sz = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	char* src = new char[sz + 1];
	fread(src, sizeof(char), sz, fp);
	src[sz] = '\0';
	char** srcp = new char* [1];
	srcp[0] = src;
	return srcp;
}

void Shader::freeShaderSource(char** srcp)
{
	delete srcp[0];
	delete srcp;
}
