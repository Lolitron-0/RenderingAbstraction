#version 330 core
#define MAX_POINT_LIGHTS 128
#define LIGHT_AMBIENT 0.1
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
	bool CastShadows;
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

in VS_OUT {
	vec3 FragPos;
	vec2 TexCoord;
	vec3 Normal;
	vec4 FragPosLightSpace;
} fs_in;

layout(location=0) out vec4 fragmentColor;
//layout(location=1) out int entityId;

uniform vec3 u_CameraPosition;
uniform Material u_Material;
uniform PointLight u_PointLights[MAX_POINT_LIGHTS];
uniform int u_PointLightsCount;
uniform DirLight u_DirLight;
uniform int u_DirLightsCount;
uniform samplerCube u_Environment;
uniform sampler2D u_ShadowMap;


vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec3 diffuseBase, float shadow);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 diffuseBase, float shadow);
vec3 CalcEnvironmentMap(samplerCube environment, vec3 normal, vec3 viewDir);
float CalcShadow(DirLight light, vec4 lightSpaceFragPos, vec3 normal);

void main()
{
	vec3 result = vec3(0);
	vec4 base;
	if (textureSize(u_Material.DiffuseMap1, 0) != ivec2(1,1))
	{
		base = texture(u_Material.DiffuseMap1, fs_in.TexCoord);
	}
	else
		base = vec4(u_Material.BaseColor, 1); 

	if (base.a <= 0.05)
		discard;

	vec3 norm = normalize(fs_in.Normal);
	vec3 viewDir = normalize(fs_in.FragPos - u_CameraPosition);

	float shadow = CalcShadow(u_DirLight, fs_in.FragPosLightSpace, norm);

	// Directional lights
	for (int i = 0; i < u_DirLightsCount; i++)
	{
		result += CalcDirLight(u_DirLight, norm, viewDir, vec3(base), shadow);
	}

	// Point lights
	for (int i = 0; i < u_PointLightsCount; i++)
	{
		result += CalcPointLight(u_PointLights[i], norm, fs_in.FragPos, viewDir, vec3(base), shadow);
	}

	
	result += CalcEnvironmentMap(u_Environment, norm, viewDir);
	fragmentColor = vec4(result,1);
	fragmentColor.a = u_Material.Opacity;

	// Gamma correction
	float gamma = 2.2;
	fragmentColor.rgb = pow(fragmentColor.rgb, vec3(1.0/gamma));
	
	//fragmentColor.rgb = texture(u_ShadowMap, ((fs_in.FragPosLightSpace.xyz / fs_in.FragPosLightSpace.w)*0.5+0.5).xy).rrr;
	//fragmentColor.rgb = (fs_in.FragPosLightSpace.xyz / fs_in.FragPosLightSpace.w)*0.5+0.5;

	//entityId = 234;
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


vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec3 diffuseBase, float shadow) {	
	vec3 lightDir = normalize(light.Direction);
	vec3 halfway = normalize(-lightDir - viewDir);

	// diffuse
	float diff = max(dot(normal, -lightDir), 0.0); // negate to match normal dir

	//specular
	vec3 reflectDir = reflect(lightDir, normal); //actual reflect
	float spec = pow(max(dot(halfway, normal), 0.0), u_Material.Shininess);

	vec3 ambient = LIGHT_AMBIENT * light.Color * diffuseBase;
	vec3 diffuse = LIGHT_DIFFUSE *  light.Color * light.Intensity * (diff * diffuseBase);

	vec3 specBase;
	if (textureValid(u_Material.SpecularMap1))
		specBase = vec3(texture(u_Material.SpecularMap1, fs_in.TexCoord));
	else
		specBase = diffuseBase;

	vec3 specular = light.Intensity * LIGHT_SPECULAR * (spec * specBase);

	return (ambient + (1.0 - shadow) * (diffuse + specular));
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 diffuseBase, float shadow) {
	vec3 lightDir = normalize(fragPos - light.Position);
	vec3 halfway = normalize(-lightDir - viewDir);

	// diffuse
	float diff = max(dot(normal, -lightDir), 0.0); // negate to match normal dir

	//specular
	vec3 reflectDir = reflect(lightDir, normal); //actual reflect
	float spec = pow(max(dot(halfway, normal), 0.0), u_Material.Shininess);

	float constant = 1.;
	float linear = .09;
	float quadratic = .032;

	//attenuation
	float distance = length(light.Position - fragPos);
//	float attenuation =  1.0 / (quadratic*(distance*distance) +
//							   linear*distance + 
//							   constant);
	float attenuation = 1.0 / (distance*distance);


	vec3 ambient =  LIGHT_AMBIENT * light.Color * attenuation * diffuseBase;
	vec3 diffuse =  LIGHT_DIFFUSE * light.Color * light.Intensity * attenuation * (diff * diffuseBase);
	vec3 specBase;
	if (textureValid(u_Material.SpecularMap1))
		specBase = vec3(texture(u_Material.SpecularMap1, fs_in.TexCoord));
	else
		specBase = diffuseBase;
	vec3 specular = light.Intensity * attenuation * LIGHT_SPECULAR *  vec3(1,1,1) * (spec * specBase);

	return (ambient + (1.0 - shadow) * (diffuse + specular));
}

float CalcShadow(DirLight light, vec4 lightSpaceFragPos, vec3 normal)
{
	vec3 lightDir = normalize(light.Direction);

	vec3 projCoords = lightSpaceFragPos.xyz / lightSpaceFragPos.w;
	projCoords = projCoords * 0.5 + 0.5;
	float currentDepth = projCoords.z;
	float bias = max(0.0005, 0.005 * (1.0 - dot(normal, -lightDir)));
	//bias = 0.0005;

	float shadow = 0;
	vec2 texelSize = 1.0 / textureSize(u_ShadowMap, 0);

	int sampleCount = 0;
	for (int x = -2; x <= 2; x++)
	{
		for (int y = -2; y <= 2; y++)
		{
			float pcfDepth = texture(u_ShadowMap, projCoords.xy + vec2(x,y)*texelSize).r;
			shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
			sampleCount++;
		}
	}
	shadow /= sampleCount;

	if (projCoords.z > 1.0)
		return 0.0;

	return shadow;
}
