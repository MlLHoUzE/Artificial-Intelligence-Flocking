#ifndef _cAngry_HG_
#define _cAngry_HG_

//#include "LoadMesh.h"
#include "iEnemy.h"
#include "structs.h"


class cAngry : public iEnemy
{
public:
	
	cAngry();
	virtual ~cAngry() {}

	
	virtual void SetPosition(glm::vec3& position);
	virtual void GetMeshEntry(sMeshEntry& meshEntry);
	virtual void SetMeshEntry(int NumIndices, int BaseIndex, int BaseVertex);
	virtual void GetColor(glm::vec4& color);
	virtual int GetID();
	virtual void SetID(int ID);
	virtual void Scale(float scale);
	virtual void Update(float dt);
	virtual void GetModelMatrix(glm::mat4& modelMatrix);
	virtual void PreRender();
	virtual void setSpeed(float speed);

	virtual void setMediator(iMediator* mediator);
	virtual void checkState(glm::vec3 targetPosition, glm::vec3 direction);

	virtual void getPhysicsInfo(sPhysicsInfo& physicsInfo);

	
private:
	float m_eAngularSpeed;
	eEnemyState m_state;
	int m_ID;
	iMediator* m_pMediator;
	float m_attentionRadius;
	sRenderingInfo renderingInfo;
	sPhysicsInfo physicsInfo;
	float m_predictionDistance;
};

#endif