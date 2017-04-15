#version 330 core

layout(location=0) in vec4 in_Position;
layout(location=1) in vec4 in_TexCoord;
layout(location=2) in vec4 in_Normal;

out vec4 ex_Color;
out vec3 ex_ToEye;
out vec3 ex_Normal;
//out vec4 ex_TexCoord;
out vec3 ex_EyePosition;
out vec3 ex_WorldPosition;

// per frame
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform vec4 EyePosition;
// per item
uniform mat4 ModelMatrix;
uniform vec4 ModelColor;

void main()
{
	gl_Position = ModelMatrix * in_Position;
	ex_WorldPosition = gl_Position.xyz;
	ex_EyePosition = EyePosition.xyz;
	ex_ToEye = normalize(ex_EyePosition - gl_Position.xyz);
	//ex_Normal = in_Normal.xyz;
	ex_Normal = (ModelMatrix * in_Normal).xyz;
	//ex_Normal = (ViewMatrix * ModelMatrix * in_Normal).xyz;
	//ex_TexCoord = vec4(in_TexCoord.x, in_TexCoord.y, 0.0f, 0.0f);

	gl_Position = ProjectionMatrix * ViewMatrix * gl_Position;
	//ex_Color = vec4(ModelColor.r * in_Color.r, ModelColor.g * in_Color.g, ModelColor.b * in_Color.b, ModelColor.a * in_Color.a);
	ex_Color = vec4(ModelColor.r, ModelColor.g, ModelColor.b, ModelColor.a);
}

