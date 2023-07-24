#version 330 core

in vec3 TexCoords;

uniform samplerCube u_Skybox;

out vec4 fragmentColor;

void main()
{
	fragmentColor = texture(u_Skybox, TexCoords);
	
	// Gamma correction
	float gamma = 2.2;
	fragmentColor.rgb = pow(fragmentColor.rgb, vec3(1.0/gamma));
}