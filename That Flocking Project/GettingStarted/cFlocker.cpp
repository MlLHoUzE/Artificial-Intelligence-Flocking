#include "cFlocker.h"
#include "global.h"


cFlocker::cFlocker()
{
	m_eAngularSpeed = 1.0f;
	m_attentionRadius = 30.f;
	m_predictionDistance = 8.f;
	m_AllignWeight = 0.5f;
	m_CohessiveWeight = 0.5f;
	m_SeparationWeight = 0.5f;
	m_timer = 6.f;
	m_timerWait = getRandFloat(2.f, 5.f);
	renderingInfo.Color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

	//SetPosition(glm::vec3(getRandFloat(0.f, 10.f), getRandFloat(0.f, 10.f), getRandFloat(0.f, 10.f)));
}
//cEnemy::~cEnemy() {}

void cFlocker::SetPosition(glm::vec3& position)
{
	physicsInfo.Position = position;
	renderingInfo.ModelMatrix[3][0] = position.x;
	renderingInfo.ModelMatrix[3][1] = position.y;
	renderingInfo.ModelMatrix[3][2] = position.z;
}

void cFlocker::GetMeshEntry(sMeshEntry& meshEntry)
{
	meshEntry = renderingInfo.MeshEntry;
}

void cFlocker::SetMeshEntry(int NumIndices, int BaseIndex, int BaseVertex)
{
	renderingInfo.MeshEntry.NumIndices = NumIndices;
	renderingInfo.MeshEntry.BaseIndex = BaseIndex;
	renderingInfo.MeshEntry.BaseVertex = BaseVertex;
}
void cFlocker::GetColor(glm::vec4& color)
{
	color = renderingInfo.Color;
}

int cFlocker::GetID()
{
	return m_ID;
}

void cFlocker::SetID(int ID)
{
	m_ID = ID;
}

void cFlocker::Scale(float scale)
{
	renderingInfo.scale = scale;
}

void cFlocker::setSpeed(float speed)
{
	physicsInfo.speed = speed;
	physicsInfo.Velocity = (physicsInfo.Orientation * glm::vec3(0.f, 0.f, 1.f)) * speed;
}

void cFlocker::getPhysicsInfo(sPhysicsInfo& physicsInfoIn)
{
	physicsInfoIn = physicsInfo;
}

void cFlocker::Update(float dt)
{
	m_timer += dt;
	if (m_timer > m_timerWait)
	{
		m_pCloseFlockers.clear();
		g_pFlockerManager->getClosestFlockers(this, &m_pCloseFlockers);
		m_timer = 0.f;
	}
	glm::vec3 finalVec;
	glm::vec3 allignmentVec;
	glm::vec3 cohessionVec;
	glm::vec3 separationVec;
	float numNeighbours = m_pCloseFlockers.size();
	for (int i = 0; i != numNeighbours; i++)
	{
		//get alignment forward
		glm::mat4 trans;
		m_pCloseFlockers[i]->GetModelMatrix(trans);
		/*allignmentVec.x += trans[2][0];
		allignmentVec.y += trans[2][1];
		allignmentVec.z += trans[2][2];*/

		//get allignment velocity
		sPhysicsInfo physicsInfo;
		m_pCloseFlockers[i]->getPhysicsInfo(physicsInfo);
		allignmentVec += physicsInfo.Velocity;

		//get cohession
		glm::vec3 otherPos = trans[3];
		cohessionVec += otherPos;

		//get separation
		separationVec += this->physicsInfo.Position - otherPos;
	}
	allignmentVec = allignmentVec / numNeighbours;
	allignmentVec = glm::normalize(allignmentVec);

	cohessionVec = cohessionVec/ numNeighbours;
	cohessionVec.y = 0.0f;
	cohessionVec = glm::normalize(cohessionVec - this->physicsInfo.Position);
	cohessionVec.y = 0.f;
	
	separationVec.y = 0.f;
	separationVec = separationVec / numNeighbours;
	separationVec = glm::normalize(separationVec);
	

	finalVec = (gAlignWeight * allignmentVec) + (gCohesWeight * cohessionVec) + (gSeparWeight * separationVec);

	this->physicsInfo.Velocity = finalVec * physicsInfo.speed;

	this->physicsInfo.Position = physicsInfo.Position + physicsInfo.Velocity * dt;
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

void cFlocker::checkState(glm::vec3 targetPosition, glm::vec3 direction)
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

void cFlocker::setMediator(iMediator* mediator)
{
	m_pMediator = mediator;
}

void cFlocker::PreRender()
{
	if (physicsInfo.IsStatic) return;
	renderingInfo.ModelMatrix = glm::mat4_cast(physicsInfo.Orientation);
	renderingInfo.ModelMatrix[3] = glm::vec4(physicsInfo.Position, 1.f);
	renderingInfo.ModelMatrix = glm::scale(renderingInfo.ModelMatrix, glm::vec3(renderingInfo.scale, renderingInfo.scale, renderingInfo.scale));
}

void cFlocker::GetModelMatrix(glm::mat4& modelMatrix)
{
	modelMatrix = renderingInfo.ModelMatrix;
}