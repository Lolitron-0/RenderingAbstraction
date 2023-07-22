#version 330 core
#define MAX_POINT_LIGHTS 128
#define LIGHT_AMBIENT 0.2
#define LIGHT_DIFFUSE 0.5
#define LIGHT_SPECULAR 1.0

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
	sampler2D DiffuseMap1;
	sampler2D DiffuseMap2;
	sampler2D DiffuseMap3;
	sampler2D DiffuseMap4;
	sampler2D SpecularMap1;
	sampler2D SpecularMap2;
	sampler2D SpecularMap3;
	sampler2D SpecularMap4;
	sampler2D NormalMap1;
	sampler2D NormalMap2;
	sampler2D NormalMap3;
	sampler2D NormalMap4;
	vec3 BaseColor;
	float Opacity;
	float Reflection;
	float Refraction;
	float RefractiveIndex;
	float Shininess;
	bool SkipLight;
};

struct PointLight {
	vec3 Position;
	vec3 Color;
	float Intensity;
};

struct DirLight {
	vec3 Direction;
	vec3 Color;
	float Intensity;
};

in vec3 Position;
in vec2 TexCoord;
in vec3 Normal;

layout(location=0) out vec4 fragmentColor;
layout(location=1) out int entityId;

uniform vec3 u_CameraPosition;
uniform Material u_Material;
uniform PointLight u_PointLights[MAX_POINT_LIGHTS];
uniform int u_PointLightsCount;
uniform DirLight u_DirLight;
uniform int u_DirLightsCount;
uniform samplerCube u_Environment;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec3 diffuseBase);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 diffuseBase);
vec3 CalcEnvironmentMap(samplerCube environment, vec3 normal, vec3 viewDir);

void main()
{
	vec3 result = vec3(0);
	vec4 base;
	if (textureSize(u_Material.DiffuseMap1, 0) != ivec2(1,1))
	{
		base = texture(u_Material.DiffuseMap1, TexCoord);
	}
	else
		base = vec4(u_Material.BaseColor, 1); 

	if (base.a <= 0.05)
		discard;

	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(Position - u_CameraPosition);
	if (!u_Material.SkipLight)
	{

		// Directional lights
		for (int i = 0; i < u_DirLightsCount; i++)
		{
			result += CalcDirLight(u_DirLight, norm, viewDir, vec3(base));
		}

		// Point lights
		for (int i = 0; i < u_PointLightsCount; i++)
		{
			result += CalcPointLight(u_PointLights[i], norm, Position, viewDir, vec3(base));
		}
	}
	else
	{
		result = vec3(base);
	}
	
	result += CalcEnvironmentMap(u_Environment, norm, viewDir);
	fragmentColor = vec4(result,1);
	fragmentColor.a = u_Material.Opacity;
	entityId = 234;
}


vec3 CalcEnvironmentMap(samplerCube environment, vec3 normal, vec3 viewDir)
{
	vec3 reflected = reflect(viewDir, normal);
	vec3 reflectionRes = u_Material.Reflection * texture(environment, reflected).rgb;

	float ratio = 1.0/u_Material.RefractiveIndex;
	vec3 refracted = refract(viewDir, normal, ratio);
	vec3 refractionRes = u_Material.Refraction * texture(environment, refracted).rgb;
	return reflectionRes + refractionRes;
}


vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec3 diffuseBase) {	
	vec3 lightDir = normalize(light.Direction);

	// diffuse
	float diff = max(dot(normal, -lightDir), 0.0); // negate to match normal dir

	//specular
	vec3 reflectDir = reflect(lightDir, normal); //actual reflect
	float spec = pow(max(dot(reflectDir, -viewDir), 0.0), u_Material.Shininess);

	vec3 ambient = LIGHT_AMBIENT * light.Color * diffuseBase;
	vec3 diffuse = LIGHT_DIFFUSE *  light.Color * light.Intensity * (diff * diffuseBase);

	vec3 specBase;
	if (textureValid(u_Material.SpecularMap1))
		specBase = vec3(texture(u_Material.SpecularMap1, TexCoord));
	else
		specBase = diffuseBase;

	vec3 specular = light.Intensity * LIGHT_SPECULAR * (spec * specBase);

	return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 diffuseBase) {
	vec3 lightDir = normalize(fragPos - light.Position);

	// diffuse
	float diff = max(dot(normal, -lightDir), 0.0); // negate to match normal dir

	//specular
	vec3 reflectDir = reflect(lightDir, normal); //actual reflect
	float spec = pow(max(dot(reflectDir, -viewDir), 0.0), u_Material.Shininess);

	float constant = 1.;
	float linear = .09;
	float quadratic = .032;

	//attenuation
	float distance = length(light.Position - fragPos);
	float attenuation =  1.0 / (quadratic*(distance*distance) +
							   linear*distance + 
							   constant);


	vec3 ambient =  LIGHT_AMBIENT * light.Color * attenuation * diffuseBase;
	vec3 diffuse =  LIGHT_DIFFUSE * light.Color * light.Intensity * attenuation * (diff * diffuseBase);
	vec3 specBase;
	if (textureValid(u_Material.SpecularMap1))
		specBase = vec3(texture(u_Material.SpecularMap1, TexCoord));
	else
		specBase = diffuseBase;
	vec3 specular = light.Intensity * attenuation * LIGHT_SPECULAR *  vec3(1,1,1) * (spec * specBase);

	return (ambient + diffuse + specular);
}