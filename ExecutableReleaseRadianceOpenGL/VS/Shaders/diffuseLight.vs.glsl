#version 330

layout(location = 0) in vec3 iPos;
layout(location = 1) in vec3 iNormal;

out vec3 vNormal;

uniform mat4 uMVPMatrix;
uniform mat4 uWorld;

void main() {
  gl_Position = uMVPMatrix * vec4(iPos, 1.f);
  vNormal = (uWorld * vec4(iNormal, 0.0)).xyz;
}


