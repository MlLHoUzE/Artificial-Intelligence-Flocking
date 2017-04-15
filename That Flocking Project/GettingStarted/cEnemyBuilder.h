#ifndef _cEnemyBuilder_HG_
#define _cEnemyBuilder_HG_

#include <string>
#include "iEnemy.h"

class cEnemyBuilder
{
public:
	~cEnemyBuilder();

	static cEnemyBuilder* getInstance(void);

	static void shutDown(void);

	iEnemy* createEnemy(std::string type);

private:
	cEnemyBuilder();

	static int enemyID;
	static cEnemyBuilder* mPTheOneBuilder;
};

#endif
