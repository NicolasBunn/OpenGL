#version 330                                                                        
                                                                                    
layout (location = 0) in vec3 Position;                                             
                                                                                    
uniform mat4 uMVPMatrix;                                                                  
                                                                                    
out vec3 TexCoord0;                                                                 
                                                                                    
void main()                                                                         
{                                                                                   
    vec4 WVP_Pos = uMVPMatrix * vec4(Position, 1.0);                                      
    //gl_Position = WVP_Pos.xyww;
	gl_Position = WVP_Pos;                                                     
    TexCoord0   = Position;                                                         
}