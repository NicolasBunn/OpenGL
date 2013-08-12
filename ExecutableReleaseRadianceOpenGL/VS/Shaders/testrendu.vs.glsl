#version 330                                                                        
                                                                                    
layout (location = 0) in vec3 iPos;
layout (location = 1) in vec4 iTex;                                                                                                                               

out vec4 vTexCoords;
                                                                                                                                                                                          
void main()                                                                         
{                                                                                   
    gl_Position = vec4(iPos, 1.0f);  
	vTexCoords = iTex;                                                                                             
}