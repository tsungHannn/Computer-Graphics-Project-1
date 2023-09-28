#pragma once

#include "Common.h"

/**
 * @brief The ViewManager class
 * The ViewManager class provides viewing manipulation related functionalities.
 *
 * To use the ViewManager class, call mousePressEvent(),
 * mouseReleaseEvent(), mouseMoveEvent(), wheelEvent() in your event handlers
 * with the event objects.
 *
 * The viewing manipulation will be done for you in the class. When you are ready
 * to render something, call GetModelMatrix(), GetViewMatrix(), GetProjectionMatrix()
 * and their composite versions to get the MVP matrices which ecode current viewing
 * properties.
 */

class ViewManager
{
public:
	ViewManager();

	void mouseEvents(int button, int state, int x, int y);
	void mousePressEvent(int button, int x, int y);
	void mouseReleaseEvent(int button, int x, int y);
	void mouseMoveEvent(int x, int y);
	void keyEvents(unsigned char key);
	void wheelEvent(int direction);
	void translate(glm::vec2 vec);

	glm::mat4 getModelMatrix();
	glm::mat4 getViewMatrix();
	glm::mat4 getProjectionMatrix();
	glm::mat4 getTranslationMatrix();
	glm::mat4 getRotationMatrix();
	glm::mat4 getProjectionMatrix(float aspect);
	glm::mat4 getViewProjectionMatrix(float aspect);
	glm::mat4 getModelViewProjectionMatrix(float aspect);
	glm::vec3 getEyePosition() { return eyePosition; }
	glm::vec3 getViewVector() { return viewVector; }

	glm::vec3 getWorldEyePosition();
	glm::vec3 getWorldViewVector();

	bool isOrthoProjection() { return ortho; }

	void setRotation(float theta, float phi);
	void setRotation(float x, float y, float z);
	void setWindowSize(int width, int height);

	bool toggleOrtho() { return ortho = !ortho; }
	void zoom(float distance);
	void reset();

private:
	float aspect;					///< 儲存目前視窗的長寬比。
	bool ortho;						///< 是否使用正交視角。
	float zoomValue;
	float moveSpeed;				///< 相機的移動速度。

	glm::mat4 translationMatrix;	///< 紀錄Translate動作的Matrix。
	glm::mat4 rotationMatrix;		///< 紀錄Rotation動作的Matrix。
	glm::mat4 viewMatrix;			///< 紀錄ViewMatrix。
	glm::mat4 projMatrix;			///< 紀錄projMatrix。
	glm::vec3 viewVector;			///< 紀錄相機看往焦點看的向量。
	glm::vec3 rotateXAxis;			///< 紀錄相機的X軸旋轉。
	glm::vec3 rotateYAxis;			///< 紀錄相機的Y軸旋轉。
	glm::vec3 eyePosition;			///< 紀錄相機的位置。
	glm::vec3 eyeLookPosition;		///< 紀錄相機的所看的位置。

	bool lmbDown;					///< 紀錄滑鼠左鍵是否被按住。
	bool midDown;					///< 紀錄滑鼠中鍵是否被按住。
	glm::vec2 lmbDownCoord;			///< 紀錄滑鼠左鍵點擊時的座標。
	glm::vec2 midDownCoord;			///< 紀錄滑鼠中鍵點擊時的座標。

	int windowWidth;					///< 紀錄螢幕的寬。
	int windowHeight;					///< 紀錄螢幕的高。
	float mouseWheelValue;				///< 紀錄滾輪的值。
};
