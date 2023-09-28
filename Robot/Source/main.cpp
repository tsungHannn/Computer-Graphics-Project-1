#include "../Include/Common.h"
#include "../Include/ViewManager.h"
#include "../Include/Scene.h"
#include "../Include/RobotModel.h"

#define MENU_Sale 1
#define MENU_Shrink 2
#define MENU_EXIT   3

using namespace glm;
using namespace std;

unsigned int timerInterval = 0;
unsigned int prevUpdateTime = 0;
unsigned int prevFixedUpdateTime = 0;
unsigned int fixedUpdateInterval = 50;
bool firstUpdate = false;

Scene* scene;

void init()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	scene = new Scene();
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	scene->render();
	glutSwapBuffers();
}

void reshape(int width, int height)
{
	scene->getCamera()->setWindowSize(width, height);
	glViewport(0, 0, width, height);
}

void timer(int val)
{
	unsigned int currentTime = glutGet(GLUT_ELAPSED_TIME);
	if (!firstUpdate) {
		firstUpdate = true;
		prevUpdateTime = currentTime;
	}
	if (currentTime - prevFixedUpdateTime >= fixedUpdateInterval) {
		// TODO: FixedUpdate
		prevFixedUpdateTime = currentTime;
	}
	unsigned int deltaTime = currentTime - prevUpdateTime;
	scene->update(deltaTime);
	//TODO: LateUpdate
	prevUpdateTime = currentTime;
	glutPostRedisplay();
	glutTimerFunc(timerInterval, timer, val);
}

void mouse(int button, int state, int x, int y)
{
	scene->mouseEvent(button, state, x, y);

	if (button == GLUT_LEFT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			printf("Mouse %d is pressed at (%d, %d)\n", button, x, y);
		}
		else if (state == GLUT_UP)
		{
			printf("Mouse %d is released at (%d, %d)\n", button, x, y);
		}
	}
	else if (button == GLUT_RIGHT_BUTTON)
	{
		printf("Mouse %d is pressed\n", button);
	}
}

void keyboard(unsigned char key, int x, int y)
{
	scene->keyboardEvent(key);
}

void specialFunc(int key, int x, int y)
{
	scene->keyboardEvent(key);
}

void motion(int x, int y) {
	scene->getCamera()->mouseMoveEvent(x, y);
}

#pragma region 選單
void leftMouseMenuEvent(int option)
{
}
void actionMenuEvent(int option)
{
	scene->menuEvent(option);
}
void modeMenuEvent(int option)
{
	switch (option) {
	case 0:
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		break;
	case 1:
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	}
}
void effectsMenuEvent(int option)
{
	scene->effectsMenuEvent(option);
}
void skyboxMenuEvent(int option)
{
	scene->setSkyBox(option);
}
void particleMenuEvent(int option)
{
	scene->setParticle(option);
}
void filterMenuEvent(int option)
{
	scene->setFilter(option);
}
void loadMenu() {
	int actionMenu = glutCreateMenu(actionMenuEvent);
	glutAddMenuEntry("Idle", (int)RobotModel::ActionEnum::Idle);
	glutAddMenuEntry("Walk", (int)RobotModel::ActionEnum::Walk);
	glutAddMenuEntry("Run", (int)RobotModel::ActionEnum::Run);
	glutAddMenuEntry("Jumping Jack", (int)RobotModel::ActionEnum::JumpingJack);
	glutAddMenuEntry("Sqaut", (int)RobotModel::ActionEnum::Squat);
	glutAddMenuEntry("Moonwalk", (int)RobotModel::ActionEnum::Moonwalk);
	glutAddMenuEntry("Gangnam Style", (int)RobotModel::ActionEnum::GangnamStyle);
	glutAddMenuEntry("Head Trick", (int)RobotModel::ActionEnum::HeadTrick);
	glutAddMenuEntry("Pushup", (int)RobotModel::ActionEnum::Pushup);
	glutAddMenuEntry("Fly", (int)RobotModel::ActionEnum::Fly);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	int modeMenu = glutCreateMenu(modeMenuEvent);
	glutAddMenuEntry("Line", 0);
	glutAddMenuEntry("Fill", 1);
	int effectsMenu = glutCreateMenu(effectsMenuEvent);
	glutAddMenuEntry("None", 0);
	glutAddMenuEntry("Reflect", 1);
	glutAddMenuEntry("Refract", 2);
	glutAddMenuEntry("White Noise", 3);
	glutAddMenuEntry("Rusty", 4);
	glutAddMenuEntry("Alien", 5);
	int skyboxMenu = glutCreateMenu(skyboxMenuEvent);
	glutAddMenuEntry("None", 0);
	glutAddMenuEntry("Forest", 1);
	glutAddMenuEntry("FortPoint", 2);
	glutAddMenuEntry("SanFrancisco", 3);
	glutAddMenuEntry("Saint Peter's Square", 4);
	glutAddMenuEntry("The Bridge", 5);
	glutAddMenuEntry("Outer Space", 6);
	int filterMenu = glutCreateMenu(filterMenuEvent);
	glutAddMenuEntry("None", 0);
	glutAddMenuEntry("Mosaic (weak)", 1);
	glutAddMenuEntry("Mosaic", 2);
	glutAddMenuEntry("Mosaic (strong)", 3);
	glutAddMenuEntry("Motion blur (weak)", 4);
	glutAddMenuEntry("Motion blur", 5);
	glutAddMenuEntry("Motion blur (strong)", 6);

	glutAttachMenu(GLUT_RIGHT_BUTTON);
	glutCreateMenu(leftMouseMenuEvent);
	glutAddSubMenu("Action", actionMenu);
	glutAddSubMenu("Mode", modeMenu);
	glutAddSubMenu("Effects", effectsMenu);
	glutAddSubMenu("Skybox", skyboxMenu);
	glutAddSubMenu("Filter", filterMenu);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}
#pragma endregion

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

	glutInitWindowPosition(10, 10);
	glutInitWindowSize(1024, 1024);
	glutCreateWindow("Robot");
	glewInit();

	Common::dumpInfo();

	init();

	loadMenu();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(specialFunc);
	glutPassiveMotionFunc(motion);
	glutMotionFunc(motion);
	glutTimerFunc(timerInterval, timer, 0);
	glutMainLoop();

	return 0;
}
