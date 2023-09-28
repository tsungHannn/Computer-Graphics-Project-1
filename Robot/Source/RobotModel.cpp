#include "../Include/RobotModel.h"
#include "../Include/Scene.h"

RobotModel::RobotModel()
{
	init();
}

void RobotModel::init()
{
	createVertexProgram("vertex.vs.glsl", "fragment.fs.glsl");
	createDepthProgram("depthmap.vs.glsl", "depthmap.fs.glsl");
	createQuadProgram("debug_quad.vs.glsl", "debug_quad.fs.glsl");

#pragma region Initialize Parts
	BaseParticleProp* propR = new BaseParticleProp();
	propR->colorBegin = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
	propR->colorEnd = { 254 / 255.0f, 89 / 255.0f, 41 / 255.0f, 1.0f };
	propR->sizeBegin = 0.3f;
	propR->sizeVariation = 0.1f;
	propR->sizeEnd = 0.0f;
	propR->lifeTime = 3000.0f;
	propR->velocity = { 0.0f, -0.005f, 0.0f };
	propR->velocityVariation = { 0.004f, 0.0025f, 0.004f };
	propR->position = { 0.0f, 0.0f, 0.0f };
	propR->active = false;
	propR->emitAmount = 6;
	BaseParticleProp* propL = new BaseParticleProp(*propR);


	robotParts[0] = RobotModelPart(nullptr, "upper_chest.obj", glm::translate(glm::mat4(), glm::vec3(0, 15.857, -0.81)));
	robotParts[1] = RobotModelPart(&robotParts[0], "lower_chest.obj", glm::translate(glm::mat4(), glm::vec3(0, -1.505, 0.3)));
	robotParts[2] = RobotModelPart(&robotParts[1], "belly.obj", glm::translate(glm::mat4(), glm::vec3(0, -1.197, -0.053)));
	robotParts[3] = RobotModelPart(&robotParts[2], "hip.obj", glm::translate(glm::mat4(), glm::vec3(0, -2.723, -0.007)));
	robotParts[4] = RobotModelPart(&robotParts[0], "R_shoulder.obj", glm::translate(glm::mat4(), glm::vec3(-3.323, 0.608, -0.433)));
	robotParts[5] = RobotModelPart(&robotParts[4], "R_upper_arm.obj", glm::translate(glm::mat4(), glm::vec3(-0.89, 0.271, -0.255)));
	robotParts[6] = RobotModelPart(&robotParts[5], "R_lower_arm.obj", glm::translate(glm::mat4(), glm::vec3(0.012, -4.049, 0.015)));
	robotParts[7] = RobotModelPart(&robotParts[6], "R_hand.obj", glm::translate(glm::mat4(), glm::vec3(-0.013, -3.076, -0.046)));
	robotParts[8] = RobotModelPart(&robotParts[0], "L_shoulder.obj", glm::translate(glm::mat4(), glm::vec3(3.332, 0.608, -0.433)));
	robotParts[9] = RobotModelPart(&robotParts[8], "L_upper_arm.obj", glm::translate(glm::mat4(), glm::vec3(0.89, 0.271, -0.255)));
	robotParts[10] = RobotModelPart(&robotParts[9], "L_lower_arm.obj", glm::translate(glm::mat4(), glm::vec3(-0.012, -4.049, 0.015)));
	robotParts[11] = RobotModelPart(&robotParts[10], "L_hand.obj", glm::translate(glm::mat4(), glm::vec3(0.013, -3.076, -0.046)));
	robotParts[12] = RobotModelPart(&robotParts[3], "R_upper_leg.obj", glm::translate(glm::mat4(), glm::vec3(-2, 0, 0)));
	robotParts[13] = RobotModelPart(&robotParts[12], "R_lower_leg.obj", glm::translate(glm::mat4(), glm::vec3(0, -3.737, 0)));
	robotParts[14] = RobotModelPart(&robotParts[13], "R_foot.obj", glm::translate(glm::mat4(), glm::vec3(0, -5.15, 0)));
	robotParts[15] = RobotModelPart(&robotParts[14], propR, glm::translate(glm::mat4(), glm::vec3(0, -1.5, 0)));
	robotParts[16] = RobotModelPart(&robotParts[3], "L_upper_leg.obj", glm::translate(glm::mat4(), glm::vec3(2, 0, 0)));
	robotParts[17] = RobotModelPart(&robotParts[16], "L_lower_leg.obj", glm::translate(glm::mat4(), glm::vec3(0, -3.737, 0)));
	robotParts[18] = RobotModelPart(&robotParts[17], "L_foot.obj", glm::translate(glm::mat4(), glm::vec3(0, -5.15, 0)));
	robotParts[19] = RobotModelPart(&robotParts[18], propL, glm::translate(glm::mat4(), glm::vec3(0, -1.5, 0)));
	robotParts[20] = RobotModelPart(&robotParts[0], "neck.obj", glm::translate(glm::mat4(), glm::vec3(0, 2, 0)));
	robotParts[21] = RobotModelPart(&robotParts[20], "head.obj", glm::translate(glm::mat4(), glm::vec3(0, 0.289, 0)));
#pragma endregion

	loadModel();
	loadAnimation();
	fireParticleEffect = BaseParticleEffect("particle.vs.glsl", "particle.fs.glsl");
}
void RobotModel::update(unsigned int dt)
{
	animation.update(dt);
	fireParticleEffect.update(dt);
	// Late Update
	for (int i = 0; i < PART_SIZE; i++)
	{
		if (robotParts[i].getParent() == nullptr) {
			robotParts[i].shape.model =
				getTranslateMatrix() * getRotateMatrix() * getScaleMatrix() *  // Base element is offset by the entire model
				robotParts[i].offsetMatrix *
				robotParts[i].translateMatrix * robotParts[i].rotateMatrix * robotParts[i].scaleMatrix;
		}
		else {
			robotParts[i].shape.model =
				robotParts[i].getParent()->shape.model *  // Child element is offset by parent
				robotParts[i].offsetMatrix *
				robotParts[i].translateMatrix * robotParts[i].rotateMatrix * robotParts[i].scaleMatrix;
		}

		if (robotParts[i].baseParticleProp != nullptr) {
			// Emit particles
			fireParticleEffect.emit(*robotParts[i].baseParticleProp, robotParts[i].shape.model);
		}
	}
}

