// glfw needs these guys :(
#include <stdio.h>
#include <stdlib.h>

#include <iostream>

#include "externals.h"
#include "global.h"
#include "LoadShaders.h"
#include "LoadMesh.h"
#include "LoadTexture.h"
//#include "Update.h"
#include "iMediator.h"
#include "cMediator.h"
#include "JsonHelp.h"
#include "cFlocker.h"
#include "cFlockerManager.h"
#include <sstream>

sMeshEntry monkeyMesh;
sMeshEntry boxMesh;
cPlayer* thePlayer;

void Callback_WindowResize(GLFWwindow* window, int width, int height);
void CreateTheBuffers(std::vector<sMeshVertex>& vertices, std::vector<unsigned int>& indices);
void BindTheBuffers();
void DrawBufferEntry(sMeshEntry& entry);
bool SetUpTheShader();
void Update();
void DrawScene();
float getRandFloat(float LO, float HIGH);


int main()
{
	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return -1;
	}

	
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	std::string fileName = "scene.json";
	Json::Value json;
	std::string errs;

	

	if (!JsonHelp::Load(fileName, json, errs))
	{
		std::cout << "something went wrong" << std::endl;
		std::cout << errs << std::endl;
	}
	gCamera = new cCamera();

	if (!JsonHelp::Set(json["Camera"], *gCamera))
	{
		std::cout << "Couldn't load the Camera" << std::endl;
	}

	//create the Player
	thePlayer = cPlayer::getInstance();
	if (!JsonHelp::Set(json["Player"], *thePlayer))
	{
		std::cout << "couldn't load the Player" << std::endl;
	}

	gAreaInfo = new cAreaInfo();
	if (!JsonHelp::Set(json["Area"], *gAreaInfo))
	{
		std::cout << "Couldn't load the Area" << std::endl;
	}
	// Open a window and create its OpenGL context
	gWindow = glfwCreateWindow(1024, 768, "Artificial Intelligence Proj 1: By Ryan Hammond", NULL, NULL);
	if (gWindow == NULL)
	{
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible.\n");
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(gWindow);
	
	// Initialize GLEW
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}


	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(gWindow, GLFW_STICKY_KEYS, GL_TRUE);
	glfwPollEvents();

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);

	// Load the shader
	if (!SetUpTheShader())
	{
		std::cout << "Oh no! The shaders didn't load!!" << std::endl;
		system("pause");
		return -1;
	}
	// load the mesh buffers
	std::vector<sMeshVertex> vertices;
	std::vector<unsigned int> indices;
	bool success = false;
	success = LoadMesh("assets/suzanne.obj", indices, vertices, monkeyMesh);
	{	//create player
		thePlayer->SetMeshEntry(monkeyMesh.NumIndices, monkeyMesh.BaseIndex, monkeyMesh.BaseVertex);
		thePlayer->renderingInfo.Color = glm::vec4(1.f, 0.f, 1.f, 1.f);
	}

	
	
	iMediator* pMediator = new cMediator();
	sEnemyDesc* enemyDesc = new sEnemyDesc();
	if (!JsonHelp::Set(json["Enemies"], *enemyDesc))
	{
		std::cout << "Couldn't load the enemy Info" << std::endl;
	}

	pMediator->init(enemyDesc, &indices, &vertices);
	pMediator->setPlayer(thePlayer);

	success = LoadMesh("assets/box.obj", indices, vertices, boxMesh);
	if (!success)
	{
		std::cout << "we couldn't load the model!" << std::endl;
		system("pause");
		return -1;
	}

	g_pFlockerManager = new cFlockerManager();

	pMediator->createFlockers(&indices, &vertices);
	
	CreateTheBuffers(vertices, indices);

	

	/*for (int i = 0; i != 20; i++)
	{
		cFlocker* newFlocker = new cFlocker();
		g_pFlockerManager->addFlocker(newFlocker);
	}
	
	std::vector<iEnemy*> test;
	iEnemy* testFlocker = new cFlocker();

	g_pFlockerManager->getClosestFlockers(testFlocker, &test);*/
	
	do {
		// Clear the screen. It's not mentioned before Tutorial 02, but it can cause flickering, so it's there nonetheless.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Update();
		DrawScene();

		// Swap buffers
		glfwSwapBuffers(gWindow);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(gWindow, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(gWindow) == 0);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();
	delete gCamera;
	gCamera = 0;
	return 0;
}

