#version 330                                                                        
                                                                                    
in vec3 TexCoord0;                                                                                                                                        

layout(location = 0) out vec4 oPosition;                                                                   
                                                                                    
uniform samplerCube gCubemapTexture;                                                
                                                                                    
void main()                                                                         
{                                                                                   
    oPosition = texture(gCubemapTexture, TexCoord0);                                
}
