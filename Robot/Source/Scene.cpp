#include "../Include/Scene.h"
#include "../Include/RobotModel.h"

ViewManager* Scene::camera;

RobotModel* robot;
Scene::Scene() {
	camera = new ViewManager();

	light = new LightSource();
	light->lightPos = glm::vec3(-3.0f, 6.0f, 2.0f);
	light->near_plane = 4.0f;
	light->far_plane = 25.0f;
	light->lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, light->near_plane, light->far_plane);
	light->lightView = glm::lookAt(light->lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
	light->lightSpaceMatrix = light->lightProjection * light->lightView;

	robot = new RobotModel();
	robot->scale(glm::vec3(0.5, 0.5, 0.5));
	robot->translate(glm::vec3(0, -8.0, 0));
	models.push_back(robot);

	//Skybox
	cubemapShader = new CubemapShader();
	std::vector<CubemapTexture> textures(6);
	textures[0].type = GL_TEXTURE_CUBE_MAP_POSITIVE_X;
	textures[0].fileName = "SkyBox_SanFrancisco/posx.jpg";
	textures[1].type = GL_TEXTURE_CUBE_MAP_NEGATIVE_X;
	textures[1].fileName = "SkyBox_SanFrancisco/negx.jpg";
	textures[2].type = GL_TEXTURE_CUBE_MAP_POSITIVE_Y;
	textures[2].fileName = "SkyBox_SanFrancisco/negy.jpg";
	textures[3].type = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y;
	textures[3].fileName = "SkyBox_SanFrancisco/posy.jpg";
	textures[4].type = GL_TEXTURE_CUBE_MAP_POSITIVE_Z;
	textures[4].fileName = "SkyBox_SanFrancisco/posz.jpg";
	textures[5].type = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z;
	textures[5].fileName = "SkyBox_SanFrancisco/negz.jpg";
	cubemapShader->Init(textures);

	//Asteroid initialization


	//uniform setting
	glUniform1i(glGetUniformLocation(cubemapShader->GetProgramID(), "CubeMap"), 2);
}

void Scene::loadCubeMap(std::string skybox_prefix) {
	std::vector<CubemapTexture> textures(6);
	textures[0].type = GL_TEXTURE_CUBE_MAP_POSITIVE_X;
	textures[0].fileName = skybox_prefix + "/posx.jpg";
	textures[1].type = GL_TEXTURE_CUBE_MAP_NEGATIVE_X;
	textures[1].fileName = skybox_prefix + "/negx.jpg";
	textures[2].type = GL_TEXTURE_CUBE_MAP_POSITIVE_Y;
	textures[2].fileName = skybox_prefix + "/negy.jpg";
	textures[3].type = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y;
	textures[3].fileName = skybox_prefix + "/posy.jpg";
	textures[4].type = GL_TEXTURE_CUBE_MAP_POSITIVE_Z;
	textures[4].fileName = skybox_prefix + "/posz.jpg";
	textures[5].type = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z;
	textures[5].fileName = skybox_prefix + "/negz.jpg";

	cubemapShader->LoadTextures(textures);
}

void Scene::mouseEvent(int button, int state, int x, int y) {
	camera->mouseEvents(button, state, x, y);
}

void Scene::keyboardEvent(int key) {

}

void Scene::keyboardEvent(unsigned char key) {
	camera->keyEvents(key);
	switch (key)
	{
	case 'I':
	case 'i':
		models[0]->translate(glm::vec3(0, 1, 0));
		break;
	case 'J':
	case 'j':
		models[0]->translate(glm::vec3(-1, 0, 0));
		break;
	case 'K':
	case 'k':
		models[0]->translate(glm::vec3(0, -1, 0));
		break;
	case 'L':
	case 'l':
		models[0]->translate(glm::vec3(1, 0, 0));
		break;
	default:
		break;
	}
}

