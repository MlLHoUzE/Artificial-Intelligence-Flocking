#pragma once
#include "externals.h"
#include "LoadMesh.h"
struct sPhysicsInfo
{
	sPhysicsInfo() : IsStatic(false) {}

	bool IsStatic;

	glm::vec3 Position;
	//Added
	glm::vec3 Velocity;
	glm::vec3 Acceleration;
	//end
	glm::quat Orientation;
	glm::quat AngularVelocity;

	float speed;
};

struct sRenderingInfo
{
	std::string MeshName;
	sMeshEntry MeshEntry;
	glm::vec4 Color;
	glm::mat4 ModelMatrix;
	float scale;
};

struct sEnemyDesc
{
	int numberOfEnemies;
	float minSpeed;
	float maxSpeed;

};