#ifndef _cPlayer_HG_
#define _cPlayer_HG_

#include "LoadMesh.h"
#include "structs.h"


class cPlayer
{
public:

	virtual ~cPlayer() {}
	static cPlayer* getInstance(void);
	static void shutDown(void);

	
	virtual void GetNextPosition(glm::vec3& position, float distance);
	virtual void SetMeshEntry(int NumIndices, int BaseIndex, int BaseVertex);

	virtual void Update(float dt);

	virtual void cPlayer::PreRender();

	
	sPhysicsInfo physicsInfo;
	sRenderingInfo renderingInfo;
private:
	cPlayer();
	float m_angularSpeed;
	static cPlayer* m_pTheOneThing;
};

#endif