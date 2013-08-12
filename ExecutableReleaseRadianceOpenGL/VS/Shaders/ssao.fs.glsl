#version 330                                                                        

in vec4 vTexCoords;

uniform sampler2D uPosition;
uniform sampler2D uNormal;
uniform sampler2D uRandom;

uniform vec4 uEye;

uniform int uWidth;
uniform int uHeight;

uniform float uSSAOScale;
uniform float uSSAOBias;
uniform float uSSAOIntensity;
uniform float uSSAORadius;
                                                                                                                                                                                     
out vec4 fragColor;                                                                 

vec2 getRandom(vec2 uv)
{
	vec2 screen = vec2(uWidth, uHeight);
	return normalize(texture(uRandom, uv * screen / 75.0f).xy);
}
 
float doAO(vec2 uv, vec3 p, vec3 n)
{
	vec3 diff = texture(uPosition, vTexCoords.xy + uv).xyz - p;
	float d = length(diff);
	vec3 v = diff * ( 1 / d);

	d *= uSSAOScale;

	return max(0, dot(n,v) - uSSAOBias) * ( 1.0 / ( 1.0 + d)) * uSSAOIntensity;
} 
                                                                                    
void main()                                                                         
{              
	vec3 P = vec3(texture(uPosition, vTexCoords.xy));
	vec3 N = vec3(texture(uNormal, vTexCoords.xy));

	vec2 vec[4] = vec2[4](vec2(1,0) , vec2( -1,0), vec2(0, 1), vec2(0,-1));

	vec2 r = getRandom(vTexCoords.xy);
	float rad;

	int iter = 4;
	int samples = 8;
	float ao = 0.0f;

	for (int i = 0 ; i < samples; ++i)
	{
		rad = i * (uSSAORadius / samples);

		for (int j = 0 ; j < iter; ++j)
		{
			vec2 coord1 = reflect(vec[j], r) * rad;
			vec2 coord2 = vec2(coord1.x * 0.707 - coord1.y * 0.707, coord1.x * 0.707 + coord1.y * 0.707);

			ao += doAO(coord1 * 0.25 , P , N);
			ao += doAO(coord2 * 0.5, P, N);
			//ao += doAO(coord1 * 0.75, P, N);
			//ao += doAO(coord2, P, N);
		}
	}
    
	ao /= samples * iter * 4.0f;
	               					                                                                                                                                                           
    fragColor = vec4(1.0 - ao, 1.0 - ao, 1.0 - ao, 1.0f);

}