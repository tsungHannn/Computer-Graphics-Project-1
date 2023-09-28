#include "../Include/BaseModelPart.h"

BaseModelPart::BaseModelPart()
{
}

BaseModelPart::BaseModelPart(BaseModelPart* parent, char* objFile, glm::mat4 offset)
{
	BaseModelPart::parent = parent;
	BaseModelPart::objFile = objFile;
	BaseModelPart::offsetMatrix = offset;
	BaseModelPart::rotateMatrix = glm::mat4();
	BaseModelPart::translateMatrix = glm::mat4();
	BaseModelPart::scaleMatrix = glm::mat4();
	BaseModelPart::shape.model = glm::mat4();
}

BaseModelPart::BaseModelPart(BaseModelPart* parent, glm::mat4 offset)
{
	BaseModelPart::parent = parent;
	BaseModelPart::offsetMatrix = offset;
	BaseModelPart::rotateMatrix = glm::mat4();
	BaseModelPart::translateMatrix = glm::mat4();
	BaseModelPart::scaleMatrix = glm::mat4();
	objFile = nullptr;
}

char* BaseModelPart::getObjFile()
{
	return objFile;
}

BaseModelPart* BaseModelPart::getParent()
{
	return parent;
}

void BaseModelPart::translate(glm::vec3 vec)
{
	translateMatrix = glm::translate(translateMatrix, vec);
}

void BaseModelPart::rotate(glm::vec3 axis, float angle)
{
	rotateMatrix = glm::rotate(rotateMatrix, angle, axis);
}

void BaseModelPart::scale(glm::vec3 vec)
{
	scaleMatrix = glm::scale(scaleMatrix, vec);
}
