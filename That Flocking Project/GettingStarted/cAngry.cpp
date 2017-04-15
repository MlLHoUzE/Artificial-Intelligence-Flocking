#include "cAngry.h"
#include "global.h"


cAngry::cAngry()
{
	m_eAngularSpeed = 1.0f;
	m_attentionRadius = 30.f;
	m_predictionDistance = 8.f;
}
//cEnemy::~cEnemy() {}

void cAngry::SetPosition(glm::vec3& position)
{
	physicsInfo.Position = position;
}

void cAngry::GetMeshEntry(sMeshEntry& meshEntry)
{
	meshEntry = renderingInfo.MeshEntry;
}

void cAngry::SetMeshEntry(int NumIndices, int BaseIndex, int BaseVertex)
{
	renderingInfo.MeshEntry.NumIndices = NumIndices;
	renderingInfo.MeshEntry.BaseIndex = BaseIndex;
	renderingInfo.MeshEntry.BaseVertex = BaseVertex;
}
void cAngry::GetColor(glm::vec4& color)
{
	color = renderingInfo.Color;
}

int cAngry::GetID()
{
	return m_ID;
}

void cAngry::SetID(int ID)
{
	m_ID = ID;
}

void cAngry::Scale(float scale)
{
	renderingInfo.scale = scale;
}

void cAngry::setSpeed(float speed)
{
	physicsInfo.speed = speed;
}

void cAngry::getPhysicsInfo(sPhysicsInfo& physicsInfoIn)
{
	physicsInfoIn = physicsInfo;
}

void cAngry::Update(float dt)
{
	glm::vec3 playerDirection;
	glm::vec3 targetDirection;

	glm::vec3 targetPosition;
	glm::vec3 playerPosition;
	m_pMediator->getTargetPosition(targetPosition, m_predictionDistance);
	m_pMediator->getPlayerPosition(playerPosition);
	glm::vec3 myPos = physicsInfo.Position;

	glm::vec3 myForward = physicsInfo.Orientation * glm::vec3(0.f, 0.f, 1.f);
	

	playerDirection = glm::normalize(myPos - playerPosition);
	targetDirection = glm::normalize(myPos - targetPosition);


	checkState(playerPosition, playerDirection);

	glm::vec3 rotationAxis;
	float yAxisValue;

	float linearSpeed = physicsInfo.speed;
	float distance = glm::distance(playerPosition, myPos);
	if (distance > m_attentionRadius)
	{
		yAxisValue = 0;
		renderingInfo.Color = glm::vec4(1.0, 1.0, 1.0, 1.0);
		return;
	}
	switch (m_state)
	{
	case STATE_SEEK:
		rotationAxis = glm::cross(myForward, targetDirection);
		yAxisValue = rotationAxis.y > 0.f ? 1.f : -1.f;
		yAxisValue *= -1;
		physicsInfo.Velocity = myForward * linearSpeed;
		break;
	case STATE_FLEE:
		if (glm::length(myPos - targetPosition) < m_predictionDistance)
		{
			//between player and target, run from player
			linearSpeed *= 1.0f;
			rotationAxis = glm::cross(myForward, playerDirection);
			yAxisValue = rotationAxis.y > 0.f ? 1.f : -1.f;
			physicsInfo.Velocity = myForward * linearSpeed;
		}
		else
		{
			linearSpeed *= 1.0f;
			rotationAxis = glm::cross(myForward, targetDirection);
			yAxisValue = rotationAxis.y > 0.f ? 1.f : -1.f;
			physicsInfo.Velocity = myForward * linearSpeed;
		}
		
		break;
	}

	physicsInfo.Position = physicsInfo.Position + physicsInfo.Velocity * dt;

	rotationAxis = glm::vec3(0.f, yAxisValue, 0.f);

	float rotationVelocity = 5.f * glm::abs(rotationAxis.y);
	float rotation = rotationVelocity * dt;

	physicsInfo.Orientation = glm::rotate(physicsInfo.Orientation, rotation, rotationAxis);
	//m_eTransform = glm::translate(m_eTransform, targetDirection * (linearSpeed * dt));
	

	//check soft bounds
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

}

void cAngry::checkState(glm::vec3 targetPosition, glm::vec3 direction)
{
	glm::vec3 targetForward;
	m_pMediator->getPlayerForward(targetForward);
	//targetForward = glm::vec3(targetTransform[2].x, targetTransform[2].y, targetTransform[2].z);
	float dotResult = glm::dot(direction, targetForward);
	if (dotResult <= 0.866)	//player is looking away from me
	{
		//seek
		m_state = STATE_SEEK;

		//change colour
		renderingInfo.Color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	}
	else if (dotResult > 0.866)//player is looking at me
	{
		//flee
		m_state = STATE_FLEE;
		//change colour

		renderingInfo.Color = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	}
}

void cAngry::setMediator(iMediator* mediator)
{
	m_pMediator = mediator;
}

void cAngry::PreRender()
{
	if (physicsInfo.IsStatic) return;
	renderingInfo.ModelMatrix = glm::mat4_cast(physicsInfo.Orientation);
	renderingInfo.ModelMatrix[3] = glm::vec4(physicsInfo.Position, 1.f);
	renderingInfo.ModelMatrix = glm::scale(renderingInfo.ModelMatrix, glm::vec3(renderingInfo.scale, renderingInfo.scale, renderingInfo.scale));
}

void cAngry::GetModelMatrix(glm::mat4& modelMatrix)
{
	modelMatrix = renderingInfo.ModelMatrix;
}