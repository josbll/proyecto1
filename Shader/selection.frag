#version 400

uniform vec3 code;

void main()
{
	gl_FragColor = vec4(code.x/255.0);
} 
