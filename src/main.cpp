#define _CRT_SECURE_NO_WARNINGS

#include <ctime>

#include "glad/glad.h"
#include "glfw3.h"
#include "MyAntTweakBar.h"

#include "ViewOpenGL.h"
#include "ProjLabShaderProgramProvider.h"

// OpenGL major and minor versions
int majorVersion = 3, minorVersion = 0;
unsigned int windowWidth, windowHeight;
GLFWwindow* window;

// handle of the shader program
unsigned int simpleShaderProgram;
unsigned int particleDrawShaderProgram;

// The virtual world: collection of Particles
ViewOpenGL view;
Model model;

double prevTimeSecs = 0.0;
float elapsedTimeAfterLastFpsSeconds = 0.0f;
float frameCount = 0.0f;

float counterForFpsAvg = 0;
float fpsSum = 0.0f;

// Idle event indicating that some time elapsed: do animation here
void animateModel() 
{
	double timeSecs = glfwGetTime();  // elapsed time since the start of the program
	float dt = timeSecs - prevTimeSecs;
	prevTimeSecs = timeSecs;

	model.animate(dt);

	elapsedTimeAfterLastFpsSeconds += dt;
	frameCount++;
	if (elapsedTimeAfterLastFpsSeconds >= 1.0f)
	{
		if (counterForFpsAvg > 3600.0f)
		{
			counterForFpsAvg = 0;
			fpsSum = 0.0f;
		}
		float fps = frameCount / elapsedTimeAfterLastFpsSeconds;
		counterForFpsAvg++;
		fpsSum += fps;

		printf("fps: %d\n", (int)roundf(fps));
		printf("Average fps: %.2f\n\n", fpsSum / counterForFpsAvg);
		elapsedTimeAfterLastFpsSeconds = 0.0f;
		frameCount = 0.0f;
	}
}

# ifdef VISUAL_S
char prtFolder[100] = "./simulation";
#else
char prtFolder[100] = "./simulation";
# endif

//  Callback function called when the 'AutoRotate' variable value of the tweak bar has changed
void TW_CALL setFreemode(const void *value, void *clientData)
{
	Camera & camera = model.camera;
	camera.setFreeMode(*(bool*)value);
}

//  Callback function called by the tweak bar to get the 'AutoRotate' value
void TW_CALL getFreemode(void *value, void *clientData)
{
	Camera & camera = model.camera;
	*(bool*)value = camera.freemode; // copy g_AutoRotate to value
}

void TW_CALL setDirection(const void *value, void *clientData)
{
	Camera & camera = model.camera;
	camera.setOrientation(((const float*)value)[0], ((const float*)value)[1], ((const float*)value)[2]);
}

void TW_CALL getDirection(void *value, void *clientData)
{
	Camera & camera = model.camera;
	float dir[3];
	glm::vec3 d = camera.getOrientation();

	dir[0] = d.x;
	dir[1] = d.y;
	dir[2] = d.z;

	((float*)value)[0] = dir[0];
	((float*)value)[1] = dir[1];
	((float*)value)[2] = dir[2];
}

void TW_CALL setUseLights(const void *value, void *clientData)
{
	view.setUseLights(*(const bool*)value);
}
void TW_CALL getUseLights(void *value, void *clientData)
{
	*(bool*)value = view.getUseLights();
}

void TW_CALL setDistance(const void *value, void *clientData)
{
	Camera & camera = model.camera;
	camera.setDistance(*(const float*)value);
}

void TW_CALL lightSource1SetDistance(const void *value, void *clientData)
{
	model.lightSource1.position = (*(const float*)value)*glm::normalize(model.lightSource1.position);
}

void TW_CALL lightSource2SetDistance(const void *value, void *clientData)
{
	model.lightSource2.position = (*(const float*)value)*glm::normalize(model.lightSource2.position);
}

void TW_CALL lightSource1SetDirection(const void *value, void *clientData)
{
	model.lightSource1.position = 
		-glm::length(model.lightSource1.position)*glm::vec3(((const float*)value)[0], ((const float*)value)[1], ((const float*)value)[2]);
}

void TW_CALL lightSource2SetDirection(const void *value, void *clientData)
{
	model.lightSource2.position = 
		-glm::length(model.lightSource2.position)*glm::vec3(((const float*)value)[0], ((const float*)value)[1], ((const float*)value)[2]);
}

void TW_CALL lightSource1GetDirection(void *value, void *clientData)
{
	glm::vec3 directionVector = glm::normalize(model.lightSource1.position);

	((float*)value)[0] = -directionVector.x;
	((float*)value)[1] = -directionVector.y;
	((float*)value)[2] = -directionVector.z;
}

