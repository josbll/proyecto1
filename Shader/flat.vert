//#version 400

uniform mat4 ModelView;
uniform mat4 Projection;
uniform mat4 NormalMatrix;
uniform float shinnes;

layout ( location = 0 ) in vec3 VertexPosition;
layout ( location = 1 ) in vec3 Normal;

flat out vec4 Color;

const vec3 lightPos = vec3(1.0, 1.0, 1.0);
const vec3 diffuseColor = vec3(0.5, 0.0, 0.0);
const vec3 specColor = vec3(0.0, 1.0, 1.0);


void main () 
{
	gl_Position = Projection * ModelView * vec4(VertexPosition,1.0);
	
	vec3 normal = vec3(NormalMatrix * vec4(Normal, 0.0));
	vec4 vertPos4 = ModelView * vec4(VertexPosition, 1.0);
	vec3 vertPos = vec3(vertPos4) / vertPos4.w;
	vec3 lightDir = normalize(lightPos - vertPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	vec3 viewDir = normalize(-vertPos);
	
	float lambertian = max(dot(lightDir,normal), 0.0);

	float specAngle = max(dot(reflectDir, viewDir), 0.0);
	float specular = pow(specAngle, shinnes);
	Color = vec4(lambertian*diffuseColor + specular*specColor, 1.0);
	
}

