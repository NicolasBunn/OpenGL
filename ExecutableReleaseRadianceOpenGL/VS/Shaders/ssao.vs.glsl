#version 330                                                                        
                                                                                    
layout (location = 0) in vec3 iPosition;
layout (location = 1) in vec4 iTexCoords;

out vec4 vTexCoords;
                                                                                                                                                                
void main()                                                                      
{                                                                                   
    gl_Position = vec4(iPosition, 1.0);  
	vTexCoords = iTexCoords;                                                                                             
}