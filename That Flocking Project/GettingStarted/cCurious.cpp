#include "cCurious.h"
#include "global.h"


cCurious::cCurious()
{
	m_eAngularSpeed = 1.0f;
	m_outerRadius = 12.f;
	m_innerRadius = 4.f;
	m_attentionRadius = 30.f;
	m_predictionDistance = 8.f;
}
//cEnemy::~cEnemy() {}

void cCurious::SetPosition(glm::vec3& position)
{
	physicsInfo.Position = position;
}

void cCurious::SetMeshEntry(int NumIndices, int BaseIndex, int BaseVertex)
{
	renderingInfo.MeshEntry.NumIndices = NumIndices;
	renderingInfo.MeshEntry.BaseIndex = BaseIndex;
	renderingInfo.MeshEntry.BaseVertex = BaseVertex;
}

void cCurious::Scale(float scale)
{
	renderingInfo.scale = scale;
}
int cCurious::GetID()
{
	return m_ID;
}

void cCurious::SetID(int ID)
{
	m_ID = ID;
}

void cCurious::setSpeed(float speed)
{
	physicsInfo.speed = speed;
}

void cCurious::GetModelMatrix(glm::mat4& modelMatrix)
{
	modelMatrix = renderingInfo.ModelMatrix;
}

void cCurious::getPhysicsInfo(sPhysicsInfo& physicsInfoIn)
{
	physicsInfoIn = physicsInfo;
}

void cCurious::Update(float dt)
{
	glm::vec3 playerDirection;
	glm::vec3 targetDirection;

	glm::vec3 targetPosition;
	glm::vec3 playerPosition;
	m_pMediator->getTargetPosition(targetPosition, m_predictionDistance);
	m_pMediator->getPlayerPosition(playerPosition);
	glm::vec3 myPos = physicsInfo.Position;

	glm::vec3 myForward = physicsInfo.Orientation * glm::vec3(0.f, 0.f, 1.f);
	//check distance between player and enemy
	
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
	case STATE_ARRIVE:
		rotationAxis = glm::cross(myForward, playerDirection);
		// because this is all we care about right now
		yAxisValue = rotationAxis.y > 0.f ? 1.f : -1.f;
		//yAxisValue *= -1;
		if (distance > m_outerRadius)
		{
			//move towards player
			physicsInfo.Velocity = myForward * -linearSpeed;
			//m_eTransform = glm::translate(m_eTransform, playerDirection * (linearSpeed * dt));
		}
		else if (distance < m_innerRadius)
		{
			linearSpeed *= 1;
			//back away from player
			physicsInfo.Velocity = myForward * linearSpeed;
			//m_eTransform = glm::translate(m_eTransform, playerDirection * (linearSpeed * dt));
		}
		else
		{
			//maintain
			float distanceIntoZone = distance -  m_innerRadius;	//the amount in the zone area we are
			float zoneSize = m_outerRadius - m_innerRadius;
			float midwayPoint = zoneSize / 2;
			float percentage;
			if(distanceIntoZone > midwayPoint -0.5f && distanceIntoZone < midwayPoint + 0.5f)
			{
				linearSpeed = 0.f;
				
				physicsInfo.Velocity = glm::vec3(0.f, 0.f, 0.f);// myForward * linearSpeed;
			}
			else if (distanceIntoZone < midwayPoint)
			{
				percentage = distanceIntoZone / zoneSize;// midwayPoint;
				percentage += 1;
				linearSpeed /= percentage;
				physicsInfo.Velocity = myForward * linearSpeed;
			}
			else if (distanceIntoZone > midwayPoint)
			{
				distanceIntoZone -= midwayPoint;
				percentage = distanceIntoZone / midwayPoint;
				linearSpeed *= -percentage;
				physicsInfo.Velocity = myForward * linearSpeed;
			}
			
			//m_eTransform = glm::translate(m_eTransform, playerDirection * (linearSpeed * dt));
			
			//set velocity based on how far into the "Zone" you are
		}
		break;
	case STATE_FLEE:
		//if (glm::length(myPos - targetPosition) < m_predictionDistance)
		//{
		//	//between player and target, run from player
		//	linearSpeed *= 1.0f;
		//	rotationAxis = glm::cross(-myForward, playerDirection);
		//	yAxisValue = rotationAxis.y > 0.f ? 1.f : -1.f;
		//	physicsInfo.Velocity = myForward * -linearSpeed;
		//}
		//else
		//{
			linearSpeed *= 1.0f;
			rotationAxis = glm::cross(-myForward, targetDirection);
			yAxisValue = rotationAxis.y > 0.f ? 1.f : -1.f;
			physicsInfo.Velocity = myForward * -linearSpeed;
		//}
		yAxisValue *= -1;
		break;
	}
	physicsInfo.Position = physicsInfo.Position + (physicsInfo.Velocity * dt);
	
	float rotationVelocity = 5.f * glm::abs(rotationAxis.y);
	float rotation = rotationVelocity * dt;

	physicsInfo.Orientation = glm::rotate(physicsInfo.Orientation, rotation, rotationAxis);

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
void cCurious::checkState(glm::vec3 targetPosition, glm::vec3 direction)
{
	glm::vec3 targetForward;
	m_pMediator->getPlayerForward(targetForward);
	//targetForward = glm::vec3(targetTransform[2].x, targetTransform[2].y, targetTransform[2].z);
	float dotResult = glm::dot(targetForward, direction);
	if (dotResult <= 0.866)	//player is looking away from me
	{
		//seek
		m_state = STATE_ARRIVE;

		//change colour
		renderingInfo.Color = (glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
	}
	else if (dotResult > 0.866)//player is looking at me
	{
		//flee
		m_state = STATE_FLEE;
		//change colour

		renderingInfo.Color = (glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
	}
}


void cCurious::setMediator(iMediator* mediator)
{
	m_pMediator = mediator;
}

void cCurious::PreRender()
{
	if (physicsInfo.IsStatic) return;
	renderingInfo.ModelMatrix = glm::mat4_cast(physicsInfo.Orientation);
	renderingInfo.ModelMatrix[3] = glm::vec4(physicsInfo.Position, 1.f);
}

void cCurious::GetColor(glm::vec4& color)
{
	color = renderingInfo.Color;
}

void cCurious::GetMeshEntry(sMeshEntry& meshEntry)
{
	meshEntry = renderingInfo.MeshEntry;
}