// this is just terrible
void DrawTheFloor()
{
	glm::mat4 boxMat;
	boxMat = glm::scale(boxMat, glm::vec3(0.5f, 0.25f, 0.5f));
	glm::vec4 boxColorLight(0.9f, 0.9f, 0.9f, 1.0f);
	glm::vec4 boxColorDark(0.2f, 0.2f, 0.2f, 1.f);
	glm::vec4 red(1.f, 0.f, 0.f, 1.f);
	glm::vec4 blue(0.f, 0.f, 1.f, 1.f);

	// per item uniforms
	//glUniformMatrix4fv(gUniformId_ModelMatrix, 1, GL_FALSE, glm::value_ptr(boxMat));
	//glUniform4fv(gUniformId_ModelColor, 1, glm::value_ptr(boxColorLight));

	glm::vec3 center(-0.f, -1.4f, -0.f);
	glm::vec3 xStep(1.f, 0.f, 0.f);
	glm::vec3 zStep(0.f, 0.f, 1.f);
	/*glm::vec4 color = boxColorLight;
	glUniformMatrix4fv(gUniformId_ModelMatrix, 1, GL_FALSE, glm::value_ptr(boxMat));
	glUniform4fv(gUniformId_ModelColor, 1, glm::value_ptr(color));*/

	//DrawBufferEntry(boxMesh);
	for (int i = gAreaInfo->Min.x; i <= gAreaInfo->Max.x; i++)
	{
		for (int k = gAreaInfo->Min.z; k <= gAreaInfo->Max.z; k++)
		{
			glm::vec3 pos = center + xStep * (float)i + zStep * (float)k;
			glm::vec4 color = (i * k) % 2 == 0 ? boxColorDark : boxColorLight;
			if (i * k == 0 && i + k != 0)
			{
				if (i == 0) color = blue;
				else color = red;
			}
			boxMat[3].x = pos.x;  boxMat[3].y = pos.y; boxMat[3].z = pos.z;

			glUniformMatrix4fv(gUniformId_ModelMatrix, 1, GL_FALSE, glm::value_ptr(boxMat));
			glUniform4fv(gUniformId_ModelColor, 1, glm::value_ptr(color));

			DrawBufferEntry(boxMesh);
		}
	}
	
}

