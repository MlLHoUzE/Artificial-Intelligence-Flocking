#ifndef _cEnemyFactory_HG_
#define _cEnemyFactory_HG_

// ONLY include the INTERFACE
// +++NEVER+++ the implementation (cpp) file
#include "iEnemy.h"	

#include <string>

class cEnemyFactory
{
public:
	// Returns a pointer to the INTERFACE type
	// Take some well known type (like int, string)
	iEnemy* CreateEnemy(std::string typeOfEnemy);

};

#endif