#version 330

//Ce fragment shader combine tout les effets pour le rendu final : SSAO, diffuse & ambient light et ombrage                                                                        
in vec4 vTexCoords;

out vec4 fragColor;                                                                 

uniform sampler2D uSkybox;
uniform sampler2D uShadow;
uniform sampler2D uDiffuseLight;

uniform vec3 ambientColor;
uniform float ambientIntensity;

uniform float activeShadow;
                                                                                    
void main()                                                                         
{   
	//Shadow
	float shadow = texture(uShadow, vTexCoords.xy).w;
	vec3 selectShadow = vec3(texture(uShadow, vTexCoords.xy));
	vec3 skybox = vec3(texture(uSkybox, vTexCoords.xy));
	vec4 diffuse = texture(uDiffuseLight, vTexCoords.xy);

	// DEBUG ZONE
	//fragColor = vec4(shadow,shadow,shadow,1.0);
	//fragColor = vec4(skybox.x,skybox.y,skybox.z,1.0);
	//fragColor = vec4(selectShadow,1.0);
	//fragColor = diffuse;
	//fragColor = vec4(vTexCoords.xy,1.0,1.0);
	//return;

	vec4 ambient = vec4(ambientColor, 0.0f) * vec4(0.65f, 0.65f, 0.65f, 1.0f);

	//Si le fond est noir (fond par défaut) application de la skybox
	if (diffuse.x == 1.0f && diffuse.y == 1.0f)
	{
		fragColor = vec4(skybox.x , skybox.y , skybox.z , 0.0f);
	}
	else
	{
		if(activeShadow == 1.0f)
		{
			fragColor = diffuse * shadow;
		}
		else
		{
			fragColor = diffuse ;
		}

	}
		      			                                                                                                                                                                                             
}