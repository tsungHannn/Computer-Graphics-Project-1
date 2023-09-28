#pragma once

#include "BaseModel.h"

class BaseModelPart {
public:
	BaseModelPart();
	BaseModelPart(BaseModelPart* parent, char* objFile, glm::mat4 offset);
	BaseModelPart(BaseModelPart* parent, glm::mat4 offset);

	BaseModel::Shape shape;

	glm::mat4 offsetMatrix;
	glm::mat4 translateMatrix;
	glm::mat4 rotateMatrix;
	glm::mat4 scaleMatrix;

	char* getObjFile();
	BaseModelPart* getParent();

	void translate(glm::vec3 vec);
	void rotate(glm::vec3 axis, float angle);
	void scale(glm::vec3 vec);


private:
	char* objFile;
	BaseModelPart* parent;
};