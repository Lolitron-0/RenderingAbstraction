#version 450 core

in VS_OUT {
	vec4 Color;
	vec2 TexCoord;
	float TexIndex;
} fs_in;

out vec4 fragmentColor; 

void main(){
	fragmentColor = fs_in.Color;

	if (fragmentColor.a == 0)
		discard;
}
