#ifndef _cCurious_HG_
#define _cCurious_HG_

//#include "LoadMesh.h"
#include "iEnemy.h"
#include "structs.h"

class cCurious : public iEnemy
{
public:
	cCurious();
	virtual ~cCurious() {}

	virtual void SetPosition(glm::vec3& position);
	virtual void SetMeshEntry(int NumIndices, int BaseIndex, int BaseVertex);
	virtual int GetID();
	virtual void SetID(int ID);
	virtual void Scale(float scale);
	virtual void Update(float dt);
	virtual void GetModelMatrix(glm::mat4& modelMatrix);
	virtual void setMediator(iMediator* mediator);
	virtual void checkState(glm::vec3 targetPosition, glm::vec3 direction);
	virtual void PreRender();
	virtual void GetColor(glm::vec4& color);
	virtual void GetMeshEntry(sMeshEntry& meshEntry);
	virtual void setSpeed(float speed);

	virtual void getPhysicsInfo(sPhysicsInfo& physicsInfo);

	
private:
	float m_eAngularSpeed;
	int m_ID;
	iMediator* m_pMediator;
	eEnemyState m_state;
	sRenderingInfo renderingInfo;
	sPhysicsInfo physicsInfo;
	

	float m_outerRadius;
	float m_innerRadius;
	float m_attentionRadius;
	float m_predictionDistance;
};

#endif