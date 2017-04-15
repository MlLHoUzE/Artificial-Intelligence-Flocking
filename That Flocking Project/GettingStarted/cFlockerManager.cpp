#include "cFlockerManager.h"
#include <map>

cFlockerManager::cFlockerManager()
{

}

void cFlockerManager::addFlocker(iEnemy* flocker)
{
	m_pFlockers.push_back(flocker);
}
void cFlockerManager::getClosestFlockers(iEnemy* flocker, std::vector<iEnemy*>* flockersOut)	//flockers out returns list of 10 flockers ordered closest to farthest
{
	glm::mat4 myTrans;
	flocker->GetModelMatrix(myTrans);
	glm::vec3 myPos = myTrans[3];

	glm::mat4 otherTrans;
	glm::vec3 otherPos;

	std::map<float/*distance*/,int/*index*/> distanceMap;
	std::multimap<float/*distance*/, int/*index*/> distanceMultiMap;

	for (int i = 0; i != m_pFlockers.size(); i++)
	{
		iEnemy* curFlocker = m_pFlockers[i];
		curFlocker->GetModelMatrix(otherTrans);
		otherPos = otherTrans[3];

		float distance = glm::distance(myPos, otherPos);
		//if(distance < 20)
		distanceMultiMap.insert(std::pair<float, int>(distance, i));

	}
	int i = 0;
	for (std::multimap<float, int>::iterator it = distanceMultiMap.begin(); it != distanceMultiMap.end(); it++, i++)
	{
		if (i > 9)
		{
			break;
		}
		else
		{
			int index = it->second;
			flockersOut->push_back(m_pFlockers[index]);
		}
	}
	return;
	
}

void cFlockerManager::Update(float dt)
{
	for (int i = 0; i != m_pFlockers.size(); i++)
	{
		m_pFlockers[i]->Update(dt);
	}
}

void cFlockerManager::getFlockers(std::vector<iEnemy*>* flockers)
{
	for (int i = 0; i != m_pFlockers.size(); i++)
	{
		flockers->push_back(m_pFlockers[i]);
	}
}