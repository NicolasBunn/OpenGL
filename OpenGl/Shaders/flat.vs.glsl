#version 330                                                                        
                                                                                    
layout (location = 0) in vec3 Position;
//layout (location = 1) in vec3 InColor;

out vec3 color;

uniform mat4 uWVP;                                                                  
                                                                                               
void main()                                                                         
{
    gl_Position = uWVP * vec4(Position, 1.0);

	color = vec3(1.0,0.0,0.0);
}