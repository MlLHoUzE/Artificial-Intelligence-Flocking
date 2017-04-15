#include "cPlayer.h"
#include "Input.h"

cPlayer::cPlayer()
{
	m_angularSpeed = 1.0f;
}

cPlayer* cPlayer::m_pTheOneThing = 0;

cPlayer* cPlayer::getInstance(void)	//Singleton step 2
{
	// Is the pointer still 0, NULL
	if (cPlayer::m_pTheOneThing == 0)
	{	// Yup. 
		cPlayer::m_pTheOneThing = new cPlayer();
	}
	return cPlayer::m_pTheOneThing;
}

void cPlayer::shutDown(void)
{
	// Do we have an instance?
	if (cPlayer::m_pTheOneThing != 0)
	{	// Yes, so get rid of it
		delete cPlayer::m_pTheOneThing;
	}
	return;
}

void cPlayer::GetNextPosition(glm::vec3& position, float distance)
{
	position = physicsInfo.Position + physicsInfo.Velocity * distance;
}

void cPlayer::SetMeshEntry(int NumIndices, int BaseIndex, int BaseVertex)
{
	renderingInfo.MeshEntry.NumIndices = NumIndices;
	renderingInfo.MeshEntry.BaseIndex = BaseIndex;
	renderingInfo.MeshEntry.BaseVertex = BaseVertex;
}

void cPlayer::Update(float dt)
{
	bool pressW = nInput::IsKeyPressed::W();
	bool pressS = nInput::IsKeyPressed::S();
	bool pressA = nInput::IsKeyPressed::A();
	bool pressD = nInput::IsKeyPressed::D();
	bool pressI = nInput::IsKeyPressed::I();
	bool pressO = nInput::IsKeyPressed::O();
	bool pressP = nInput::IsKeyPressed::P();
	bool pressShift = nInput::IsKeyPressed::LEFTSHIFT();

	if (pressI)
	{
		if(!pressShift)	gAlignWeight += 0.05f;
		else gAlignWeight -= 0.05f;

		if (gAlignWeight > 1.f) gAlignWeight = 1.f;
		if (gAlignWeight < 0.f) gAlignWeight = 0.f;
	}
	if (pressO)
	{
		if (!pressShift)	gCohesWeight += 0.05f;
		else gCohesWeight -= 0.05f;

		if (gCohesWeight > 1.f) gCohesWeight = 1.f;
		if (gCohesWeight < 0.f) gCohesWeight = 0.f;
	}
	if (pressP)
	{
		if (!pressShift)	gSeparWeight += 0.05f;
		else gSeparWeight -= 0.05f;

		if (gSeparWeight > 1.f) gSeparWeight = 1.f;
		if (gSeparWeight < 0.f) gSeparWeight = 0.f;
	}
	//camera controls:
	//up/down = zoom in/zoom out
	//left/right = pan left/pan right
	glm::vec3 up(0.f, 1.f, 0.f);
	glm::vec3 forward(0.f, 0.f, 1.f);
	forward = physicsInfo.Orientation * forward;
	float linearSpeed = 0;
	float angularSpeed = m_angularSpeed;
	//movement controls:
	// w/s = forward / backward
	if (pressW != pressS)
	{
		linearSpeed = pressW ? physicsInfo.speed : -physicsInfo.speed;
		
		//m_transform = glm::translate(m_transform, glm::vec3(0.0f, 0.0f, linearSpeed * dt));
		//SetPosition(glm::vec3(linearSpeed, 0.f, 0.f));
	}
	physicsInfo.Velocity = forward * linearSpeed;
	if (physicsInfo.Position.x < gAreaInfo->Min.x)
	{
		physicsInfo.Position.x = gAreaInfo->Max.x;
	}
	else if (physicsInfo.Position.x > gAreaInfo->Max.x)
	{
		physicsInfo.Position.x = gAreaInfo->Min.x;
	}

	if (physicsInfo.Position.z < gAreaInfo->Min.z)
	{
		physicsInfo.Position.z = gAreaInfo->Max.z;
	}
	else if (physicsInfo.Position.z > gAreaInfo->Max.z)
	{
		physicsInfo.Position.z = gAreaInfo->Min.z;
	}

	if (pressA != pressD)
	{
		if (pressD) angularSpeed *= -1.f;
		physicsInfo.Orientation = glm::rotate(physicsInfo.Orientation, angularSpeed * dt, up);
		//m_transform = glm::rotate(m_transform, angularSpeed*dt, glm::vec3(0.f, 1.f, 0.f));
	}

	physicsInfo.Position = physicsInfo.Position + (physicsInfo.Velocity * dt);


}

void cPlayer::PreRender()
{
	if (physicsInfo.IsStatic) return;
	renderingInfo.ModelMatrix = glm::mat4_cast(physicsInfo.Orientation);
	renderingInfo.ModelMatrix[3] = glm::vec4(physicsInfo.Position, 1.f);
}