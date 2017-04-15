#ifndef _iEnemy_HG_
#define _iEnemy_HG_


#include "LoadMesh.h"
#include "iMediator.h"



class iEnemy
{
public:

	enum eEnemyState
	{
		STATE_FLEE,
		STATE_SEEK,
		STATE_ARRIVE
	};
	//	iShip();
	virtual ~iEnemy() {};

	virtual void setMediator(iMediator* pMediator) = 0;

	virtual void SetPosition(glm::vec3& position) = 0;
	virtual void SetMeshEntry(int NumIndices, int BaseIndex, int BaseVertex) = 0;
	virtual void GetModelMatrix(glm::mat4& modelMatrix) = 0;
	virtual int GetID() = 0;
	virtual void GetColor(glm::vec4& color) = 0;
	virtual void GetMeshEntry(sMeshEntry& meshEntry) = 0;
	virtual void SetID(int ID) = 0;
	virtual void Scale(float scale) = 0;
	virtual void Update(float dt) = 0;
	virtual void checkState(glm::vec3 targetPosition, glm::vec3 direction) = 0;
	virtual void PreRender() = 0;
	virtual void setSpeed(float speed) = 0;

	virtual void getPhysicsInfo(sPhysicsInfo& physicsInfo) = 0;

};



#endif
