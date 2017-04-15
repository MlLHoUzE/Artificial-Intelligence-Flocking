#ifndef _iMediator_HG_
#define _iMediator_HG_

#include "externals.h"
#include <string>
#include <vector>
#include "LoadMesh.h"
#include "cPlayer.h"

class iMediator
{
public:
	virtual ~iMediator() {};
	virtual void init(sEnemyDesc* enemyDesc, std::vector<unsigned int>* indices, std::vector<sMeshVertex>* vertices) = 0;

	virtual void spawnEnemy(int ID) = 0;

	virtual void setPlayer(cPlayer* player) = 0;
	virtual void getTargetPosition(glm::vec3& targetPosition, float distance) = 0;
	virtual void getPlayerPosition(glm::vec3 &playerPosition) = 0;
	virtual void getPlayerForward(glm::vec3& playerForward) = 0;
	virtual void createFlockers(std::vector<unsigned int>* indices, std::vector<sMeshVertex>* vertices) = 0;
	//virtual bool EatFood() = 0;
};

#endif