void Scene::menuEvent(int item) {
	robot->setAction(RobotModel::ActionEnum(item));
}
void Scene::effectsMenuEvent(int item) {
	if (item == 0) {
		enEnvMap = false;
		shaderMaterialSel = 0;
	}
	else if (item == 1) {
		enEnvMap = true;
		reflect_refract = false;
		shaderMaterialSel = 0;
	}
	else if (item == 2) {
		enEnvMap = true;
		reflect_refract = true;
		shaderMaterialSel = 0;
	}
	else {  //special skin
		shaderMaterialSel = item - 2;
		enEnvMap = false;
	}
}
void Scene::setSkyBox(int option) {
	if (option == 0)  enSkyBox = false;
	else enSkyBox = true;
	if (option == 0)  loadCubeMap("SkyBox_None");
	else if (option == 1) loadCubeMap("SkyBox_Forest");
	else if (option == 2) loadCubeMap("SkyBox_FortPoint");
	else if (option == 3) loadCubeMap("SkyBox_SanFrancisco");
	else if (option == 4) loadCubeMap("SkyBox_Saint_Peter's_Square");
	else if (option == 5) loadCubeMap("SkyBox_Bridge");
	else if (option == 6) loadCubeMap("SkyBox_Outer_Space");
}
void Scene::setParticle(int option) {
	enParticle = !option;
}
void Scene::setFilter(int option) {
	if (option == 0) filter = Filter::None;
	else if (option == 1) {
		filter = Filter::Mosaic;
		Mosaic_rows = 256.0;
		Mosaic_cols = 256.0;
	}
	else if (option == 2) {
		filter = Filter::Mosaic;
		Mosaic_rows = 64.0;
		Mosaic_cols = 64.0;
	}
	else if (option == 3) {
		filter = Filter::Mosaic;
		Mosaic_rows = 32.0;
		Mosaic_cols = 32.0;
	}
	else if (option == 4) {
		filter = Filter::Motion_Blur;
		mixrate = 0.4;
		for (int i = 0; i < models.size(); ++i) models[i]->SetMixRate(mixrate);
	}
	else if (option == 5) {
		filter = Filter::Motion_Blur;
		mixrate = 0.6;
		for (int i = 0; i < models.size(); ++i) models[i]->SetMixRate(mixrate);
	}
	else if (option == 6) {
		filter = Filter::Motion_Blur;
		mixrate = 0.8;
		for (int i = 0; i < models.size(); ++i) models[i]->SetMixRate(mixrate);
	}
}

void Scene::render() {

	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//render model
	cubemapShader->UseTexture(true);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapShader->GetTextureID());
	for (size_t i = 0; i < models.size(); i++) {
		glm::mat4 modelMat = models[i]->getTranslateMatrix() * models[i]->getRotateMatrix();
		glm::vec3 NewCameraPos = camera->getEyePosition() * glm::mat3(camera->getModelMatrix());
		models[i]->SetLightSource(light);
		models[i]->SetCameraPos(NewCameraPos);
		models[i]->SetUseEnvMap(enEnvMap);
		models[i]->SetReflectOrRefract(reflect_refract);
		models[i]->SetShaderMaterialSel(shaderMaterialSel);
		models[i]->SetUseSkyBox(enSkyBox);
		models[i]->render();
	}
	cubemapShader->UseTexture(false);

	glBindFramebuffer(GL_FRAMEBUFFER, robot->ScreenFBO);

	//skybox
	glBindFramebuffer(GL_FRAMEBUFFER, robot->ScreenFBO);
	glDepthFunc(GL_LEQUAL);
	cubemapShader->Enable();
	cubemapShader->SetMVMat(glm::mat4(glm::mat3(getCamera()->getViewMatrix() * getCamera()->getModelMatrix())));
	cubemapShader->SetPMat(getCamera()->getProjectionMatrix());
	cubemapShader->Render();
	cubemapShader->Disable();
	glDepthFunc(GL_LESS);

	robot->renderToScreen(filter, Mosaic_rows, Mosaic_cols);
}
void Scene::update(unsigned int dt) {
	for (size_t i = 0; i < models.size(); i++) {
		models[i]->update(dt);
	}
}

std::vector<BaseModel*> Scene::getModels() {
	return models;
}