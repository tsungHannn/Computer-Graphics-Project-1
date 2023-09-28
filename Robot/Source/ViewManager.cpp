#include "../Include/ViewManager.h"

/**
* 建立相機，並進行初始化。
*/
ViewManager::ViewManager()
{
	ortho = false;
	zoomValue = 3.0f;
	moveSpeed = 50.0f;
	lmbDown = false;
	midDown = false;
	eyePosition = glm::vec3(0.0f, 0.0f, 10.0f);
	eyeLookPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 up = glm::vec3(0, 1, 0);
	viewMatrix = lookAt(eyePosition, eyeLookPosition, up);
	viewVector = eyePosition - eyeLookPosition;
	viewVector = normalize(viewVector);
}

/**
* 取得Model Matrix。
*/
glm::mat4 ViewManager::getModelMatrix() {
	return translationMatrix * rotationMatrix;
}

/**
* 取得View Matrix。
*/
glm::mat4 ViewManager::getViewMatrix()
{
	return viewMatrix;
}

glm::mat4 ViewManager::getRotationMatrix() {
	return rotationMatrix;
}

glm::mat4 ViewManager::getTranslationMatrix() {
	return translationMatrix;
}
/**
* 設定並取得Projection Matrix。
* @param aspect 畫面的長寬比。
*/
glm::mat4 ViewManager::getProjectionMatrix(float aspect)
{
	float nearVal;
	float farVal;
	nearVal = 0.1f;
	farVal = 300.0f;
	if (ortho) {
		float size = 1.5f * zoomValue;
		projMatrix = glm::ortho(-aspect * size, aspect * size, -size, size, nearVal, farVal);
	}
	else {
		projMatrix = glm::perspective(glm::radians(30.0f * zoomValue), aspect, nearVal, farVal);
	}
	return projMatrix;
}

/**
* 取得Projection Matrix。
*/
glm::mat4 ViewManager::getProjectionMatrix()
{
	return getProjectionMatrix(aspect);
}


/**
* 取得 V * P 的矩陣。
* @param aspect 畫面的長寬比。
*/
glm::mat4 ViewManager::getViewProjectionMatrix(float aspect)
{
	return getProjectionMatrix(aspect) * viewMatrix;
}

/**
* 取得 M * V * P 的矩陣。
* @param aspect 畫面的長寬比。
*/
glm::mat4 ViewManager::getModelViewProjectionMatrix(float aspect)
{
	return getViewProjectionMatrix(aspect) * getModelMatrix();
}

/**
* 取得目前相機在世界座標的位置。
*/
glm::vec3 ViewManager::getWorldEyePosition() {
	glm::vec4 wordEyePosition = glm::vec4(eyePosition, 0) * getModelMatrix();
	return wordEyePosition.xyz();
}

/**
* 取得目前相機在世界座標的所看的點位置。
*/
glm::vec3 ViewManager::getWorldViewVector() {
	glm::vec4 wordLookVector = glm::vec4(-viewVector, 0) * getModelMatrix();
	return wordLookVector.xyz();
}

/**
* 處理當有按鍵輸入時，相機的動作。
* @param key 輸入的按鍵。
*/
void ViewManager::keyEvents(unsigned char key) {
	switch (key)
	{
		//向上移動。
	case 'w':
	case 'W':
		translate(glm::vec2(0, moveSpeed));
		break;

		//向左移動。
	case 'a':
	case 'A':
		translate(glm::vec2(moveSpeed, 0));
		break;

		//向下移動。
	case 's':
	case 'S':
		translate(glm::vec2(0, -moveSpeed));
		break;

		//向右移動。
	case 'd':
	case 'D':
		translate(glm::vec2(-moveSpeed, 0));
		break;
		//放大。
	case '+':
		wheelEvent(int(-moveSpeed));
		break;

		//縮小。
	case '-':
		wheelEvent(int(moveSpeed));
		break;
	default:
		break;
	}
}

/**
* 處理當有滑鼠事件時，相機的動作。
* @param button 輸入的按鍵。
* @param state 按鍵的狀態,點下,彈起。
* @param x 輸入的時, 滑鼠在畫面的x座標值。
* @param y 輸入的時, 滑鼠在畫面的y座標值。
*/
void ViewManager::mouseEvents(int button, int state, int x, int y) {

	if (state == GLUT_UP)
	{
		mouseReleaseEvent(button, x, y);
	}
	else if (state == GLUT_DOWN)
	{
		mousePressEvent(button, x, y);
	}

	//處理滑鼠中鍵向上滾動時
	if (button == 4)
	{
		wheelEvent(1);
	}
	//處理滑鼠中鍵向下滾動時
	else if (button == 3)
	{
		wheelEvent(-1);
	}
}

/**
* 處理當滑鼠按鍵點下時的動作。
* @param button 滑鼠的按鍵。
* @param x 輸入的時, 滑鼠在畫面的x座標值。
* @param y 輸入的時, 滑鼠在畫面的y座標值。
*/
void ViewManager::mousePressEvent(int button, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON)
	{
		//紀錄現在左鍵被按住
		lmbDown = true;
		lmbDownCoord = glm::vec2(x, y);
		glm::mat4 invrtRot = inverse(rotationMatrix);
		rotateYAxis = (invrtRot * glm::vec4(0, 1, 0, 0)).xyz();
		rotateXAxis = (invrtRot * glm::vec4(1, 0, 0, 0)).xyz();
	}
	else if (button == GLUT_MIDDLE_BUTTON)
	{
		//紀錄現在中鍵被按住
		midDown = true;
		midDownCoord = glm::vec2(x, y);
	}
}

