#pragma once

#include "Common.h"
#include "ShaderProgram.h"
#include "BaseParticleProp.h"

class BaseParticleEffect {
public:
	BaseParticleEffect();
	BaseParticleEffect(char* vertexShaderFile, char* fragmentShaderFile);
	~BaseParticleEffect();
	virtual void init();
	virtual void update(float dt);
	virtual void render();
	virtual void emit(BaseParticleProp prop, glm::mat4 model);
	typedef struct
	{
		GLuint vao;
		GLuint vbo;
	} Shape;

protected:
	ShaderProgram* particleShader;

private:
	virtual void createShaderProgram(char* vertexShaderFile, char* fragmentShaderFile);
	int seed = 0x13371337;
	float randomFloat();
	BaseParticleEffect::Shape shape;
	size_t poolIndex;
	size_t poolSize;
	std::vector<BaseParticleProp> particles;
};