#include "..\Include\RobotModelPart.h"

RobotModelPart::RobotModelPart()
{
}

RobotModelPart::RobotModelPart(RobotModelPart* parent, char* objFile, glm::mat4 offset)
	: BaseModelPart(parent, objFile, offset)
{
	this->baseParticleProp = nullptr;
}

RobotModelPart::RobotModelPart(RobotModelPart* parent, BaseParticleProp* baseParticleProp, glm::mat4 offset)
	: BaseModelPart(parent, offset)
{
	this->baseParticleProp = baseParticleProp;
}

void (*RobotModelPart::applyFunc)(unsigned int time, RobotModelPart* part, const std::string& prop, float value) = [](unsigned int time, RobotModelPart* part, const std::string& prop, float value) {
	if (prop == "RX") {
		part->rotateMatrix = glm::rotate(part->rotateMatrix, glm::radians(value), glm::vec3(1.0, 0.0, 0.0));
	}
	else if (prop == "RY") {
		part->rotateMatrix = glm::rotate(part->rotateMatrix, glm::radians(value), glm::vec3(0.0, 1.0, 0.0));
	}
	else if (prop == "RZ") {
		part->rotateMatrix = glm::rotate(part->rotateMatrix, glm::radians(value), glm::vec3(0.0, 0.0, 1.0));
	}
	else if (prop == "TX") {
		part->translateMatrix = glm::translate(part->translateMatrix, glm::vec3(value, 0.0, 0.0));
	}
	else if (prop == "TY") {
		part->translateMatrix = glm::translate(part->translateMatrix, glm::vec3(0.0, value, 0.0));
	}
	else if (prop == "TZ") {
		part->translateMatrix = glm::translate(part->translateMatrix, glm::vec3(0.0, 0.0, value));
	}
	else if (prop == "SX") {
		part->scaleMatrix = glm::scale(part->scaleMatrix, glm::vec3(value, 0.0, 0.0));
	}
	else if (prop == "SY") {
		part->scaleMatrix = glm::scale(part->scaleMatrix, glm::vec3(0.0, value, 0.0));
	}
	else if (prop == "SZ") {
		part->scaleMatrix = glm::scale(part->scaleMatrix, glm::vec3(0.0, 0.0, value));
	}
	else if (prop == "PA") {
		part->baseParticleProp->active = value == 0 ? false : true;
	}
	else if (prop == "PL") {
		part->baseParticleProp->lifeTime = value;
	}
	else if (prop == "PVX") {
		part->baseParticleProp->velocity.x = value;
	}
	else if (prop == "PVY") {
		part->baseParticleProp->velocity.y = value;
	}
	else if (prop == "PVZ") {
		part->baseParticleProp->velocity.z = value;
	}
	else if (prop == "PVVX") {
		part->baseParticleProp->velocityVariation.x = value;
	}
	else if (prop == "PVVY") {
		part->baseParticleProp->velocityVariation.y = value;
	}
	else if (prop == "PVVZ") {
		part->baseParticleProp->velocityVariation.z = value;
	}
	else if (prop == "PCBR") {
		part->baseParticleProp->colorBegin.r = value / 255.0f;
	}
	else if (prop == "PCBG") {
		part->baseParticleProp->colorBegin.g = value / 255.0f;
	}
	else if (prop == "PCBB") {
		part->baseParticleProp->colorBegin.b = value / 255.0f;
	}
	else if (prop == "PCER") {
		part->baseParticleProp->colorEnd.r = value / 255.0f;
	}
	else if (prop == "PCEG") {
		part->baseParticleProp->colorEnd.g = value / 255.0f;
	}
	else if (prop == "PCEB") {
		part->baseParticleProp->colorEnd.b = value / 255.0f;
	}
	else if (prop == "PSB") {
		part->baseParticleProp->sizeBegin = value;
	}
	else if (prop == "PSV") {
		part->baseParticleProp->sizeVariation = value;
	}
	else if (prop == "PSE") {
		part->baseParticleProp->sizeEnd = value;
	}
	else if (prop == "PPX") {
		part->baseParticleProp->position.x = value;
	}
	else if (prop == "PPY") {
		part->baseParticleProp->position.y = value;
	}
	else if (prop == "PPZ") {
		part->baseParticleProp->position.z = value;
	}
};

void (*RobotModelPart::resetFunc)(unsigned int time, RobotModelPart* part) = [](unsigned int time, RobotModelPart* part) {
	part->translateMatrix = glm::mat4();
	part->rotateMatrix = glm::mat4();
	part->scaleMatrix = glm::mat4();
	if (part->baseParticleProp != nullptr) {
		part->baseParticleProp->active = false;
	}
};
