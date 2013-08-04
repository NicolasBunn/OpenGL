#version 330                                                                        
                                                                                    
layout (location = 0) in vec3 iPosition;                                                                                                                               
layout (location = 1) in vec3 iNormal;

out vec4 vPosition;
out vec4 vNormal;
out vec3 vWorldPos;
out vec4 vShadowCoord;
out vec4 vShadowCoord2;
out vec4 vShadowCoord3;
out vec3 vLightDirection;
out vec4 vSSAOCoords;
                                                         
uniform mat4 uMVPMatrix;
uniform mat4 uWorld;

uniform mat4 uViewMatrix;
uniform mat4 uModelMatrix;
uniform vec3 uLightDirection;
uniform mat4 uDepthBiasMVP;
uniform mat4 uDepthBiasMVP2;
uniform mat4 uDepthBiasMVP3;
uniform mat4 uSSAOBiasMVP;
                                                                
                                                                                               
void main()                                                                         
{                                                                                   
    gl_Position = uMVPMatrix * vec4(iPosition, 1.0);  
	vPosition = vec4(iPosition, 1.0) * uMVPMatrix;
	vNormal = uWorld * vec4(iNormal, 1.0);
	vShadowCoord = uDepthBiasMVP * vec4(iPosition, 1);
	vShadowCoord2 = uDepthBiasMVP2 * vec4(iPosition, 1);
	vShadowCoord3 = uDepthBiasMVP3 * vec4(iPosition, 1);
	vWorldPos = (uMVPMatrix * vec4(iPosition, 1.f)).xyz;
	vLightDirection = (uViewMatrix * vec4(uLightDirection,0)).xyz;
	vSSAOCoords = uSSAOBiasMVP * vec4(iPosition, 1.f);
	                                                                                             
}