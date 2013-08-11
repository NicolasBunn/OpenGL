#version 330

in vec3 vNormal;

out vec4 fFragColor;

struct Attenuation
{
	float constant;
	float linear;
	float exp;
};

struct Light
{
  float AmbientIntensity;
  float DiffuseIntensity;

  vec3 L;
  vec3 Position;
  vec3 Normal;
  vec3 Kd;

  Attenuation atten;

  int type;
};

uniform Light uLight;

void main() 
{
	vec4 AmbientColor = vec4(uLight.L , 0.0f) *
						uLight.AmbientIntensity;

	float DiffuseFactor = dot(normalize(vNormal), - uLight.Normal);

	vec4 DiffuseColor;

	if (DiffuseFactor > 0)
	{
		DiffuseColor = vec4(uLight.L, 1.0f) *
							uLight.DiffuseIntensity *
							DiffuseFactor;
	}
	else
	{
		DiffuseColor = vec4(0,0,0,0);
	}

	fFragColor = AmbientColor + DiffuseColor;
}

