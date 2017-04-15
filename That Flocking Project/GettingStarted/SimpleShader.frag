#version 330 core

in vec4 ex_Color;
in vec3 ex_ToEye;
in vec3 ex_Normal;
//in vec4 ex_TexCoord;
in vec3 ex_EyePosition;
in vec3 ex_WorldPosition;

out vec4 out_Color;

void main()
{
	out_Color = vec4(ex_Color.xyz, 1.0f);
	//float dot = dot(normalize(ex_ToEye), normalize(ex_Normal)) * 0.7 + 0.3;
	//out_Color.x *= dot;
	//out_Color.y *= dot;
	//out_Color.z *= dot;
	//out_Color.w = 1.0f;
}