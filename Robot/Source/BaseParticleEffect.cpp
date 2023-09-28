#include "BaseParticleEffect.h"
#include "Shader.h"
#include "Scene.h"

BaseParticleEffect::BaseParticleEffect()
{

}

BaseParticleEffect::BaseParticleEffect(char* vertexShaderFile, char* fragmentShaderFile)
{
	createShaderProgram(vertexShaderFile, fragmentShaderFile);
	init();
}

BaseParticleEffect::~BaseParticleEffect()
{
}

void BaseParticleEffect::init()
{
	float vertices[] = {
		// back face
		-1.0f, -1.0f, -1.0f, // bottom-left
		 1.0f,  1.0f, -1.0f, // top-right
		 1.0f, -1.0f, -1.0f, // bottom-right         
		 1.0f,  1.0f, -1.0f,  // top-right
		-1.0f, -1.0f, -1.0f, // bottom-left
		-1.0f,  1.0f, -1.0f,  // top-left
		// front face
		-1.0f, -1.0f,  1.0f,  // bottom-left
		 1.0f, -1.0f,  1.0f,  // bottom-right
		 1.0f,  1.0f,  1.0f,  // top-right
		 1.0f,  1.0f,  1.0f,  // top-right
		-1.0f,  1.0f,  1.0f,  // top-left
		-1.0f, -1.0f,  1.0f,  // bottom-left
		// left face
		-1.0f,  1.0f,  1.0f, // top-right
		-1.0f,  1.0f, -1.0f, // top-left
		-1.0f, -1.0f, -1.0f, // bottom-left
		-1.0f, -1.0f, -1.0f, // bottom-left
		-1.0f, -1.0f,  1.0f, // bottom-right
		-1.0f,  1.0f,  1.0f, // top-right
		// right face
		 1.0f,  1.0f,  1.0f,  // top-left
		 1.0f, -1.0f, -1.0f,  // bottom-right
		 1.0f,  1.0f, -1.0f,  // top-right         
		 1.0f, -1.0f, -1.0f,  // bottom-right
		 1.0f,  1.0f,  1.0f,  // top-left
		 1.0f, -1.0f,  1.0f,  // bottom-left     
		// bottom face
		-1.0f, -1.0f, -1.0f,  // top-right
		 1.0f, -1.0f, -1.0f,  // top-left
		 1.0f, -1.0f,  1.0f,  // bottom-left
		 1.0f, -1.0f,  1.0f,  // bottom-left
		-1.0f, -1.0f,  1.0f,  // bottom-right
		-1.0f, -1.0f, -1.0f, // top-right
		// top face
		-1.0f,  1.0f, -1.0f,  // top-left
		 1.0f,  1.0f , 1.0f,  // bottom-right
		 1.0f,  1.0f, -1.0f,  // top-right     
		 1.0f,  1.0f,  1.0f,  // bottom-right
		-1.0f,  1.0f, -1.0f, // top-left
		-1.0f,  1.0f,  1.0f,  // bottom-left        
	};
	glGenVertexArrays(1, &shape.vao);
	glGenBuffers(1, &shape.vbo);
	glBindVertexArray(shape.vao);
	glBindBuffer(GL_ARRAY_BUFFER, shape.vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glBindVertexArray(0);
	poolIndex = 0;
	poolSize = 10000;
	particles.resize(poolSize);
}

void BaseParticleEffect::update(float dt)
{
	for (auto& particle : particles) {
		if (!particle.active)continue;
		if (particle.lifeRemaining <= 0.0f) {
			particle.active = false;
			continue;
		}

		particle.lifeRemaining -= dt;
		particle.position += particle.velocity * (float)dt;
		particle.rotation += 0.01f * dt;
	}
}

void BaseParticleEffect::render()
{
	particleShader->useProgram();
	glm::mat4 viewProj = Scene::getCamera()->getProjectionMatrix() *
		Scene::getCamera()->getViewMatrix() *
		Scene::getCamera()->getModelMatrix();
	particleShader->setMat4("u_ViewProj", viewProj);
	for (auto& particle : particles) {
		if (!particle.active)
			continue;

		float life = particle.lifeRemaining / particle.lifeTime;
		glm::vec4 color = (1.0f - life) * particle.colorEnd + life * particle.colorBegin;
		color.a = color.a * life;

		float size = (1.0f - life) * particle.sizeEnd + life * particle.sizeBegin;

		glm::mat4 transform = particle.model
			* glm::translate(glm::mat4(1.0), { particle.position.x, particle.position.y, particle.position.z })
			* glm::rotate(glm::mat4(1.0f), particle.rotation.x, { 1.0f, 0.0f, 0.0f })
			* glm::rotate(glm::mat4(1.0f), particle.rotation.y, { 0.0f,1.0f, 0.0f })
			* glm::rotate(glm::mat4(1.0f), particle.rotation.z, { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size, size, size });

		particleShader->setMat4("u_Transform", transform);
		glUniform4fv(glGetUniformLocation(particleShader->getId(), "u_Color"), 1, glm::value_ptr(color));
		glBindVertexArray(shape.vao);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
}

void BaseParticleEffect::emit(BaseParticleProp prop, glm::mat4 model)
{
	if (!prop.active) {
		return;
	}
	for (int i = 0; i < prop.emitAmount; i++) {
		particles[poolIndex] = BaseParticleProp(prop);
		particles[poolIndex].model = model;
		particles[poolIndex].rotation.x = randomFloat() * 2.0f * glm::pi<float>();
		particles[poolIndex].rotation.y = randomFloat() * 2.0f * glm::pi<float>();
		particles[poolIndex].rotation.z = randomFloat() * 2.0f * glm::pi<float>();
		particles[poolIndex].velocity.x += prop.velocityVariation.x * (randomFloat() - 0.5f);
		particles[poolIndex].velocity.y += prop.velocityVariation.x * (randomFloat() - 0.5f);
		particles[poolIndex].velocity.z += prop.velocityVariation.x * (randomFloat() - 0.5f);
		particles[poolIndex].lifeRemaining = prop.lifeTime;
		poolIndex++;
		poolIndex %= poolSize;
	}
}

void BaseParticleEffect::createShaderProgram(char* vertexShaderFile, char* fragmentShaderFile)
{
	particleShader = new ShaderProgram();
	particleShader->createProgram();
	Shader vs = Shader();
	Shader fs = Shader();
	vs.loadShader(vertexShaderFile, GL_VERTEX_SHADER);
	fs.loadShader(fragmentShaderFile, GL_FRAGMENT_SHADER);
	particleShader->attachShader(vs.getId());
	particleShader->attachShader(fs.getId());
	particleShader->linkProgram();

	glDeleteShader(vs.getId());
	glDeleteShader(fs.getId());
}

float BaseParticleEffect::randomFloat()
{
	float res;
	unsigned int tmp;

	seed *= 16807;

	tmp = seed ^ (seed >> 4) ^ (seed << 15);

	*((unsigned int*)&res) = (tmp >> 9) | 0x3F800000;

	return (res - 1.0f);
}
