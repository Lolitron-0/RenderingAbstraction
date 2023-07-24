#version 330 core
layout(location=0) in vec3 a_Position;
layout(location=1) in vec3 a_Normal;
layout(location=2) in vec2 a_TexCoord;

uniform mat4 u_ViewProjection;
uniform mat4 u_Model;
uniform mat3 u_NormalModel;

out vec3 Position;
out vec2 TexCoord;
out vec3 Normal;

void main()
{
	gl_Position = u_ViewProjection * u_Model * vec4(a_Position, 1.0);
	Position = vec3(u_Model * vec4(a_Position, 1.));
	TexCoord = a_TexCoord;
	Normal = u_NormalModel * a_Normal;
}
