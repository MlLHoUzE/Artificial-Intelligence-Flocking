#ifndef _cFlockerManager_HG_
#define _cFlockerManager_HG_

#include "cFlocker.h"
#include <vector>


class cFlockerManager
{
public:
	cFlockerManager();

	void addFlocker(iEnemy* flocker);
	void getClosestFlockers(iEnemy* flocker, std::vector<iEnemy*>* flockersOut);
	void Update(float dt);
	void getFlockers(std::vector<iEnemy*>* flockers);

private:
	std::vector<iEnemy*> m_pFlockers;
};

#endif