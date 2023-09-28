#include "../Include/BaseModel.h"
#include "../Include/Scene.h"

BaseModel::BaseModel() {
}

BaseModel::BaseModel(char* modelFile, char* textureFile) {
	BaseModel::objFile = modelFile;
	BaseModel::textureFile = textureFile;
	init();
}

BaseModel::~BaseModel() {

}
GLuint BaseModel::getProgramID() {
	return program->getId();
}

void BaseModel::init() {
	createVertexProgram("vertex.vs.glsl", "fragment.fs.glsl");

	m_shape.model = glm::mat4();
	translateMatrix = glm::mat4();
	rotateMatrix = glm::mat4();
	scaleMatrix = glm::mat4();
	
	useSkyBox = true;
	useEnvMap = false;
	loadModel();
}

void BaseModel::loadModel() {
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string err;

	bool ret = tinyobj::LoadObj(shapes, materials, err, objFile);
	if (err.size() > 0)
	{
		printf("Model loading failed!");
		return;
	}

	for (size_t i = 0; i < shapes.size(); i++)
	{
		glGenVertexArrays(1, &m_shape.vao);
		glBindVertexArray(m_shape.vao);

		glGenBuffers(3, &m_shape.vbo);
		glGenBuffers(1, &m_shape.pNormal);
		glBindBuffer(GL_ARRAY_BUFFER, m_shape.vbo);
		glBufferData(GL_ARRAY_BUFFER, shapes[i].mesh.positions.size() * sizeof(float) + shapes[i].mesh.normals.size() * sizeof(float), NULL, GL_STATIC_DRAW);

		glBufferSubData(GL_ARRAY_BUFFER, 0, shapes[i].mesh.positions.size() * sizeof(float), &shapes[i].mesh.positions[0]);
		glBufferSubData(GL_ARRAY_BUFFER, shapes[i].mesh.positions.size() * sizeof(float), shapes[i].mesh.normals.size() * sizeof(float), &shapes[i].mesh.normals[0]);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)(shapes[i].mesh.positions.size() * sizeof(float)));

		glBindBuffer(GL_ARRAY_BUFFER, m_shape.pNormal);
		glBufferData(GL_ARRAY_BUFFER, shapes[i].mesh.normals.size() * sizeof(float), shapes[i].mesh.normals.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindBuffer(GL_ARRAY_BUFFER, m_shape.vboTex);
		glBufferData(GL_ARRAY_BUFFER, shapes[i].mesh.texcoords.size() * sizeof(float), shapes[i].mesh.texcoords.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_shape.ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, shapes[i].mesh.indices.size() * sizeof(unsigned int), shapes[i].mesh.indices.data(), GL_STATIC_DRAW);
		m_shape.materialId = shapes[i].mesh.material_ids[0];
		m_shape.indexCount = shapes[i].mesh.indices.size();

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
	}


	TextureData tdata = Common::loadPng(textureFile);

	glGenTextures(1, &m_shape.mTexture);
	glBindTexture(GL_TEXTURE_2D, m_shape.mTexture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, tdata.width, tdata.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tdata.data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void BaseModel::createVertexProgram(char* vertexShaderFile, char* fragmentShaderFile)
{
	program = new ShaderProgram();
	program->createProgram();
	Shader* vs = new Shader();
	Shader* fs = new Shader();
	vs->loadShader(vertexShaderFile, GL_VERTEX_SHADER);
	fs->loadShader(fragmentShaderFile, GL_FRAGMENT_SHADER);
	program->attachShader(vs->getId());
	program->attachShader(fs->getId());
	program->linkProgram();

	uniforms.projMatrix = glGetUniformLocation(program->getId(), "um4p");
	uniforms.modelMatrix = glGetUniformLocation(program->getId(), "um4m");
	uniforms.viewMatrix = glGetUniformLocation(program->getId(), "um4v");

	uniforms.cameraPos = glGetUniformLocation(program->getId(), "cameraPos");
	uniforms.useEnvMap = glGetUniformLocation(program->getId(), "useEnvMap");
	uniforms.reflectOrRefract = glGetUniformLocation(program->getId(), "reflectOrRefract");
	program->useProgram();
}
//tmp
void BaseModel::createDepthProgram(char* vertexShaderFile, char* fragmentShaderFile)
{
	program_depth = new ShaderProgram();
	program_depth->createProgram();
	Shader* vs = new Shader();
	Shader* fs = new Shader();
	vs->loadShader(vertexShaderFile, GL_VERTEX_SHADER);
	fs->loadShader(fragmentShaderFile, GL_FRAGMENT_SHADER);
	program_depth->attachShader(vs->getId());
	program_depth->attachShader(fs->getId());
	program_depth->linkProgram();

	program_depth->useProgram();

	uniforms_depth.lightSpaceMatrix = glGetUniformLocation(program_depth->getId(), "lightSpaceMatrix");
	uniforms_depth.modelMatrix = glGetUniformLocation(program_depth->getId(), "model");

	if (uniforms_depth.lightSpaceMatrix == -1 || uniforms_depth.modelMatrix == -1) {
		std::cout << "Uniform error." << std::endl;
		return;
	}
}
void BaseModel::createQuadProgram(char* vertexShaderFile, char* fragmentShaderFile)
{
	program_quad = new ShaderProgram();
	program_quad->createProgram();
	Shader* vs = new Shader();
	Shader* fs = new Shader();
	vs->loadShader(vertexShaderFile, GL_VERTEX_SHADER);
	fs->loadShader(fragmentShaderFile, GL_FRAGMENT_SHADER);
	program_quad->attachShader(vs->getId());
	program_quad->attachShader(fs->getId());
	program_quad->linkProgram();

	program_quad->useProgram();
	
	uniforms_quad.tDepth = glGetUniformLocation(program_quad->getId(), "depthMap");

	if (uniforms_quad.tDepth == -1) {
		std::cout << "Uniform error." << std::endl;
		return;
	}
}

void BaseModel::update(unsigned int dt) {

}

void BaseModel::render() {
	program->useProgram();
	glBindVertexArray(m_shape.vao);

	m_shape.model = translateMatrix * rotateMatrix * scaleMatrix;
	glBindTexture(GL_TEXTURE_2D, m_shape.mTexture);
	glUniformMatrix4fv(uniforms.modelMatrix, 1, GL_FALSE, glm::value_ptr(Scene::getCamera()->getModelMatrix() * m_shape.model));
	glUniformMatrix4fv(uniforms.viewMatrix, 1, GL_FALSE, glm::value_ptr(Scene::getCamera()->getViewMatrix()));
	glUniformMatrix4fv(uniforms.projMatrix, 1, GL_FALSE, glm::value_ptr(Scene::getCamera()->getProjectionMatrix()));

	glUniform3fv(uniforms.cameraPos, 1, glm::value_ptr(cameraPos));
	glUniform3fv(uniforms.lightPos, 1, glm::value_ptr(lightPos));
	glUniform1i(uniforms.useEnvMap, useEnvMap);
	glUniform1i(uniforms.reflectOrRefract, reflectOrRefract);
	glDrawElements(GL_TRIANGLES, m_shape.indexCount, GL_UNSIGNED_INT, 0);
}

void BaseModel::Enable()
{
	program->useProgram();
}

void BaseModel::Disable()
{
	glUseProgram(0);
}

void BaseModel::translate(glm::vec3 vec) {
	translateMatrix = glm::translate(translateMatrix, vec);
}

void BaseModel::rotate(glm::vec3 axis, float angle) {
	rotateMatrix = glm::rotate(rotateMatrix, angle, axis);
}

void BaseModel::scale(glm::vec3 vec) {
	scaleMatrix = glm::scale(scaleMatrix, vec);
}

glm::mat4 BaseModel::getTranslateMatrix()
{
	return translateMatrix;
}

glm::mat4 BaseModel::getRotateMatrix()
{
	return rotateMatrix;
}

glm::mat4 BaseModel::getScaleMatrix()
{
	return scaleMatrix;
}

void BaseModel::SetCameraPos(glm::vec3 _cameraPos){
	//glUniform3fv(uniforms.cameraPos, 1, glm::value_ptr(cameraPos));
	cameraPos = _cameraPos;
}
void BaseModel::SetUseEnvMap(bool _useEnvMap) {
	//glUniform1i(uniforms.reflectOrRefract, reflectOrRefract);
	useEnvMap = _useEnvMap;
}
void BaseModel::SetUseSkyBox(bool _useSkyBox) {
	//glUniform1i(uniforms.reflectOrRefract, reflectOrRefract);
	useSkyBox = _useSkyBox;
}
void BaseModel::SetReflectOrRefract(bool _reflectOrRefract) {
	//glUniform1i(uniforms.reflectOrRefract, reflectOrRefract);
	reflectOrRefract = _reflectOrRefract;
}
void BaseModel::SetShaderMaterialSel(int _shaderMaterialSel) {
	//glUniform1i(uniforms.reflectOrRefract, reflectOrRefract);
	shaderMaterialSel = _shaderMaterialSel;
}
void BaseModel::SetLightSource(LightSource* _light) {
	lightPos = _light->lightPos;
	lightSpaceMatrix = _light->lightSpaceMatrix;
	near_plane = _light->near_plane;
	far_plane = _light->far_plane;
}