void TW_CALL lightSource2GetDirection(void *value, void *clientData)
{
	glm::vec3 directionVector = glm::normalize(model.lightSource2.position);

	((float*)value)[0] = -directionVector.x;
	((float*)value)[1] = -directionVector.y;
	((float*)value)[2] = -directionVector.z;
}

void TW_CALL lightSource1GetDistance(void *value, void *clientData)
{
	*(float*)value = glm::length(model.lightSource1.position);
}
void TW_CALL lightSource2GetDistance(void *value, void *clientData)
{
	*(float*)value = glm::length(model.lightSource2.position);
}

//  Callback function called by the tweak bar to get the 'AutoRotate' value
void TW_CALL getDistance(void *value, void *clientData)
{
	Camera & camera = model.camera;
	*(float*)value = camera.getDistance(); // copy g_AutoRotate to value
}

void TW_CALL getPrtFolder(void *value, void *clientData)
{
    //char *dest = (char *)value;
}

void TW_CALL loaderButton(void *clientData)
{
	ParticleSystem *particleSystem = model.particleSystem;
    if(!particleSystem->loadFiles(getPrtFiles(prtFolder)))
    {
        char str[25] = "prt files not found";
        strcpy(prtFolder,str);
    }
    view.initializeParticleCenterBuffers(&model);


	//For calculating average fps
	counterForFpsAvg = 0;
	fpsSum = 0.0f;
}

void windowResized(GLFWwindow* window, int width, int height)
{
	windowWidth = width;
	windowHeight = height;
	
	view.setWindowSize(width, height);
	TwWindowSize(width, height);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	TwEventMouseButtonGLFW3(window, button, action, mods);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Camera & camera = model.camera;
	if (key == GLFW_KEY_W && action == GLFW_PRESS) 
	{
		camera.keyDown('w');
	}
	if (key == GLFW_KEY_A && action == GLFW_PRESS) 
	{
		camera.keyDown('a');
	}
	if (key == GLFW_KEY_S && action == GLFW_PRESS)
	{
		camera.keyDown('s');
	}
	if (key == GLFW_KEY_D && action == GLFW_PRESS)
	{
		camera.keyDown('d');
	}
	if (key == GLFW_KEY_E && action == GLFW_PRESS) 
	{
		camera.keyDown('e');
	}
	if (key == GLFW_KEY_Q && action == GLFW_PRESS) 
	{
		camera.keyDown('q');
	}
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) 
	{
		camera.keyDown(' ');
	}
	if (key == GLFW_KEY_W && action == GLFW_RELEASE)
	{
		camera.keyUp('w');
	}
	if (key == GLFW_KEY_A && action == GLFW_RELEASE) 
	{
		camera.keyUp('a');
	}
	if (key == GLFW_KEY_S && action == GLFW_RELEASE) 
	{
		camera.keyUp('s');
	}
	if (key == GLFW_KEY_D && action == GLFW_RELEASE) 
	{
		camera.keyUp('d');
	}
	if (key == GLFW_KEY_E && action == GLFW_RELEASE) 
	{
		camera.keyUp('e');
	}
	if (key == GLFW_KEY_Q && action == GLFW_RELEASE) 
	{
		camera.keyUp('q');
	}
	if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE) 
	{
		camera.keyUp(' ');
	}
	TwEventKeyGLFW3(window, key, scancode, action, mods);
}

float prevX = 0, prevY = 0;
bool first = true;
static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	Camera& camera = model.camera;
	if (first)
	{
		prevX = xpos;
		prevY = ypos;
		first = false;
	}
	else
	{
		camera.mouseMotion(xpos - prevX, ypos - prevY);
		prevX = xpos;
		prevY = ypos;
	}
	TwEventCursorPosGLFW3(window, xpos,ypos);
}

int initializeGLFW() 
{
	windowWidth = 1680;
	windowHeight = 1050;
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return -1;
	}
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(windowWidth, windowHeight, "Project laboratory", NULL, NULL);
	if (window == NULL) 
	{
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	gladLoadGL();

    return 0;
}

