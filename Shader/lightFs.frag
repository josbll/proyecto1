#version 400

uniform vec3 Color;

in vec4 fNormal;

layout ( location = 0 ) out vec4 FragColor;

void main () {
	gl_FragColor = normalize(vec4(Color,1.0) + fNormal * 2);
}