#version 330 core

vec4 validateTexture(sampler2D tex, vec2 texCoord)
{
	if (textureSize(tex, 0) != ivec2(1,1))
		return texture(tex, texCoord);
	else
		return vec4(0,0,0,0);
}

bool textureValid(sampler2D tex)
{
	return textureSize(tex, 0) != ivec2(1,1);
}


struct Material 
{
	sampler2D DiffuseMap;
	sampler2D SpecularMap;
	vec3 BaseColor;
	float Transparency;
	float Shininess;
};

#define MAX_POINT_LIGHTS 128
struct PointLight {
	vec3 Position;
	vec3 Color;
};

in vec3 Position;
in vec2 TexCoord;
in vec3 Normal;

out vec4 fragmentColor;

uniform vec3 u_CameraPosition;
uniform Material u_Material;
uniform PointLight u_PointLights[MAX_POINT_LIGHTS];
uniform int u_PointLightsCount;

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 diffuseBase);

void main()
{
	vec4 result = vec4(0,0,0,0);
	vec4 base;
	if (textureSize(u_Material.DiffuseMap, 0) != ivec2(1,1))
	{
		base = texture(u_Material.DiffuseMap, TexCoord);
	}
	else
		base = vec4(u_Material.BaseColor,1);

	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(Position - u_CameraPosition);
	for (int i = 0; i < u_PointLightsCount; i++)
	{
		result += vec4(CalcPointLight(u_PointLights[i], norm, Position, viewDir, vec3(base)), 1.);
	}
	
	result.a = u_Material.Transparency;
	fragmentColor = result;
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 diffuseBase) {
	vec3 lightDir = normalize(fragPos - light.Position);
	vec3 norm = normalize(normal);

	// diffuse
	float diff = max(dot(norm, -lightDir), 0.0); // negate to match normal dir

	//specular
	vec3 reflectDir = reflect(lightDir, norm); //actual reflect
	float spec = pow(max(dot(reflectDir, -viewDir), 0.0), u_Material.Shininess);

	float constant = 1.;
	float linear = .09;
	float quadratic = .032;

	//attenuation
	float distance = length(light.Position - fragPos);
	float attenuation = 1.0 / (quadratic*(distance*distance) +
							   linear*distance + 
							   constant);


	vec3 ambient =  attenuation * light.Color * .2 * diffuseBase;
	vec3 diffuse =  attenuation * light.Color * .5 * (diff * diffuseBase);
	vec3 specBase;
	if (textureValid(u_Material.SpecularMap))
		specBase = vec3(texture(u_Material.SpecularMap, TexCoord));
	else
		specBase = diffuseBase;
	vec3 specular = attenuation * vec3(1,1,1) * (spec * specBase);

	return (ambient + diffuse + specular);
}