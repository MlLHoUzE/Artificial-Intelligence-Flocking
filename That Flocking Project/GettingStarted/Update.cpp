#include "Update.h"

void Update()
{
	static double lastTime = glfwGetTime();
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);
	if (deltaTime == 0.f)
	{
		lastTime = currentTime;
		return;
	}
	secondCounter += deltaTime;
	for (int index = 0; index != gEnemies.size(); index++)	//loop through all entities and run their updates
	{
		gEnemies[index]->Update(deltaTime);
	}
	//gPlayer->Update(deltaTime);
	//refocus camera
	glm::mat4 cameraTargetTransform;
	//thePlayer->GetTransform(cameraTargetTransform);
	gCamera->SetTargetTransform(cameraTargetTransform);
	gCamera->Update(deltaTime);
	lastTime = currentTime;	//update for deltaTime
	
	if (secondCounter > 1)
	{
		secondCounter = 0;
	}
	
}