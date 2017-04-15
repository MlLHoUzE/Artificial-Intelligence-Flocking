#include "global.h"
#include <iostream>

float gAlignWeight = 0.5f;
float gCohesWeight = 0.5f;
float gSeparWeight = 0.5f;

GLFWwindow* gWindow = 0;

cCamera* gCamera = 0;
std::vector<iEnemy*> gEnemies;

cAreaInfo* gAreaInfo = 0;

float secondCounter = 0;

cEnemyFactory* gEnemyFactory = 0;

cFlockerManager* g_pFlockerManager = 0;


GLuint gVertexBufferID = 0;
GLuint gTexCoordBufferID = 0;
GLuint gNormalBufferID = 0;
GLuint gIndexBufferID = 0;

GLuint gProgramID = 0;
GLuint gUniformId_ModelMatrix = 0;
GLuint gUniformId_ViewMatrix = 0;
GLuint gUniformId_PojectionMatrix = 0;
GLuint gUniformId_ModelColor = 0;
GLuint gUniformId_EyePosition = 0;


float getRandFloat(float LO, float HIGH)
{
	float r3 = LO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (HIGH - LO)));

	return r3;
}