void RobotModel::setAction(ActionEnum action)
{
	animation.setState(action);
	animation.play();
}

void RobotModel::reloadAnimations()
{
	loadAnimation();
}

void RobotModel::renderQuad(GLuint& quadVAO, GLuint& quadVBO)
{
	if (quadVAO == 0)
	{
		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

void RobotModel::render()
{
	program_depth->useProgram();
	program_depth->setMat4("lightSpaceMatrix", lightSpaceMatrix);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	program_depth->setMat4("model", glm::mat4(1.0));
	glBindVertexArray(floorVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	for (int i = 0; i < PART_SIZE; i++) {
		if (robotParts[i].baseParticleProp == nullptr) {
			glActiveTexture(GL_TEXTURE3);
			glBindTexture(GL_TEXTURE_2D, depthMap);

			//program_depth->setMat4("model", glm::mat4(1.0));
			program_depth->setMat4("model", robotParts[i].shape.model);
			//render shadow
			glBindVertexArray(robotParts[i].shape.vao);
			glDrawElements(GL_TRIANGLES, robotParts[i].shape.indexCount, GL_UNSIGNED_INT, 0);
		}
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	program->useProgram();
	//Uniform settings
	glUniformMatrix4fv(uniforms.viewMatrix, 1, GL_FALSE, value_ptr(Scene::getCamera()->getViewMatrix() * Scene::getCamera()->getModelMatrix()));
	glUniformMatrix4fv(uniforms.projMatrix, 1, GL_FALSE, value_ptr(Scene::getCamera()->getProjectionMatrix()));

	glUniform3fv(uniforms.cameraPos, 1, glm::value_ptr(cameraPos));
	glUniform3fv(uniforms.lightPos, 1, glm::value_ptr(lightPos));
	glUniform1i(uniforms.useEnvMap, useEnvMap);
	glUniform1i(uniforms.reflectOrRefract, reflectOrRefract);
	glUniform1i(uniforms.shaderMaterial, shaderMaterialSel);
	glUniform1i(uniforms.normalMapping, false);  //enable normal map
	program->setMat4("lightSpaceMatrix", lightSpaceMatrix);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glBindFramebuffer(GL_FRAMEBUFFER, ScreenFBO);
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//render floor
	if (!useSkyBox) {
		glBindVertexArray(floorVAO);
		glUniformMatrix4fv(uniforms.modelMatrix, 1, GL_FALSE, value_ptr(glm::mat4(1.0f)));
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, floorTex);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, floorTex);
		glUniform1i(uniforms.useEnvMap, false);
		glUniform1i(uniforms.shaderMaterial, 0);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniform1i(uniforms.useEnvMap, useEnvMap);
		glUniform1i(uniforms.shaderMaterial, shaderMaterialSel);
	}

	//render robot
	for (int i = 0; i < PART_SIZE; ++i) {
		if (robotParts[i].baseParticleProp == nullptr) {
			//render the scene with shadow
			glBindVertexArray(robotParts[i].shape.vao);
			glUniformMatrix4fv(uniforms.modelMatrix, 1, GL_FALSE, value_ptr(robotParts[i].shape.model));

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, robotParts[i].shape.mTexture);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, robotParts[i].shape.normalTexture);
			glDrawElements(GL_TRIANGLES, robotParts[i].shape.indexCount, GL_UNSIGNED_INT, 0);
		}
	}


	// Render particles
	glBindFramebuffer(GL_FRAMEBUFFER, ScreenFBO);
	fireParticleEffect.render();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RobotModel::loadModel()
{
	TextureData tdata = Common::loadPng("IronGiant_color.png");
	TextureData tdata_normal = Common::loadPng("IronGiant_bump.png");
	TextureData tdata_floor = Common::loadPng("floor.png");

	GLuint texture, texture_normal, texture_floor;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, tdata.width, tdata.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tdata.data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glGenTextures(1, &texture_normal);
	glBindTexture(GL_TEXTURE_2D, texture_normal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, tdata_normal.width, tdata_normal.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tdata_normal.data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glGenTextures(1, &texture_floor);
	glBindTexture(GL_TEXTURE_2D, texture_floor);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, tdata_floor.width, tdata_floor.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tdata_floor.data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	floorTex = texture_floor;

	//depth map
	glGenFramebuffers(1, &depthMapFBO);
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	// attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer error : " << fboStatus << std::endl;

	//Screen map
	glGenFramebuffers(1, &ScreenFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, ScreenFBO);
	glGenTextures(1, &Screen);
	glBindTexture(GL_TEXTURE_2D, Screen);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	GLuint RBO;
	glGenRenderbuffers(1, &RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SHADOW_WIDTH, SHADOW_HEIGHT);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, Screen, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

	fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer error : " << fboStatus << std::endl;

	//inner Screen map
	glGenFramebuffers(1, &InnerScreenFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, InnerScreenFBO);
	glGenTextures(1, &InnerScreen);
	glBindTexture(GL_TEXTURE_2D, InnerScreen);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	GLuint innerRBO;
	glGenRenderbuffers(1, &innerRBO);
	glBindRenderbuffer(GL_RENDERBUFFER, innerRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SHADOW_WIDTH, SHADOW_HEIGHT);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, InnerScreen, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, innerRBO);

	fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer error : " << fboStatus << std::endl;

	//update Screen map
	glGenFramebuffers(1, &updateScreenFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, updateScreenFBO);
	glGenTextures(1, &updateScreen);
	glBindTexture(GL_TEXTURE_2D, updateScreen);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	GLuint updateRBO;
	glGenRenderbuffers(1, &updateRBO);
	glBindRenderbuffer(GL_RENDERBUFFER, updateRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SHADOW_WIDTH, SHADOW_HEIGHT);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, updateScreen, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, updateRBO);

	fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer error : " << fboStatus << std::endl;



	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//floor
	float planeVertices[] = {
		// positions            // normals         // texcoords
		 25.0f, -8.0f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
		-25.0f, -8.0f,  25.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
		-25.0f, -8.0f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,

		 25.0f, -8.0f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
		-25.0f, -8.0f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,
		 25.0f, -8.0f, -25.0f,  0.0f, 1.0f, 0.0f,  25.0f, 25.0f
	};
	// plane VAO
	unsigned int planeVBO;
	glGenVertexArrays(1, &floorVAO);
	glGenBuffers(1, &planeVBO);
	glBindVertexArray(floorVAO);
	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glBindVertexArray(0);


	//Set uniforms
	program->useProgram();
	glUniform1i(uniforms.tDiffuse, 0);
	glUniform1i(uniforms.tNormal, 1);
	glUniform1i(uniforms.tCube, 2);
	glUniform1i(uniforms.tDepth, 3);

	program_quad->useProgram();
	program_quad->setInt("textureMap", 4);
	program_quad->setInt("prevTextureMap", 5);

	program->useProgram();
	for (int i = 0; i < PART_SIZE; i++) {
		if (robotParts[i].getObjFile() == nullptr) {
			continue;
		}
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;

		std::string err;

		bool ret = tinyobj::LoadObj(shapes, materials, err, robotParts[i].getObjFile());
		if (err.size() > 0)
		{
			printf("Model loading failed!");
			return;
		}

		for (size_t j = 0; j < shapes.size(); j++)
		{
			glGenVertexArrays(1, &robotParts[i].shape.vao);
			glBindVertexArray(robotParts[i].shape.vao);

			glGenBuffers(3, &robotParts[i].shape.vbo);
			glGenBuffers(1, &robotParts[i].shape.pNormal);

			//Calc tangents & bitangents
			std::vector<float> tangents(shapes[j].mesh.positions.size(), 0.0), bitangents(shapes[j].mesh.positions.size(), 0.0);
			int ind = 0, n = shapes[j].mesh.indices.size() - 2;
			while (ind < n) {
				//unsigned int idx1 = shapes[j].mesh.indices[ind], idx2 = shapes[j].mesh.indices[ind + 1], idx3 = shapes[j].mesh.indices[ind + 2];
				glm::vec3 pos1 = glm::vec3(shapes[j].mesh.positions[shapes[j].mesh.indices[ind] * 3], shapes[j].mesh.positions[shapes[j].mesh.indices[ind] * 3 + 1], shapes[j].mesh.positions[shapes[j].mesh.indices[ind] * 3 + 2]);
				glm::vec3 pos2 = glm::vec3(shapes[j].mesh.positions[shapes[j].mesh.indices[ind + 1] * 3], shapes[j].mesh.positions[shapes[j].mesh.indices[ind + 1] * 3 + 1], shapes[j].mesh.positions[shapes[j].mesh.indices[ind + 1] * 3 + 2]);
				glm::vec3 pos3 = glm::vec3(shapes[j].mesh.positions[shapes[j].mesh.indices[ind + 2] * 3], shapes[j].mesh.positions[shapes[j].mesh.indices[ind + 2] * 3 + 1], shapes[j].mesh.positions[shapes[j].mesh.indices[ind + 2] * 3 + 2]);
				glm::vec2 uv1 = glm::vec2(shapes[j].mesh.texcoords[shapes[j].mesh.indices[ind] * 2], shapes[j].mesh.texcoords[shapes[j].mesh.indices[ind] * 2 + 1]);
				glm::vec2 uv2 = glm::vec2(shapes[j].mesh.texcoords[shapes[j].mesh.indices[ind + 1] * 2], shapes[j].mesh.texcoords[shapes[j].mesh.indices[ind + 1] * 2 + 1]);
				glm::vec2 uv3 = glm::vec2(shapes[j].mesh.texcoords[shapes[j].mesh.indices[ind + 2] * 2], shapes[j].mesh.texcoords[shapes[j].mesh.indices[ind + 2] * 2 + 1]);

				glm::vec3 tangent, bitangent;
				glm::vec3 edge1 = pos2 - pos1;
				glm::vec3 edge2 = pos3 - pos1;
				glm::vec2 deltaUV1 = uv2 - uv1;
				glm::vec2 deltaUV2 = uv3 - uv1;

				float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
				tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
				tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
				tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
				tangent = glm::normalize(tangent);

				bitangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
				bitangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
				bitangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
				bitangent = glm::normalize(bitangent);

				for (int k = 0; k < 3; ++k) {
					tangents[shapes[j].mesh.indices[ind + k] * 3] = tangent.x;
					tangents[shapes[j].mesh.indices[ind + k] * 3 + 1] = tangent.y;
					tangents[shapes[j].mesh.indices[ind + k] * 3 + 2] = tangent.z;

					bitangents[shapes[j].mesh.indices[ind + k] * 3] = bitangent.x;
					bitangents[shapes[j].mesh.indices[ind + k] * 3 + 1] = bitangent.y;
					bitangents[shapes[j].mesh.indices[ind + k] * 3 + 2] = bitangent.z;
				}

				ind += 3;
			}
			//std::cout << shapes[j].mesh.positions.size() << " " << shapes[j].mesh.normals.size() << " " << tangents.size()<< std::endl;


			glBindBuffer(GL_ARRAY_BUFFER, robotParts[i].shape.vbo);
			glBufferData(GL_ARRAY_BUFFER, shapes[j].mesh.positions.size() * sizeof(float) + shapes[j].mesh.normals.size() * sizeof(float) + tangents.size() * sizeof(float) + bitangents.size() * sizeof(float), NULL, GL_STATIC_DRAW);

			glBufferSubData(GL_ARRAY_BUFFER, 0, shapes[j].mesh.positions.size() * sizeof(float), &shapes[j].mesh.positions[0]);
			glBufferSubData(GL_ARRAY_BUFFER, shapes[j].mesh.positions.size() * sizeof(float), shapes[j].mesh.normals.size() * sizeof(float), &shapes[j].mesh.normals[0]);
			glBufferSubData(GL_ARRAY_BUFFER, shapes[j].mesh.positions.size() * sizeof(float) + shapes[j].mesh.normals.size() * sizeof(float), tangents.size() * sizeof(float), &tangents[0]);
			glBufferSubData(GL_ARRAY_BUFFER, shapes[j].mesh.positions.size() * sizeof(float) + shapes[j].mesh.normals.size() * sizeof(float) + tangents.size() * sizeof(float), bitangents.size() * sizeof(float), &bitangents[0]);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)(shapes[j].mesh.positions.size() * sizeof(float)));
			glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (void*)(shapes[j].mesh.positions.size() * sizeof(float) + shapes[j].mesh.normals.size() * sizeof(float)));
			glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, (void*)(shapes[j].mesh.positions.size() * sizeof(float) + shapes[j].mesh.normals.size() * sizeof(float) + tangents.size() * sizeof(float)));

			glBindBuffer(GL_ARRAY_BUFFER, robotParts[i].shape.pNormal);
			glBufferData(GL_ARRAY_BUFFER, shapes[j].mesh.normals.size() * sizeof(float), shapes[j].mesh.normals.data(), GL_STATIC_DRAW);
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

			glBindBuffer(GL_ARRAY_BUFFER, robotParts[i].shape.vboTex);
			glBufferData(GL_ARRAY_BUFFER, shapes[j].mesh.texcoords.size() * sizeof(float), shapes[j].mesh.texcoords.data(), GL_STATIC_DRAW);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, robotParts[i].shape.ebo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, shapes[j].mesh.indices.size() * sizeof(unsigned int), shapes[j].mesh.indices.data(), GL_STATIC_DRAW);

			robotParts[i].shape.materialId = shapes[j].mesh.material_ids[0];
			robotParts[i].shape.indexCount = shapes[j].mesh.indices.size();
			robotParts[i].shape.mTexture = texture;
			robotParts[i].shape.normalTexture = texture_normal;

			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
			glEnableVertexAttribArray(2);
			glEnableVertexAttribArray(3);
			glEnableVertexAttribArray(4);
		}
	}
}

