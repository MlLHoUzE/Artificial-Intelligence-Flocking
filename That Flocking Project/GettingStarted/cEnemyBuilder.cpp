#include "cEnemyBuilder.h"
#include "cEnemyFactory.h"

cEnemyBuilder::~cEnemyBuilder()
{

}

cEnemyBuilder::cEnemyBuilder()
{
	return;
}

cEnemyBuilder* cEnemyBuilder::mPTheOneBuilder = 0;

int cEnemyBuilder::enemyID = 0;

cEnemyBuilder* cEnemyBuilder::getInstance(void)	//Singleton Step 1
{
	if (cEnemyBuilder::mPTheOneBuilder == 0)
	{
		cEnemyBuilder::mPTheOneBuilder = new cEnemyBuilder();
	}
	return cEnemyBuilder::mPTheOneBuilder;
}

void cEnemyBuilder::shutDown(void)
{
	// Do we have an instance?
	if (cEnemyBuilder::mPTheOneBuilder != 0)
	{	// Yes, so get rid of it
		delete cEnemyBuilder::mPTheOneBuilder;
	}
	return;
}

iEnemy* cEnemyBuilder::createEnemy(std::string type)
{
	iEnemy* pTheEnemy = 0;

	cEnemyFactory* pEnemyFactory = new cEnemyFactory();

	if (type == "Curious")
	{
		pTheEnemy = pEnemyFactory->CreateEnemy(type);
	}
	else if (type == "Angry")
	{
		pTheEnemy = pEnemyFactory->CreateEnemy(type);
	}
	else if (type == "Flocker")
	{
		pTheEnemy = pEnemyFactory->CreateEnemy(type);
	}
	pTheEnemy->SetID(cEnemyBuilder::enemyID);
	cEnemyBuilder::enemyID++;
	delete pEnemyFactory;

	return pTheEnemy;
}