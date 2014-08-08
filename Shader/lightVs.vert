#version 400

uniform mat4 ModelView;
uniform mat4 Projection;
uniform mat4 NormalMatrix;

layout ( location = 0 ) in vec3 VertexPosition;
layout ( location = 1 ) in vec3 Normal;

flat out vec4 fNormal;

void main () 
{
	fNormal = NormalMatrix * vec4(Normal,0.0);

	mat4 MVP = Projection * ModelView;
	gl_Position = MVP * vec4(VertexPosition,1.0);
}