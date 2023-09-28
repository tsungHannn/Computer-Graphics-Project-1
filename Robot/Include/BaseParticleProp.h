#pragma once

#include "Common.h"

class BaseParticleProp
{
	friend class BaseParticleEffect;
public:
	glm::mat4 model;
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 velocity;
	glm::vec3 velocityVariation;
	float sizeBegin;
	float sizeEnd;
	float sizeVariation;
	glm::vec4 colorBegin, colorEnd;
	float lifeTime;
	bool active;
	int emitAmount;
private:
	
	float lifeRemaining;
};
