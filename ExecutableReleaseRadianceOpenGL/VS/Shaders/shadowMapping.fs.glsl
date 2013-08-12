#version 330

in vec3 vWorldNormal;
in vec3 vWorldPos;
in vec4 vShadowCoordNear;
in vec4 vShadowCoordFar;
in vec4 vShadowCoordMiddle;
in vec3 vWVPPos;	
in vec3 vModelPos;

layout(location = 0) out vec4 oDiffuseLight;
layout(location = 1) out vec4 oShadow;

struct Light
{
	float AmbientIntensity;
	float DiffuseIntensity;
	
	vec3 L;
	vec3 Position;
	vec3 Normal;
};

uniform Light uLight;

uniform sampler2D varianceShadowMapFar;
uniform sampler2D varianceShadowMapMiddle;
uniform sampler2D varianceShadowMapNear;

uniform vec3 uMinNearBox;
uniform vec3 uMaxNearBox;
uniform vec3 uMinMiddleBox;
uniform vec3 uMaxMiddleBox;

uniform mat4 uMVPMatrix; //MVP : Model View Projection

const vec2 scaleUV = vec2(1.0/1024.0);

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

vec4 blurComponent(sampler2D tex,vec2 aTexCoord)
{
	vec4 res = vec4(0.0);

	res += texture2D( tex, aTexCoord + vec2( -3.0*scaleUV.x, -3.0*scaleUV.y ) )*0.015625;
	res += texture2D( tex, aTexCoord + vec2( -2.0*scaleUV.x, -2.0*scaleUV.y ) )*0.09375;
	res += texture2D( tex, aTexCoord + vec2( -1.0*scaleUV.x, -1.0*scaleUV.y ) )*0.234375;
	res += texture2D( tex, aTexCoord + vec2( 0.0 , 0.0) )*0.3125;
	res += texture2D( tex, aTexCoord + vec2( 1.0*scaleUV.x,  1.0*scaleUV.y ) )*0.234375;
	res += texture2D( tex, aTexCoord + vec2( 2.0*scaleUV.x,  2.0*scaleUV.y ) )*0.09375;
	res += texture2D( tex, aTexCoord + vec2( 3.0*scaleUV.x, -3.0*scaleUV.y ) )*0.015625;

	return res;
}

//Calcul de la variance shadow map (pas utilisé)
float chebyshev(int typeShadowMap)
{
	vec2 moments;
	float distance;
	vec4 c;

	if(typeShadowMap == 0)
	{
		distance = vShadowCoordFar.z;
		c = blurComponent(varianceShadowMapFar,vShadowCoordFar.xy);
	}
	
	if(typeShadowMap == 1)
	{
		// Pas utilisé pour l'instan
		distance = vShadowCoordNear.z;
		c = blurComponent(varianceShadowMapNear,vShadowCoordNear.xy);
		//moments = texture2D(varianceShadowMapMiddle, vShadowCoordMiddle.xy).rg;
	}

	if(typeShadowMap == 2)
	{
		distance = vShadowCoordNear.z;
		c = blurComponent(varianceShadowMapNear,vShadowCoordNear.xy);
	}

	moments = c.rg;

	if(distance < moments.x)
	{
		return 1.0f;
	}

	float variance = moments.y - (moments.x * moments.x);
	variance = max(variance, 0.00002);

	float d = distance - moments.x;
	float p_max = variance / (variance + d * d);

	return p_max;
}

//Calcul de la lumiere ambiente et diffuse
vec4 CalcLightInternal(struct Light light, vec3 normal)
{
	float diffuseFactor = dot (normal, light.Normal);

	vec4 diffuseColor = vec4(0,0,0,0);

	if(diffuseFactor > 0)
	{
		diffuseColor = vec4(light.L , 1.0f) * light.DiffuseIntensity * diffuseFactor;
	}

	return diffuseColor;
}

vec4 CalcDirectionalLight(vec3 normal)
{
	return CalcLightInternal(uLight,normal);
}

void main() 
{
	vec3 projCoordShadow;
	vec3 normal = normalize(vWorldNormal);

	//Couleur separant les différentes shadow maps
	vec3 colorShadow;
	vec4 projPosition = uMVPMatrix * vec4(vModelPos, 1.f);
	 projPosition /= projPosition.w;

	float shadowFactor = 1.0f;

	//Biais de la shadow map
	float bias = 0.01f;

	int typeShadowMap; // 0 = proche, 1 = moyenne, 2 = eloigné

	/*
	float normalizedDistance = projPosition.z;
	// La borne consiste en (d*(f+n)-2fn)/(d*(f-n)) ou d = profondeur champs near, f : far plan, d : near plane
	if (normalizedDistance < 0.807228916f) {
		colorShadow = vec3(1.0f, 0.0f, 0.0f);
		typeShadowMap = 2;
	}
	else {
		colorShadow = vec3(0.0f, 1.0f, 0.0f);
	}
	*/
	
	//Selection de la shadow map en fonction du vertex dans le repere monde..
	/*
	// DEBUG ZONE
	if(vMVPPos.x < uMaxNearBox.x && vMVPPos.x > uMinNearBox.x && vMVPPos.y < uMaxNearBox.y &&  vMVPPos.z < uMaxNearBox.z)
	{
		typeShadowMap = 0;
		//Couleur montrant les limites de chaque shadow map
		colorShadow = vec3(1.0f, 0.0f,0.0f);
	}
	else if(vMVPPos.x < uMaxMiddleBox.x && vMVPPos.x > uMinMiddleBox.x && vMVPPos.y < uMaxMiddleBox.y && vMVPPos.z > uMinMiddleBox.z && vMVPPos.z < uMaxMiddleBox.z)
	{
		colorShadow = vec3(0.0f, 1.0f,0.0f);
		typeShadowMap = 1;
	}
	else
	{
		colorShadow = vec3(0.0f, 0.0f,1.0f);
		typeShadowMap = 2;
	}
	*/

	/* 
	// DEBUG ZONE
	vec2 moments = texture2D(varianceShadowMapNear, vShadowCoordNear.xy).rg;
	if (vShadowCoordNear.z < moments.x + bias) {
		shadowFactor = 1.0;
	}
	else {
		shadowFactor = 0.0;
	}
	*/
	
	typeShadowMap = 0;
	
		shadowFactor = chebyshev(typeShadowMap);
	//shadowFactor = texture2D(varianceShadowMapFar, vShadowCoordFar.xy).r;

	oDiffuseLight = CalcDirectionalLight(normal);
	oShadow =  vec4(colorShadow.x,colorShadow.y,colorShadow.z,shadowFactor);
}
