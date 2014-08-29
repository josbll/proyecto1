#version 110



uniform vec3 Color;

layout ( location = 0 ) in vec3 VertexPosition;
layout ( location = 1 ) in vec3 Normal;
flat in vec3 lightintensity;

uniform vec3 code;
//flat in vec3 lightintensity;
//flat in vec3 normal;
//flat in vec3 pos;

 

	
void main()
{   
     
    
	gl_FragColor = vec4(lightintensity,1.0);
} 
