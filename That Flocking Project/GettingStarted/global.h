#pragma once
#include "externals.h"
#include "cCamera.h"
#include "cPlayer.h"
#include "cAreaInfo.h"
#include "cEnemyFactory.h"
#include "cFlockerManager.h"

extern float gAlignWeight;
extern float gCohesWeight;
extern float gSeparWeight;

extern GLFWwindow* gWindow;

extern float secondCounter;

extern cCamera* gCamera;

extern cAreaInfo* gAreaInfo;


extern cEnemyFactory* gEnemyFactory;


extern std::vector<iEnemy*> gEnemies;

extern cFlockerManager* g_pFlockerManager;


extern GLuint gVertexBufferID;
extern GLuint gTexCoordBufferID;
extern GLuint gNormalBufferID;
extern GLuint gIndexBufferID;

extern GLuint gProgramID;
extern GLuint gUniformId_ModelMatrix;
extern GLuint gUniformId_ViewMatrix;
extern GLuint gUniformId_PojectionMatrix;
extern GLuint gUniformId_ModelColor;
extern GLuint gUniformId_EyePosition;


float getRandFloat(float LO, float HIGH);

