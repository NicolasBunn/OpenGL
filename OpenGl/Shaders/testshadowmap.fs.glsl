#version 330                                                                        

in vec4 vPosition;
in vec4 vNormal;
in vec3 vWorldPos;
in vec4 vShadowCoord;
in vec4 vShadowCoord2;
in vec4 vShadowCoord3;
in vec4 vSSAOCoords;
in vec3 vLightDirection;	

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

uniform sampler2D shadowMapNear;
uniform sampler2D shadowMapFar;
uniform sampler2D shadowMapMiddle;
                                                                                                                                                                                     
layout(location = 0) out vec4 oPosition;                                                               

vec2 poissonDisk[16] = vec2[]( 
   vec2( -0.94201624, -0.39906216 ), 
   vec2( 0.94558609, -0.76890725 ), 
   vec2( -0.094184101, -0.92938870 ), 
   vec2( 0.34495938, 0.29387760 ), 
   vec2( -0.91588581, 0.45771432 ), 
   vec2( -0.81544232, -0.87912464 ), 
   vec2( -0.38277543, 0.27676845 ), 
   vec2( 0.97484398, 0.75648379 ), 
   vec2( 0.44323325, -0.97511554 ), 
   vec2( 0.53742981, -0.47373420 ), 
   vec2( -0.26496911, -0.41893023 ), 
   vec2( 0.79197514, 0.19090188 ), 
   vec2( -0.24188840, 0.99706507 ), 
   vec2( -0.81409955, 0.91437590 ), 
   vec2( 0.19984126, 0.78641367 ), 
   vec2( 0.14383161, -0.14100790 ) 
);

float random(vec3 seed, int i)
{
	vec4 seed4 = vec4(seed, i);
	float dotProduct = dot(seed4, vec4(12.9298, 78.233, 164, 94.673));
	return fract(sin(dotProduct) * 43758.5453);
}

vec4 CalcLightInternal(struct Light light, vec3 normal, float ambient)
{
	vec4 ambientColor = vec4(light.L, 0.0f) * light.AmbientIntensity;

	float diffuseFactor = dot (normal, light.Normal);

	vec4 diffuseColor = vec4(0,0,0,0);

	if(diffuseFactor > 0)
	{
		diffuseColor = vec4(light.L , 1.0f) * light.DiffuseIntensity * diffuseFactor;
	}

	return ambientColor + diffuseColor;
}

vec4 CalcDirectionalLight(vec3 normal, float ambient)
{
	return CalcLightInternal(uLight,normal, ambient);
}

                                                                                    
void main()                                                                         
{                                                                                                                                                                                                                                                        
	vec3 N = normalize(vec3(vNormal));
	vec3 light = normalize(vLightDirection);
	vec3 ProjCoords;

	vec4 totalLight = CalcDirectionalLight(N, 1.0f);

	float shadowFactor = 1.0f;
	float bias = 0.005f;

	int typeShadowMap; // 0 = near, 1 = medium, 2 = far

	if(vWorldPos.z < 15.0f)
	{
		typeShadowMap = 0;
		oPosition = vec4(0.5f, 0.0f, 0.0f, 0.0f);
	}
	else if(vWorldPos.z > 15.0f && vWorldPos.z < 30.0f)
	{
		typeShadowMap = 1;
		oPosition = vec4(0.0f, 0.0f, 5.0f, 0.0f);
	}
	else
	{
		typeShadowMap = 2;
		oPosition = vec4(0.0f, 0.5f, 0.0f, 0.0f);
	}

	
	if(typeShadowMap == 0)
	{
		ProjCoords =  vShadowCoord.xyz / vShadowCoord.w;
		bias = 0.01f;
	}
	else if (typeShadowMap == 1)
	{
		ProjCoords =  vShadowCoord3.xyz / vShadowCoord3.w;
		bias = 0.01f;
	}
	else if (typeShadowMap == 2)
	{
		ProjCoords =  vShadowCoord2.xyz / vShadowCoord2.w;
		bias = 0.009f;
	}

	for (int i = 0 ; i < 4 ; ++i)
	{
		if (typeShadowMap == 0)
		{
			shadowFactor -= 0.2 * (1.0 - ( texture(shadowMapNear, ProjCoords.xy + poissonDisk[i]/700.0, (ProjCoords.z - bias)).x));
		}
		else if (typeShadowMap == 1)
		{
			shadowFactor -= 0.2 * (1.0 - ( texture(shadowMapMiddle, ProjCoords.xy + poissonDisk[i]/700.0, (ProjCoords.z - bias)).x));
		}
		else if (typeShadowMap == 2)
		{
			shadowFactor -= 0.2 * (1.0 - ( texture(shadowMapFar, ProjCoords.xy + poissonDisk[i]/700.0, (ProjCoords.z - bias)).x));
		}
	}

	oPosition = totalLight * shadowFactor;

}