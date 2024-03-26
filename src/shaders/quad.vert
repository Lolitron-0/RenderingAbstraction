#version 450 core
layout(location=0) in vec3 a_Position;
layout(location=1) in vec4 a_Color;
layout(location=2) in vec2 a_TexCoord;
layout(location=3) in float a_TexIndex;

out VS_OUT {
	vec4 Color;
	vec2 TexCoord;
	float TexIndex;
} vs_out;

uniform mat4 u_ViewProjection;

void main() {
	gl_Position = u_ViewProjection * vec4(a_Position, 1.0); 
	vs_out.Color = a_Color;
	vs_out.TexCoord = a_TexCoord;
	vs_out.TexIndex = a_TexIndex;
}



