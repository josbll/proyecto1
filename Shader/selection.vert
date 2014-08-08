#version 400

uniform mat4 ModelView;
uniform mat4 Projection;

layout ( location = 0 ) in vec3 VertexPosition;
layout ( location = 1 ) in vec3 Normal;

void main()
{
	mat4 MVP = Projection * ModelView;
	gl_Position = MVP * vec4(VertexPosition,1.0);
} 