/**
* 處理當滑鼠按鍵彈起時的動作。
* @param button 滑鼠的按鍵。
* @param x 輸入的時, 滑鼠在畫面的x座標值。
* @param y 輸入的時, 滑鼠在畫面的y座標值。
*/
void ViewManager::mouseReleaseEvent(int button, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON)
	{
		lmbDown = false;
	}
	else if (button == GLUT_MIDDLE_BUTTON || button == 3 || button == 4) {
		midDown = false;
	}
}

/**
* 處理當滑鼠移動時的動作。
* @param x 滑鼠在畫面的x座標值。
* @param y 滑鼠在畫面的y座標值。
*/
void ViewManager::mouseMoveEvent(int x, int y)
{
	if (lmbDown)
	{
		/*
		* 當滑鼠左鍵按住,進行拖曳時的時候
		* 計算移動的向量,進行相機的旋轉
		*/
		glm::vec2 coord = glm::vec2(x, y);
		glm::vec2 diff = coord - lmbDownCoord;
		float factor = 0.002f;
		rotationMatrix = glm::rotate(rotationMatrix, diff.x * factor, rotateYAxis);
		rotationMatrix = glm::rotate(rotationMatrix, diff.y * factor, rotateXAxis);
		lmbDownCoord = coord;
	}
	else if (midDown)
	{
		glm::vec2 coord = glm::vec2(x, y);
		glm::vec2 diff = coord - midDownCoord;

		glm::vec4 up = glm::vec4(0, 1, 0, 0);
		glm::vec4 right = glm::vec4(1, 0, 0, 0);

		glm::vec3 diffUp = up.xyz() * diff.y / (float)windowHeight;
		glm::vec3 diffRight = right.xyz() * diff.x / (float)windowWidth;

		translationMatrix = glm::translate(translationMatrix, (-diffUp + diffRight) * zoomValue * 3.0f);
		midDownCoord = coord;
	}
}

/**
* 根據中鍵的滾動方向處理事件。
* @param direction 前滾,後滾。
*/
void ViewManager::wheelEvent(int direction)
{
	mouseWheelValue = direction * 15.0f;
	zoom(mouseWheelValue / 120.0f);
}

/**
* 根據輸入的值,調整目前的縮放值。
* @param distance 增加的值。
*/
void ViewManager::zoom(float distance)
{
	zoomValue *= (1.0f + 0.05f * distance);
	zoomValue = glm::clamp(0.1f, zoomValue, 3.0f);
}

/**
* 告訴相機現在的螢幕大小。
* @param width 螢幕的寬。
* @param height 螢幕的高。
*/
void ViewManager::setWindowSize(int width, int height) {
	if (width == 0) {
		width = 1;
	}
	windowWidth = width;
	windowHeight = height;
	projMatrix = getProjectionMatrix();
	aspect = float(height) / width;
}


/**
* 以尤拉角設定相機的旋轉。
* @param theta 尤拉角的theta旋轉。
* @param phi 尤拉角的phi旋轉。
*/
void ViewManager::setRotation(float theta, float phi)
{
	rotationMatrix = glm::mat4(1.0);
	rotationMatrix = rotate(rotationMatrix, theta, glm::vec3(0, 1, 0));
	rotationMatrix = rotate(rotationMatrix, phi, glm::vec3(1, 0, 0));
}

/**
* 以尤拉角設定相機的旋轉。
* @param x 尤拉角的x值。
* @param y 尤拉角的y值。
* @param z 尤拉角的z值。
*/
void ViewManager::setRotation(float x, float y, float z)
{
	glm::vec3 v(x, y, z);
	v = normalize(v);
	glm::vec3 o(0, 0, 1);
	double angle = acos(dot(v, o));
	rotationMatrix = glm::mat4(1.0);
	rotationMatrix = rotate(rotationMatrix, (float)angle, cross(o, v));
}

/**
* 重設相機的設定。
*/
void ViewManager::reset()
{
	mouseWheelValue = 0.0f;
	zoomValue = 1.0f;
	translationMatrix = glm::mat4(1.0);
	rotationMatrix = glm::mat4(1.0);
}

/**
* 使相機移動。
* @param vec 使相機移動vec單位。
*/
void ViewManager::translate(glm::vec2 vec) {
	glm::vec2 diff = vec;

	glm::vec4 up = glm::vec4(0, 1, 0, 0);
	glm::vec4 right = glm::vec4(1, 0, 0, 0);

	glm::vec3 diffUp = up.xyz() * diff.y / (float)windowHeight;
	glm::vec3 diffRight = right.xyz() * diff.x / (float)windowWidth;

	translationMatrix = glm::translate(translationMatrix, (-diffUp + diffRight) * zoomValue * 3.0f);
}