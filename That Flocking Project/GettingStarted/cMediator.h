#include "iMediator.h"
#include <vector>
#include "iEnemy.h"
#include "cPlayer.h"
#include "cEnemyBuilder.h"

class cMediator : public iMediator
{
public:
	cMediator();
	virtual void spawnEnemy(int ID);
	virtual void init(sEnemyDesc* enemyDesc, std::vector<unsigned int>* indices, std::vector<sMeshVertex>* vertices);
	virtual void setPlayer(cPlayer* player);
	virtual void getTargetPosition(glm::vec3& targetPosition, float distance);
	virtual void getPlayerPosition(glm::vec3 &playerPosition);
	virtual void getPlayerForward(glm::vec3& playerForward);
	virtual void createFlockers(std::vector<unsigned int>* indices, std::vector<sMeshVertex>* vertices);

	
private:
	//std::vector<iPlant*> m_vec_pPlants;
	//std::vector<iEnemy*> m_vec_pEnemys;
	cEnemyBuilder* m_pEnemyBuilder;
	sMeshEntry m_curiousMesh;
	sMeshEntry m_angryMesh;
	std::vector<sMeshVertex> m_vertices;
	std::vector<unsigned int> m_indices;
	cPlayer* m_ThePlayer;
	iEnemy* m_pFindEnemyByID(int ID);
	void deleteEnemy(int ID);
};