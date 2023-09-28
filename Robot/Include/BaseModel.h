#pragma once

#include "Common.h"
#include "Shader.h"
#include "ShaderProgram.h"

struct LightSource {
	glm::vec3 lightPos;
	glm::mat4 lightProjection, lightView;
	glm::mat4 lightSpaceMatrix;
	float near_plane, far_plane;
};

class BaseModel {
public:
	BaseModel();
	BaseModel(char* modelFile, char* textFile);
	~BaseModel();
	virtual void init();
	virtual void update(unsigned int dt);
	virtual void render();
	virtual void Enable();
	virtual void Disable();

	void translate(glm::vec3 vec);
	void rotate(glm::vec3 axis, float angle);
	void scale(glm::vec3 vec);

	glm::mat4 getTranslateMatrix();
	glm::mat4 getRotateMatrix();
	glm::mat4 getScaleMatrix();
	inline ShaderProgram* getProgram() { return program; }
	inline ShaderProgram* getQuadProgram() { return program_quad; }
	inline void SetMixRate(float _mixRate) { mixrate = _mixRate; }
	GLuint getProgramID();

	typedef struct
	{
		GLuint vao;
		GLuint vbo;
		GLuint vboTex;
		GLuint ebo;

		GLuint pNormal;
		int materialId;
		int indexCount;
		GLuint mTexture;
		GLuint normalTexture;

		glm::mat4 model;
	} Shape;

	typedef struct
	{
		GLint  modelMatrix;
		GLint  viewMatrix;
		GLint  projMatrix;

		GLint  cameraPos;
		GLint  lightPos;
		GLint  useEnvMap;
		GLint  reflectOrRefract;
		GLint  shaderMaterial;
		GLint normalMapping;

		GLint tDiffuse, tNormal, tCube, tDepth;
	} Uniform;
	typedef struct
	{
		GLint lightSpaceMatrix;
		GLint modelMatrix;
	} UniformDep;
	typedef struct
	{
		GLint tDepth;
	} UniformQuad;


	//CubeMap
	void SetCameraPos(glm::vec3 _cameraPos);
	void SetUseEnvMap(bool _useEnvMap);
	void SetUseSkyBox(bool _useSkyBox);
	void SetReflectOrRefract(bool _reflectOrRefract);
	void SetShaderMaterialSel(int _shaderMaterialSel);
	void SetLightSource(LightSource* _light);

private:
	char* objFile;
	char* textureFile;
	ShaderProgram* program;
	ShaderProgram* program_depth;
	ShaderProgram* program_quad;
	Shape m_shape;
	virtual void loadModel();
	virtual void createVertexProgram(char* vertexShaderFile, char* fragmentShaderFile);
	virtual void createDepthProgram(char* vertexShaderFile, char* fragmentShaderFile);
	virtual void createQuadProgram(char* vertexShaderFile, char* fragmentShaderFile);
	Uniform uniforms;
	UniformDep uniforms_depth;
	UniformQuad uniforms_quad;
	glm::mat4 translateMatrix;
	glm::mat4 rotateMatrix;
	glm::mat4 scaleMatrix;

protected:
	glm::mat4 lightSpaceMatrix;
	glm::vec3 cameraPos, lightPos;
	float near_plane, far_plane;
	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;  //depth map size

	//CubeMap
	bool reflectOrRefract, useEnvMap;
	bool useSkyBox;

	//motion blur
	float mixrate = 0.5;

	//shader material
	int shaderMaterialSel = 0;
};