void RobotModel::createVertexProgram(char* vertexShaderFile, char* fragmentShaderFile)
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

	program->useProgram();
	uniforms.projMatrix = glGetUniformLocation(program->getId(), "um4p");
	uniforms.modelMatrix = glGetUniformLocation(program->getId(), "um4m");
	uniforms.viewMatrix = glGetUniformLocation(program->getId(), "um4v");

	uniforms.cameraPos = glGetUniformLocation(program->getId(), "cameraPos");
	uniforms.lightPos = glGetUniformLocation(program->getId(), "lightPos");
	uniforms.useEnvMap = glGetUniformLocation(program->getId(), "useEnvMap");
	uniforms.reflectOrRefract = glGetUniformLocation(program->getId(), "reflectOrRefract");
	uniforms.shaderMaterial = glGetUniformLocation(program->getId(), "shaderMaterialID");
	uniforms.normalMapping = glGetUniformLocation(program->getId(), "normalMapping");

	uniforms.tDiffuse = glGetUniformLocation(program->getId(), "diffuseMap");
	uniforms.tNormal = glGetUniformLocation(program->getId(), "normalMap");
	uniforms.tCube = glGetUniformLocation(program->getId(), "cubeMap");
	uniforms.tDepth = glGetUniformLocation(program->getId(), "depthMap");

	glDeleteShader(vs->getId());
	glDeleteShader(fs->getId());
}
void RobotModel::createDepthProgram(char* vertexShaderFile, char* fragmentShaderFile)
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
		std::cout << "Depth Uniform error." << std::endl;
		return;
	}
	glDeleteShader(vs->getId());
	glDeleteShader(fs->getId());
}
void RobotModel::createQuadProgram(char* vertexShaderFile, char* fragmentShaderFile)
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

	uniforms_quad.tDepth = glGetUniformLocation(program_quad->getId(), "textureMap");

	if (uniforms_quad.tDepth == -1) {
		std::cout << "Quad Uniform error." << std::endl;
		return;
	}

	glDeleteShader(vs->getId());
	glDeleteShader(fs->getId());
}

