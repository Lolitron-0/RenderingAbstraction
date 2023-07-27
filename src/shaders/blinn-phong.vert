#version 330 core
layout(location=0) in vec3 a_Position;
layout(location=1) in vec3 a_Normal;
layout(location=2) in vec2 a_TexCoord;

uniform mat4 u_ViewProjection;
uniform mat4 u_LightSpaceMatrix;
uniform mat4 u_Model;
uniform mat3 u_NormalModel;

out VS_OUT {
	vec3 FragPos;
	vec2 TexCoord;
	vec3 Normal;
	vec4 FragPosLightSpace;
} vs_out;

void main()
{
	gl_Position = u_ViewProjection * u_Model * vec4(a_Position, 1.0);
	vs_out.FragPos = vec3(u_Model * vec4(a_Position, 1.));
	vs_out.TexCoord = a_TexCoord;
	vs_out.Normal = u_NormalModel * a_Normal;
	vs_out.FragPosLightSpace = u_LightSpaceMatrix * vec4(vs_out.FragPos, 1.);
}
