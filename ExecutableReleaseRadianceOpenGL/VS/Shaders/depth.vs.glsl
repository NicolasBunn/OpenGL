#version 330                                                                        
                                                                                    
layout (location = 0) in vec3 Position;                                                                                                                               

out vec4 vPosition;
                                                         
uniform mat4 uWVP;                                                                  
                                                                                               
void main()                                                                         
{                                                                                   
    gl_Position = uWVP * vec4(Position, 1.0);  
	vPosition = gl_Position;                                                                                             
}