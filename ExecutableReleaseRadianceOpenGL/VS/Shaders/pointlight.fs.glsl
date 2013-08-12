#version 330

in vec3 vNormal;
in vec3 vWorldPos;

out vec4 fFragColor;

struct BaseLight
{
	float AmbientIntensity;
	float DiffuseIntensity;
	vec3 Color;
};

struct Attenuation
{
	float constant;
	float linear;
	float exp;
};

struct DirectionalLight
{
	BaseLight base;
	vec3 Direction;
};

struct PointLight
{
	BaseLight base;
	vec3 position;
	Attenuation atten;
};

uniform int uNumPointLights;
uniform DirectionalLight uDirectionalLight;
uniform PointLight uPointLights;

vec4 CalcLightInternal(struct BaseLight light, vec3 lightDirection, vec3 normal)
{
	vec4 AmbientColor = vec4(light.Color , 0.0f) *
						light.AmbientIntensity;

	float DiffuseFactor = dot(normal, -lightDirection);

	vec4 DiffuseColor = vec4(0, 0, 0, 0);

	if (DiffuseFactor > 0)
	{
		DiffuseColor = vec4(light.Color, 1.0f) *
						light.DiffuseIntensity *
						DiffuseFactor;
	}

	return AmbientColor + DiffuseColor;
}

vec4 CalcPointLight(vec3 normal)
{
	vec3 lightDirection = vWorldPos - uPointLights.position;
	float distance = length(lightDirection);
	lightDirection = normalize(lightDirection);
	vec4 color = CalcLightInternal(uPointLights.base, lightDirection, normal);

	float attenuation = uPointLights.atten.constant + 
						uPointLights.atten.linear * distance +
						uPointLights.atten.exp * distance * distance;

	return color / attenuation;
}

vec4 CalcDirectionalLight(vec3 normal)
{
	return CalcLightInternal(uDirectionalLight.base, uDirectionalLight.Direction, normal);
}

void main() 
{
	vec3 normal = normalize(vNormal);

	vec4 totalLight = CalcDirectionalLight(normal);
	totalLight += CalcPointLight(normal);

	fFragColor = totalLight;

}

