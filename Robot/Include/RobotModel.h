#pragma once

#include "BaseModel.h"
#include "RobotModelPart.h"
#include "Animation.h"
#include "CubeMap.h"
#include "BaseParticleEffect.h"

#include <map>

class RobotModel : public BaseModel {
public:
	enum class ActionEnum {
		Idle = 0,
		Walk = 1,
		Run = 2,
		JumpingJack = 3,
		Squat = 4,
		Moonwalk = 5,
		GangnamStyle = 6,
		HeadTrick = 7,
		Pushup = 8,
		Fly = 9
	};

	RobotModel();

	void init() override;
	void update(unsigned int dt) override;
	void render() override;
	void renderQuad(GLuint &quadVAO, GLuint &quadVBO);
	void setAction(ActionEnum action);
	void reloadAnimations();
	void renderToScreen(int _filter_selction, float rows, float cols);
	inline ShaderProgram* getProgram() { return program; }
	inline ShaderProgram* getQuadProgram() { return program_quad; }

	GLuint depthMapFBO;
	GLuint quadVAO = 0;
	GLuint quadVBO = 0;
	GLuint depthMap;

	GLuint updateScreenFBO;
	GLuint updateScreen;
	GLuint InnerScreenFBO;
	GLuint InnerScreen;
	GLuint ScreenFBO;
	GLuint Screen;

	GLuint floorTex, floorVAO;
private:
	void loadModel() override;
	void createVertexProgram(char* vertexShaderFile, char* fragmentShaderFile) override;
	void createDepthProgram(char* vertexShaderFile, char* fragmentShaderFile) override;
	void createQuadProgram(char* vertexShaderFile, char* fragmentShaderFile) override;
	void loadAnimation();
	static const size_t PART_SIZE = 22;
	RobotModelPart robotParts[PART_SIZE];
	ShaderProgram* program, * program_depth, * program_quad;
	Uniform uniforms;
	UniformDep uniforms_depth;
	UniformQuad uniforms_quad;
	Animation<RobotModelPart, ActionEnum> animation;
	BaseParticleEffect fireParticleEffect;
};

