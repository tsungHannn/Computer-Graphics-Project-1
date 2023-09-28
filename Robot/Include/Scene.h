#pragma once

#include <vector>
#include "ViewManager.h"
#include "BaseModel.h"
#include "CubeMap.h"

class Scene
{
public:
	Scene();
	void mouseEvent(int button, int state, int x, int y);
	void keyboardEvent(int key);
	void keyboardEvent(unsigned char key);
	void menuEvent(int item);
	void effectsMenuEvent(int item);
	void setSkyBox(int option);
	void setParticle(int option);
	void setFilter(int option);

	void render();
	void update(unsigned int dt);

	static ViewManager* getCamera() { return camera; }
	std::vector<BaseModel*> getModels();
	void loadCubeMap(std::string skybox_prefix);
private:
	enum Anime_Mode {
		IDLE = 0, WALK, RUN, JUMPING_JACK, SQUAT, MOONWALK, GANGNAM_STYLE
	};
	std::vector<BaseModel*> models;
	CubemapShader* cubemapShader;
	static ViewManager* camera;
	LightSource* light;

	//special effects
	bool enEnvMap = false, reflect_refract = false;  //true for reflect ,false for refract
	bool enSkyBox = true;

	//not used
	bool enParticle = false;


	enum Filter {
		None = 0, Mosaic = 1, Motion_Blur = 2
	};
	int filter = 0;
	float mixrate = 0.5;
	float Mosaic_rows = 64.0, Mosaic_cols = 64.0;

	//shaderMaterialSel
	int  shaderMaterialSel = 0;
};