void DrawScene()
{
	glm::mat4 projectionMatrix;
	glm::mat4 viewMatrix;
	glm::vec4 eye;
	gCamera->GetProjectionMatrix(projectionMatrix);
	gCamera->GetViewMatrix(viewMatrix);
	gCamera->GetEyePosition(eye);

	BindTheBuffers();
	glUseProgram(gProgramID);
	
	// per frame uniforms
	glUniformMatrix4fv(gUniformId_PojectionMatrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(gUniformId_ViewMatrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glm::vec4 eye4(eye.x, eye.y, eye.z, eye.w);
	glUniform4fv(gUniformId_EyePosition, 1, glm::value_ptr(eye4));
	
	DrawTheFloor();


	for (int index = 0; index != gEnemies.size(); index++)
	{

		iEnemy* curEnemy = gEnemies[index];
		curEnemy->PreRender();
		glm::mat4 curTransform;
		curEnemy->GetModelMatrix(curTransform);
		glm::vec4 curColor;
		curEnemy->GetColor(curColor);
		sMeshEntry curMeshEntry;
		curEnemy->GetMeshEntry(curMeshEntry);
		glUniformMatrix4fv(gUniformId_ModelMatrix, 1, GL_FALSE, glm::value_ptr(curTransform));
		glUniform4fv(gUniformId_ModelColor, 1, glm::value_ptr(curColor));
		DrawBufferEntry(curMeshEntry);
	}

	std::vector<iEnemy*> flockers;
	g_pFlockerManager->getFlockers(&flockers);

	for (int i = 0; i != flockers.size(); i++)
	{
		iEnemy* curEnemy = flockers[i];
		curEnemy->PreRender();
		glm::mat4 curTransform;
		curEnemy->GetModelMatrix(curTransform);
		glm::vec4 curColor;
		curEnemy->GetColor(curColor);
		sMeshEntry curMeshEntry;
		curEnemy->GetMeshEntry(curMeshEntry);
		glUniformMatrix4fv(gUniformId_ModelMatrix, 1, GL_FALSE, glm::value_ptr(curTransform));
		glUniform4fv(gUniformId_ModelColor, 1, glm::value_ptr(curColor));
		DrawBufferEntry(curMeshEntry);
	}

	//draw the player
	thePlayer->PreRender();

	glm::mat4 curTransform;
	curTransform = thePlayer->renderingInfo.ModelMatrix;
	glm::vec4 curColor;
	curColor = thePlayer->renderingInfo.Color;
	sMeshEntry curMeshEntry;
	curMeshEntry = thePlayer->renderingInfo.MeshEntry;
	glUniformMatrix4fv(gUniformId_ModelMatrix, 1, GL_FALSE, glm::value_ptr(curTransform));
	glUniform4fv(gUniformId_ModelColor, 1, glm::value_ptr(curColor));
	DrawBufferEntry(curMeshEntry);
	//for each entity
	//grab teh entitty
	//get physics stuff, pass the transform as model matrix
	//get the render stuff, pass the color, and render the mesh

	// per item uniforms
	//glUniformMatrix4fv(gUniformId_ModelMatrix, 1, GL_FALSE, glm::value_ptr(gMonkeyMatrix));
	//glUniform4fv(gUniformId_ModelColor, 1, glm::value_ptr(gMonkeyColor));

	//DrawBufferEntry(monkeyMesh);
}

bool SetUpTheShader()
{
	// Create and compile our GLSL program from the shaders
	gProgramID = LoadShaders("SimpleShader.vert", "SimpleShader.frag");
	if (!gProgramID)
	{
		std::cout << "Oh no! The shaders didn't load!!" << std::endl;
		return false;
	}
	
	// per item
	gUniformId_ModelMatrix = glGetUniformLocation(gProgramID, "ModelMatrix");
	gUniformId_ModelColor = glGetUniformLocation(gProgramID, "ModelColor");
	// per frame
	gUniformId_ViewMatrix = glGetUniformLocation(gProgramID, "ViewMatrix");
	gUniformId_PojectionMatrix = glGetUniformLocation(gProgramID, "ProjectionMatrix");
	gUniformId_EyePosition = glGetUniformLocation(gProgramID, "EyePosition");


	return true;
}

void Callback_WindowResize(GLFWwindow* window, int width, int height)
{
	gCamera->WindowResize(width, height);
}

void CreateTheBuffers(std::vector<sMeshVertex>& vertices, std::vector<unsigned int>& indices)
{
	glGenVertexArrays(1, &gVertexBufferID);
	glBindVertexArray(gVertexBufferID);

	glGenBuffers(1, &gTexCoordBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, gTexCoordBufferID);

	glGenBuffers(1, &gNormalBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, gNormalBufferID);

	int bytesInVertexArray = vertices.size() * sizeof(sMeshVertex);
	glBufferData(GL_ARRAY_BUFFER, bytesInVertexArray, &vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &gIndexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIndexBufferID);

	int bytesInIndexArray = indices.size() * sizeof(GLuint);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, bytesInIndexArray, &indices[0], GL_STATIC_DRAW);

	BindTheBuffers();
}

void BindTheBuffers()
{
	glBindVertexArray(gVertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, gTexCoordBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, gNormalBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIndexBufferID);

	glEnableVertexAttribArray(0); // Position
	glEnableVertexAttribArray(1); // TexCoord
	glEnableVertexAttribArray(2); // Normal

	glVertexAttribPointer(0,		// index or "slot" in the shader
		4,							// Number of variables: vec4 would be 4 32-bit variables
		GL_FLOAT,					// Type: vec4 is float
		GL_FALSE,					// "normalize" the values (or not)
		sizeof(sMeshVertex),			// Number of bytes per vertex (the "stride")
		(GLvoid*)0);				// Offset from vertex (position is first, so offset = 0

	int offsetToTexCoordsInBytes = sizeof(((sMeshVertex*)0)->Position);
	glVertexAttribPointer(1,
		4,
		GL_FLOAT,
		GL_FALSE,
		sizeof(sMeshVertex),
		(GLvoid*)offsetToTexCoordsInBytes);		// Offset in bytes to TexCoords

	int offsetToNormalInBytes = offsetToTexCoordsInBytes + sizeof(((sMeshVertex*)0)->TexCoord);
	glVertexAttribPointer(2,
		4,
		GL_FLOAT,
		GL_FALSE,
		sizeof(sMeshVertex),
		(GLvoid*)offsetToNormalInBytes);		// Offset in bytes to Normal
}

void DrawBufferEntry(sMeshEntry& entry)
{
	glDrawElementsBaseVertex(GL_TRIANGLES,
		entry.NumIndices,
		GL_UNSIGNED_INT,
		(void*)(sizeof(unsigned int) * entry.BaseIndex),
		entry.BaseVertex);
}

void Update()
{
	//std::stringstream title;
	//title.str("Alignment Weight: " + std::to_string(gAlignWeight) + " Cohession Weight: " + std::to_string(gCohesWeight) + " Separation Weight: " + std::to_string(gSeparWeight));
	std::string title = "Alignment Weight: " + std::to_string(gAlignWeight) + " Cohession Weight: " + std::to_string(gCohesWeight) + " Separation Weight: " + std::to_string(gSeparWeight);
	glfwSetWindowTitle(gWindow, title.c_str());
	static double lastTime = glfwGetTime();
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);
	if (deltaTime > 0.015)
		deltaTime = 0.015;
	if (deltaTime == 0.f)
	{
		lastTime = currentTime;
		return;
	}
	for (int index = 0; index != gEnemies.size(); index++)	//loop through all entities and run their updates
	{
		gEnemies[index]->Update(deltaTime);
	}
	g_pFlockerManager->Update(deltaTime);
	thePlayer->Update(deltaTime);
	//refocus camera
	glm::mat4 cameraTargetTransform = thePlayer->renderingInfo.ModelMatrix;// (cameraTargetTransform);
	gCamera->SetTargetTransform(cameraTargetTransform);
	gCamera->Update(deltaTime);
	lastTime = currentTime;	//update for deltaTime

}

//float getRandFloat(float LO, float HIGH)
//{
//	float r3 = LO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (HIGH - LO)));
//
//	return r3;
//}