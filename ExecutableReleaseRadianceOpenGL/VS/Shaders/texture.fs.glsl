#version 330

in vec3 vNormal;
in vec2 vTexCoords;

uniform sampler2D gSampler;

out vec4 fFragColor;

void main() {
    //fFragColor = vec4(vTexCoords, 0.f, 0.f);
	fFragColor = texture2D(gSampler, vTexCoords.xy); 
}