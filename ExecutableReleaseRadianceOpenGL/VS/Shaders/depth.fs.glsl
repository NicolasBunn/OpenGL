#version 330                                                                        

in vec4 vPosition;
                                                                                                                                                                                      
layout(location = 0) out vec4 fragColor;                                                        
                                                                                    
void main()                                                                         
{                                                                                                                            
	float depth = vPosition.z;// / vPosition.w;

	depth = depth * 0.5 + 0.5; // scaling [-1..1] => [0..1]

	float moment1 = depth;
	float moment2 = depth * depth;

	float dx = dFdx(depth);
	float dy = dFdy(depth);

	moment2 += 0.25 * ( dx * dx + dy * dy);
      	  	  			                                                                                                                        
    fragColor = vec4(moment1, moment2,0.0,0.0);
}