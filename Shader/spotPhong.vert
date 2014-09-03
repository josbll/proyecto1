#version 110

uniform mat4 ModelView;
uniform mat4 Projection;
uniform mat4 NormalMatrix;
//uniform vec3 lightposition;
uniform vec3 Ld;	
uniform vec3 Kd;


layout ( location = 0 ) in vec3 VertexPosition;
layout ( location = 1 ) in vec3 Normal;
layout ( location = 2 ) in vec3 VertexNormal;
flat out vec3 lightintensity;
flat out vec3 normal;
flat out vec3 pos;
flat out vec3 lightPos;
void main () 
{  
	vec4 p = ModelView * vec4(VertexPosition, 1.0);
	pos = p.xyz;
	//lightPos=lightposition;
	normal = normalize(mat3(NormalMatrix) * Normal);
	mat4 MVP = Projection * ModelView;
	
	gl_Position = MVP * vec4(VertexPosition,1.0);
}