void RobotModel::renderToScreen(int _filter_selection, float rows, float cols) {
	//apply filter
	glBindFramebuffer(GL_FRAMEBUFFER, updateScreenFBO);
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	program_quad->useProgram();
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, Screen);
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, InnerScreen);
	program_quad->setInt("filter_selection", _filter_selection);
	program_quad->setFloat("mixRate", mixrate);
	program_quad->setFloat("rows", rows);
	program_quad->setFloat("cols", cols);
	renderQuad(quadVAO, quadVBO);

	//update inner screen
	glBindFramebuffer(GL_FRAMEBUFFER, InnerScreenFBO);
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, updateScreen);
	program_quad->setInt("filter_selection", 0);
	renderQuad(quadVAO, quadVBO);


	//render to the screen
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, updateScreen);
	program_quad->setInt("filter_selection", 0);
	renderQuad(quadVAO, quadVBO);
}
void RobotModel::loadAnimation()
{
	animation = Animation<RobotModelPart, ActionEnum>();

	AnimationClip<RobotModelPart> idleClip = AnimationClip<RobotModelPart>(robotParts, sizeof(robotParts) / sizeof(RobotModelPart), RobotModelPart::applyFunc, RobotModelPart::resetFunc);
	idleClip.parseFile("robot_anim_idle.csv");
	animation.addClip(ActionEnum::Idle, idleClip);

	AnimationClip<RobotModelPart> walkClip = AnimationClip<RobotModelPart>(robotParts, sizeof(robotParts) / sizeof(RobotModelPart), RobotModelPart::applyFunc, RobotModelPart::resetFunc);
	walkClip.parseFile("robot_anim_walk.csv");
	animation.addClip(ActionEnum::Walk, walkClip);

	AnimationClip<RobotModelPart> runClip = AnimationClip<RobotModelPart>(robotParts, sizeof(robotParts) / sizeof(RobotModelPart), RobotModelPart::applyFunc, RobotModelPart::resetFunc);
	runClip.parseFile("robot_anim_run.csv");
	animation.addClip(ActionEnum::Run, runClip);

	AnimationClip<RobotModelPart> jumpingJackClip = AnimationClip<RobotModelPart>(robotParts, sizeof(robotParts) / sizeof(RobotModelPart), RobotModelPart::applyFunc, RobotModelPart::resetFunc);
	jumpingJackClip.parseFile("robot_anim_jumpingjack.csv");
	animation.addClip(ActionEnum::JumpingJack, jumpingJackClip);

	AnimationClip<RobotModelPart> squatClip = AnimationClip<RobotModelPart>(robotParts, sizeof(robotParts) / sizeof(RobotModelPart), RobotModelPart::applyFunc, RobotModelPart::resetFunc);
	squatClip.parseFile("robot_anim_squat.csv");
	animation.addClip(ActionEnum::Squat, squatClip);

	AnimationClip<RobotModelPart> moonwalkClip = AnimationClip<RobotModelPart>(robotParts, sizeof(robotParts) / sizeof(RobotModelPart), RobotModelPart::applyFunc, RobotModelPart::resetFunc);
	moonwalkClip.parseFile("robot_anim_moonwalk.csv");
	animation.addClip(ActionEnum::Moonwalk, moonwalkClip);

	AnimationClip<RobotModelPart> gangnamStyleClip = AnimationClip<RobotModelPart>(robotParts, sizeof(robotParts) / sizeof(RobotModelPart), RobotModelPart::applyFunc, RobotModelPart::resetFunc);
	gangnamStyleClip.parseFile("robot_anim_gangnamstyle.csv");
	animation.addClip(ActionEnum::GangnamStyle, gangnamStyleClip);

	AnimationClip<RobotModelPart> headTrickClip = AnimationClip<RobotModelPart>(robotParts, sizeof(robotParts) / sizeof(RobotModelPart), RobotModelPart::applyFunc, RobotModelPart::resetFunc);
	headTrickClip.parseFile("robot_anim_headtrick.csv");
	animation.addClip(ActionEnum::HeadTrick, headTrickClip);

	AnimationClip<RobotModelPart> pushupClip = AnimationClip<RobotModelPart>(robotParts, sizeof(robotParts) / sizeof(RobotModelPart), RobotModelPart::applyFunc, RobotModelPart::resetFunc);
	pushupClip.parseFile("robot_anim_pushup.csv");
	animation.addClip(ActionEnum::Pushup, pushupClip);

	AnimationClip<RobotModelPart> flyClip = AnimationClip<RobotModelPart>(robotParts, sizeof(robotParts) / sizeof(RobotModelPart), RobotModelPart::applyFunc, RobotModelPart::resetFunc);
	flyClip.parseFile("robot_anim_fly.csv");
	animation.addClip(ActionEnum::Fly, flyClip);

	animation.setState(ActionEnum::Idle);
	animation.setLoop(true);
	animation.play();
}
