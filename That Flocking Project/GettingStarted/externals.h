#pragma once
#include <limits>

#include <glm\fwd.hpp>
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\vec2.hpp>


// include glew first
#define GLEW_STATIC
#include <GL\glew.h>
// include glfw
#include <GLFW\glfw3.h>

inline void Set(glm::vec2& v, float x, float y)
{
	v.x = x; v.y = y;
}
inline void Set(glm::vec3& v, float x, float y, float z)
{
	v.x = x; v.y = y; v.z = z;
}
inline void Set(glm::vec4& v, float x, float y, float z, float w)
{
	v.x = x; v.y = y; v.z = z; v.w = w;
}
inline void SetMax(glm::vec3& v, float x, float y, float z)
{
	if (x > v.x) v.x = x;
	if (y > v.y) v.y = y;
	if (z > v.z) v.z = z;
}
inline void SetMin(glm::vec3& v, float x, float y, float z)
{
	if (x < v.x) v.x = x;
	if (y < v.y) v.y = y;
	if (z < v.z) v.z = z;
}
inline void SetMax(glm::vec3& v)
{
	v.x = std::numeric_limits<float>::max();
	v.y = std::numeric_limits<float>::max();
	v.z = std::numeric_limits<float>::max();
}
inline void SetMin(glm::vec3& v)
{
	v.x = std::numeric_limits<float>::min();
	v.y = std::numeric_limits<float>::min();
	v.z = std::numeric_limits<float>::min();
}