#include "../Include/ShaderProgram.h"

ShaderProgram::ShaderProgram()
{
	linked = false;
}

GLuint ShaderProgram::getId()
{
	return id;
}

GLuint ShaderProgram::createProgram()
{
	id = glCreateProgram();
	return id;
}

void ShaderProgram::deleteProgram()
{
	if (!linked) {
		return;
	}
	linked = false;
	glDeleteProgram(id);
}

void ShaderProgram::linkProgram()
{
	glLinkProgram(id);
	int iLinkStatus;
	glGetProgramiv(id, GL_LINK_STATUS, &iLinkStatus);
	linked = iLinkStatus == GL_TRUE;
}

void ShaderProgram::useProgram()
{
	if (linked) {
		glUseProgram(id);
	}
}

void ShaderProgram::attachShader(GLuint shaderId) {
	glAttachShader(id, shaderId);
}


void ShaderProgram::setBool(const std::string& name, bool value)
{
	glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value);
}
// ------------------------------------------------------------------------
void ShaderProgram::setInt(const std::string& name, int value)
{
	glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}
// ------------------------------------------------------------------------
void ShaderProgram::setFloat(const std::string& name, float value)
{
	glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}

void ShaderProgram::setVec2(const std::string& name, const glm::vec2& value)
{
	glUniform2fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
}
void ShaderProgram::setVec3(const std::string& name, const glm::vec3& value)
{
	glUniform3fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
}
void ShaderProgram::setMat3(const std::string& name, const glm::mat3& mat)
{
	glUniformMatrix3fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
void ShaderProgram::setMat4(const std::string& name, const glm::mat4& mat)
{
	glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}