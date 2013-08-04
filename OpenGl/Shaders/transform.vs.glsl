#version 330

layout(location = 0) in vec3 iPos;
layout(location = 1) in vec3 iNormal;
layout(location = 2) in vec2 iTextCoord;


uniform mat4 uMVPMatrix;
// send to fragment shader
out vec3 vNormal;
out vec2 vTexCoords;

void main() {
  gl_Position = uMVPMatrix * vec4(iPos, 1.f);
  vNormal = iNormal;
  vTexCoords = iTextCoord;
}