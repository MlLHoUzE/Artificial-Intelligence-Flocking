#include "cEnemyFactory.h"

// HERE you include the SPECFIC classes
// ONLY here, NEVER in the .h file
#include "cCurious.h"

#include "cAngry.h"   //Factory step 2

#include "cFlocker.h"

// Returns a pointer to the INTERFACE type
// Take some well known type (like int, string)
iEnemy* cEnemyFactory::CreateEnemy(std::string typeOfEnemy)
{
	iEnemy* pEnemyToReturn = 0;	// Or NULL or nullprt

	if (typeOfEnemy == "Curious")
	{
		pEnemyToReturn = new cCurious();
	}
	else if (typeOfEnemy == "Angry")
	{
		pEnemyToReturn = new cAngry();	//Factory step 3
	}
	else if (typeOfEnemy == "Flocker")
	{
		pEnemyToReturn = new cFlocker();
	}

	return pEnemyToReturn;
}
