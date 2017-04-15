#pragma once

#include <glm\fwd.hpp>
#include <glm\glm.hpp>

namespace nGraphics
{
	struct sShaderPerItemVars
	{
		sShaderPerItemVars()
			: ModelColor(1.f, 1.f, 1.f, 1.f), ModelScale(1.f, 1.f, 1.f) {}

		glm::mat4	ModelMatrix;
		glm::vec4	ModelColor;
		glm::vec3	ModelScale;
	};

	struct sShaderPerFrameVars
	{
		glm::mat4	ViewMatrix;
		glm::mat4	ProjectionMatrix;
		glm::vec3	EyePosition;
	};
}