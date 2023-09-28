#pragma once

#include "Common.h"

class ShaderProgram
{
public:
	ShaderProgram();
	GLuint getId();
	GLuint createProgram();
	void deleteProgram();
	void linkProgram();
	void useProgram();
	void attachShader(GLuint shaderId);

	void setBool(const std::string& name, bool value);
	void setInt(const std::string& name, int value);
	void setFloat(const std::string& name, float value);
	void ShaderProgram::setVec2(const std::string& name, const glm::vec2& value);
	void ShaderProgram::setVec3(const std::string& name, const glm::vec3& value);
	void ShaderProgram::setMat3(const std::string& name, const glm::mat3& mat);
	void ShaderProgram::setMat4(const std::string& name, const glm::mat4& mat);

private:
	GLuint id;
	bool linked;
};