void initializeAntTweakBar() 
{
	ParticleSystem *particleSystem = model.particleSystem;
	Camera *camera = &model.camera;

	TwInit(TW_OPENGL_CORE, NULL);
	TwWindowSize(windowWidth, windowHeight);
	TwBar *bar;
	bar = TwNewBar("bar");
	TwDefine("bar label='Settings' position='10 10' size='220 470' ");
	TwAddButton(bar, "load", loaderButton, NULL, " label='Load file' ");
	TwAddVarRW(bar, "pause", TW_TYPE_BOOL8, &particleSystem->pause,
		"label='pause' key=return help='pause the simulation'");
	TwAddVarCB(bar, "cameraDirection", TW_TYPE_DIR3F, setDirection, getDirection, NULL,
		"group='display' label='camera direction' opened=false help='Change the camera direction in target mode.' ");
	TwAddVarRW(bar, "Zoom", TW_TYPE_FLOAT, &camera->viewAngle,
		"group='display' min=10.0 max=90.0 step=0.4 keyIncr=q keyDecr=e help='Adjust the FOV of the camera.' ");
	TwAddVarCB(bar, "cameraDistance", TW_TYPE_FLOAT, setDistance, getDistance, NULL,
		"group='display' label = 'camera distance' min=1.0  step=0.3 help = 'Adjust the distance of the camera in target mode' ");
	TwAddVarCB(bar, "AutoRotate", TW_TYPE_BOOL8, setFreemode, getFreemode, NULL,
		"group='display' label = 'free mode' key=space help = 'Change to free mode.' ");
	TwAddVarRW(bar, "drawCube", TW_TYPE_BOOL8, &view.drawCube,
		"group='display' label='show 3D grid' help='Draw a cube around the 3D grid' ");
	TwAddVarRW(bar, "frameSpeed", TW_TYPE_FLOAT, &particleSystem->filePerSec,
		"group='display' label='max fps' min=1.0 max=1000.0 step=0.2 help='Adjust the number of prt file frames in one second' ");
	TwAddVarRW(bar, "prtFolder", TW_TYPE_CSSTRING(100), prtFolder,
		"group='file parameters' label='prt folder' help='This is the folder of the prt files.' ");
	TwAddVarRW(bar, "loadBefore", TW_TYPE_BOOL8, &particleSystem->loadMode,
		"group='file parameters' label='load before' help='true: Loading the particles from all files before rendering\nfalse: Loading the files while rendering' ");
	TwAddVarRW(bar, "onlyFileParticles", TW_TYPE_BOOL8, &particleSystem->onlyFileParticles,
		"group='file parameters' label='no generation'  help='If its true, the program doesnt generate particles' ");
	TwAddVarRW(bar, "generatedParticleFile", TW_TYPE_INT32, &particleSystem->generatedParticleFile,
		"group='file parameters' label='generation base'  help='The number of the frame that contains the control particles for generating.' ");
	TwAddVarRW(bar, "cubeLength", TW_TYPE_FLOAT, &particleSystem->cubeLength,
		"group='3D array' min=0.0001 label='3D grid width' step=0.1 help='Adjust the width of the space, where the 3D grid is applied' ");
	TwAddVarRW(bar, "splitting", TW_TYPE_INT32, &particleSystem->velocityArrayParts,
		"group='3D array' min=1 max=100 step=1 label='grid splitting' help='Adjust the number of vectors in one 3d Array row' ");
	TwAddVarRW(bar, "genPerParticle", TW_TYPE_INT32, &particleSystem->genPerParticle,
		"group='generated particles' label='num of particles' min=1 step=1 max=100 help='Adjust the number of particles, that generated for each file particle' ");
	TwAddVarRW(bar, "atwR", TW_TYPE_FLOAT, &particleSystem->atwR,
		"group='generated particles' min=0.1 label='max distance' help='adjust the max distance from the original particle'");
	TwAddVarRW(bar, "particleColor", TW_TYPE_COLOR3F, &particleSystem->generatedParticleColor,
		" group='generated particles' label='particle color' help='Change the color of the particles in load time' ");
	TwAddVarRW(bar, "ignoreNullVelocity", TW_TYPE_BOOL8, &particleSystem->ignoreNullVelocity,
		"group='Corrections' label='ignore null velocity' help='ignore the 0 elements when average the velocity'");
	TwAddVarRW(bar, "followControllPoints", TW_TYPE_BOOL8, &particleSystem->followControllPoints,
		"group='Corrections' label='follow controllpoints' help='true: Ha az eredeti pont tul messze van, a generalt pont kovetni fogja.'");
	TwAddVarRW(bar, "distanceMultiplier", TW_TYPE_FLOAT, &particleSystem->distanceMultiplier,
		"group='Corrections' label='follow speed' max=1 min=0 help='A kovetes sebessege'");
	TwAddVarRW(bar, "randomR", TW_TYPE_FLOAT, &particleSystem->randomR,
		"group='Corrections' label='random r' min=0 help='Ha nem 0, nem fogja teljesen az eredeti pontot kovetni'");
	TwAddVarRW(bar, "triggerDistance", TW_TYPE_FLOAT, &particleSystem->triggerDistance,
		"group='Corrections' label='trigger distance' min=0 help='A tavolsag, ami folott korrigalja a pont poziciojat'");

	TwBar *lightsBar = TwNewBar("Lights");
	TwDefine(" Lights label='Light settings' position='10 520' size='220 320' help='Felulet a fenyforrasok tulajdonsagainak beallitasara.' ");
	
	TwAddVarCB(lightsBar, "useLights", TW_TYPE_BOOL8, setUseLights, getUseLights, NULL,
		"group='' label = 'use lights' key=l help = 'Change to free mode.' ");
	TwAddVarRW(lightsBar, "particleSmoothness", TW_TYPE_FLOAT, &view.particleSmoothness,
		"group='' label='particle smoothness' min=0.1  max=10.0 step=0.1 help='Smootness of the surface of the particles'");

	TwAddVarCB(lightsBar, "lightSource1Direction", TW_TYPE_DIR3F, lightSource1SetDirection, lightSource1GetDirection, NULL,
		"group='light source 1' label='direction' opened=false help='Change the direction of the first light source' ");
	TwAddVarCB(lightsBar, "lightSource1Distance", TW_TYPE_FLOAT, lightSource1SetDistance, lightSource1GetDistance, NULL,
		"group='light source 1' label = 'distance' min=0.3 max=20.0 step=0.1 help = 'Set the distance of the origin for the first light source' ");
	TwAddVarRW(lightsBar, "lightSource1Color", TW_TYPE_COLOR3F, &model.lightSource1.color,
		" group='light source 1' label='color' help='Color of the first light source' ");
	TwAddVarRW(lightsBar, "lightSource1LightPower", TW_TYPE_FLOAT, &model.lightSource1.lightPower,
		"group='light source 1' label='light power' min=0  max=200.0 step=1 help='Adjust the power of the light' ");
	TwAddVarRW(lightsBar, "lightSource1SpecularPower", TW_TYPE_FLOAT, &model.lightSource1.specularPower,
		"group='light source 1' label='specular power' min=0  max=200.0 step=1 help='Adjust the power of the light' ");
	TwAddVarRW(lightsBar, "lightSource1DrawSphere", TW_TYPE_BOOL8, &view.drawSphereForLightSource1,
		"group='light source 1' label='draw sphere' help='Draw sphere to the position of the first light source'");

	TwAddVarCB(lightsBar, "lightSource2Direction", TW_TYPE_DIR3F, lightSource2SetDirection, lightSource2GetDirection, NULL,
		"group='light source 2' label='direction' opened=false help='Change the direction of the second light source' ");
	TwAddVarCB(lightsBar, "lightSource2Distance", TW_TYPE_FLOAT, lightSource2SetDistance, lightSource2GetDistance, NULL,
		"group='light source 2' label = 'distance' min=0.3 max=20.0 step=0.1 help = 'Set the distance of the origin for the second light source' ");
	TwAddVarRW(lightsBar, "lightSource2Color", TW_TYPE_COLOR3F, &model.lightSource2.color,
		" group='light source 2' label='color' help='Color of the first light source' ");
	TwAddVarRW(lightsBar, "lightSource2LightPower", TW_TYPE_FLOAT, &model.lightSource2.lightPower,
		"group='light source 2' label='light power' min=0  max=200.0 step=1 help='Adjust the power of the light' ");
	TwAddVarRW(lightsBar, "lightSource2SpecularPower", TW_TYPE_FLOAT, &model.lightSource2.specularPower,
		"group='light source 2' label='specular power' min=0  max=200.0 step=10 help='Adjust the power of the light' ");
	TwAddVarRW(lightsBar, "lightSource2DrawSphere", TW_TYPE_BOOL8, &view.drawSphereForLightSource2,
		"group='light source 2' label='draw sphere' help='Draw sphere to the position of the second light source'");

	//Callbacks
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetCharModsCallback(window, (GLFWcharmodsfun)TwEventCharModsGLFW3);
	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetScrollCallback(window, (GLFWscrollfun)TwEventScrollGLFW3);
	glfwSetWindowSizeCallback(window, windowResized);
}

int main(int argc, char * argv[]) 
{
	srand(time(NULL));
	initializeGLFW();
	model.initialize();
    ProjLabShaderProgramProvider shaderProvider;
    view.initialize(&model,shaderProvider);
	view.setWindowSize(windowWidth, windowHeight);
	initializeAntTweakBar();

	do {
		animateModel();
		view.render();
		TwDraw();

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	while (glfwWindowShouldClose(window) == 0);

	model.cleanUp();
	view.cleanUp(); 
	TwTerminate();
	glfwTerminate();
	return 1;
}
