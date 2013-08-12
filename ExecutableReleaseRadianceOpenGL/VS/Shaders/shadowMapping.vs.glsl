#version 330

layout(location = 0) in vec3 iPos;
layout(location = 1) in vec3 iNormal;

out vec3 vWorldNormal;
out vec3 vWorldPos;
out vec3 vWVPPos;
out vec3 vModelPos;

//Differentes coordonnées en fonction de la shadow map choisie
out vec4 vShadowCoordNear;
out vec4 vShadowCoordFar;
out vec4 vShadowCoordMiddle;

uniform mat4 uMVPMatrix; //MVP : Model View Projection
uniform mat4 uModelMatrix; //Model matrix only (goes to world position...)

//Matrice pour calculer les coordonnées des ombres
uniform mat4 uDepthBiasMVPNear;
uniform mat4 uDepthBiasMVPFar;
//uniform mat4 uDepthBiasMVPMiddle;

void main() 
{
  gl_Position = uMVPMatrix * vec4(iPos, 1.f);
  
  vWVPPos = vec3(gl_Position/gl_Position.w);
  vModelPos = iPos;
  
  vWorldPos = (uModelMatrix*vec4(iPos, 1.0f)).xyz;
  vWorldNormal = (uModelMatrix*vec4(iNormal, 1.0f)).xyz;


  vShadowCoordNear = uDepthBiasMVPNear * vec4(iPos, 1);
  vShadowCoordFar = uDepthBiasMVPFar * vec4(iPos, 1);
  // = uDepthBiasMVPMiddle * vec4(iPos, 1);


}

