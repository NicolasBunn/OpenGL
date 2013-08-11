#version 330

out vec4 fFragColor;

struct DirectionalLight
{
	vec3 Color;
	float AmbientIntensity;
};

uniform DirectionalLight gDirectionalLight;

void main() 
{
    fFragColor = vec4(gDirectionalLight.Color, 1.0f) * gDirectionalLight.AmbientIntensity;
}

