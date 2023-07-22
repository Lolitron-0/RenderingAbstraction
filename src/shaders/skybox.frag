#version 330 core

in vec3 TexCoords;

uniform samplerCube u_Skybox;

out vec4 fragmentColor;

void main()
{
	fragmentColor = texture(u_Skybox, TexCoords);
	//fragmentColor = vec4(0, 0, TexCoords.z, 1);
}