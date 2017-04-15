#ifndef _cAreaInfo_HG_
#define _cAreaInfo_HG_

#include "externals.h"

class cAreaInfo
{
public:
	cAreaInfo();
	~cAreaInfo();

	glm::vec3 Max;
	glm::vec3 Min;
		//here is some info about the boundaries of the area we're in
		//these are going to be soft and repeating
};

#endif