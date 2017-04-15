#include "cMediator.h"
#include "global.h"
#include <iostream>

cMediator::cMediator()
{
	m_pEnemyBuilder->getInstance();
	
}

void cMediator::init(sEnemyDesc* enemyDesc, std::vector<unsigned int>* indices, std::vector<sMeshVertex>* vertices)
{

	LoadMesh("assets/Vader_Tie_Low_N.ply", *indices, *vertices, m_curiousMesh);
	LoadMesh("assets/Y-Wing_Low_N.ply", *indices, *vertices, m_angryMesh);
	iEnemy* curEnemy;
	for (int index = 0; index != enemyDesc->numberOfEnemies/2; index++)
	{
		
		curEnemy = m_pEnemyBuilder->createEnemy("Angry");
		curEnemy->SetPosition(glm::vec3(getRandFloat(gAreaInfo->Min.x, gAreaInfo->Max.x),
					getRandFloat(gAreaInfo->Min.y, gAreaInfo->Max.y),
					getRandFloat(gAreaInfo->Min.z, gAreaInfo->Max.z)));
		curEnemy->Scale(0.2f);
		curEnemy->setSpeed(getRandFloat(enemyDesc->minSpeed, enemyDesc->maxSpeed));
		curEnemy->setMediator(this);
		curEnemy->SetMeshEntry(m_angryMesh.NumIndices, m_angryMesh.BaseIndex, m_angryMesh.BaseVertex);
		//curEnemy->SetColor(glm::vec4(1.0, 1.0, 0.0, 1.0));
			
		gEnemies.push_back(curEnemy);
		
	}
	for (int index = 0; index != enemyDesc->numberOfEnemies/2; index++)
	{

		curEnemy = m_pEnemyBuilder->createEnemy("Curious");
		curEnemy->SetPosition(glm::vec3(getRandFloat(gAreaInfo->Min.x, gAreaInfo->Max.x),
			getRandFloat(gAreaInfo->Min.y, gAreaInfo->Max.y),
			getRandFloat(gAreaInfo->Min.z, gAreaInfo->Max.z)));
		curEnemy->setSpeed(getRandFloat(enemyDesc->minSpeed, enemyDesc->maxSpeed));
		curEnemy->Scale(1.f);
		curEnemy->setMediator(this);
		curEnemy->SetMeshEntry(m_curiousMesh.NumIndices, m_curiousMesh.BaseIndex, m_curiousMesh.BaseVertex);
		//curEnemy->SetColor(glm::vec4(1.0, 1.0, 0.0, 1.0));

		gEnemies.push_back(curEnemy);

	}
}

void cMediator::createFlockers(std::vector<unsigned int>* indices, std::vector<sMeshVertex>* vertices)
{
	LoadMesh("assets/X-Wing_Attack_Low_N.ply", *indices, *vertices, m_angryMesh);
	iEnemy* curEnemy;
	for (int index = 0; index != 200; index++)
	{

		curEnemy = m_pEnemyBuilder->createEnemy("Flocker");
		curEnemy->SetPosition(glm::vec3(getRandFloat(gAreaInfo->Min.x, gAreaInfo->Max.x),
			10.f,//set to 10 so they are all at same height
			getRandFloat(gAreaInfo->Min.z, gAreaInfo->Max.z)));
		curEnemy->Scale(0.2f);
		curEnemy->setSpeed(getRandFloat(5.f, 10.f));
		curEnemy->setMediator(this);
		curEnemy->SetMeshEntry(m_angryMesh.NumIndices, m_angryMesh.BaseIndex, m_angryMesh.BaseVertex);
		//curEnemy->SetColor(glm::vec4(1.0, 1.0, 0.0, 1.0));

		g_pFlockerManager->addFlocker(curEnemy);

	}

}

void cMediator::setPlayer(cPlayer* player)
{
	m_ThePlayer = player;
	return;
}

void cMediator::getTargetPosition(glm::vec3 &targetPosition, float distance)
{
	//glm::vec3 targetPosition;
	m_ThePlayer->GetNextPosition(targetPosition, distance);
	return;
}

void cMediator::getPlayerPosition(glm::vec3 &playerPosition)
{
	playerPosition = m_ThePlayer->physicsInfo.Position;
	return;
}

void cMediator::getPlayerForward(glm::vec3& playerForward)
{
	playerForward = m_ThePlayer->physicsInfo.Orientation * glm::vec3(0.f, 0.f, 1.f);
	return;
}

iEnemy* cMediator::m_pFindEnemyByID(int ID)
{
	for (int index = 0; index != gEnemies.size(); index++)
	{
		if (ID == gEnemies[index]->GetID())
		{
			return gEnemies[index];
		}
	}

	return NULL;
}

void cMediator::deleteEnemy(int ID)
{

	
	std::cout << "Delete Called" << std::endl;
	for ( int index = 0; index != gEnemies.size(); index++)
	{
		if (gEnemies[index]->GetID() == ID)
		{
			gEnemies.erase(gEnemies.begin() + index);
			break;
		}
	}
	gEnemies.shrink_to_fit();
}

void cMediator::spawnEnemy(int ID)
{
	
	
}