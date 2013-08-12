#version 330

layout(location = 0) in vec3 iPos;

uniform mat4 uMVPMatrix;

void main() {
  gl_Position = uMVPMatrix * vec4(iPos, 1.f);

}


