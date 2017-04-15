#pragma once
#include <vector>

#include <glm\fwd.hpp>
#include <glm\glm.hpp>

// include glew first
#define GLEW_STATIC
#include <GL\glew.h>
// include glfw
#include <GLFW\glfw3.h>

namespace nGL
{
	extern std::vector<const char*> Errors;

	void ErrorsToConsole();
	bool IsGLOK(GLenum glResult, bool pushError = true);
	bool IsThereAnError(bool pushError = true);
	bool IsThereAnError(const char* additionalErrorMessage);
	const char* ToConstChar(const GLubyte* glString);
	GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path);

}