#pragma once

#include "BaseModelPart.h"
#include "AnimationClip.h"
#include "BaseParticleProp.h"

class RobotModelPart :public BaseModelPart {
public:
	RobotModelPart();
	RobotModelPart(RobotModelPart* parent, char* objFile, glm::mat4 offset);
	RobotModelPart(RobotModelPart* parent, BaseParticleProp* baseParticleProp, glm::mat4 offset);
	static void (*applyFunc)(unsigned int time, RobotModelPart* part, const std::string& prop, float value);
	static void (*resetFunc)(unsigned int tine, RobotModelPart* part);

	BaseParticleProp* baseParticleProp;
private:

};