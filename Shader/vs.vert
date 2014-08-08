//#version 400

// Material parameters
uniform vec3 Kd; // Diffuse reflectivity
uniform vec3 Ka; // Ambient reflectivity
uniform vec3 Ks; // Specular reflectivity
uniform float Shininess; // Specular shininess factor

uniform mat4 ModelView;
uniform mat4 Projection;
uniform mat4 NormalMatrix;

struct LightInfo {
	int type;
	bool status;

	vec3 ambient;              // Aclarri   
	vec3 diffuse;              // Dcli   
	vec3 specular;             // Scli   
	vec3 position;             // Ppli   
	vec3 spotDirection;        // Sdli   
	float spotExponent;        // Srli   
	float spotCutoff;          // Crli                              
	                           // (range: [0.0,90.0], 180.0)   
	float constantAttenuation; // K0   
	float linearAttenuation;   // K1   
	float quadraticAttenuation;// K2  
};
uniform LightInfo lights[5];

layout ( location = 0 ) in vec3 VertexPosition;
layout ( location = 1 ) in vec3 Normal;

flat out vec3 NormalI;
flat out vec3 VertexPositionI;

vec3 ads( int lightIndex, vec4 position, vec3 norm )
{
	vec3 s = normalize( vec3(lights[lightIndex].position – position) );
	vec3 v = normalize(vec3(-position));
	vec3 r = reflect( -s, norm );
	vec3 I = lights[lightIndex].Intensity;
	
	return
		I * ( Ka +
		Kd * max( dot(s, norm), 0.0 ) +
		Ks * pow( max( dot(r,v), 0.0 ), Shininess ) );
}

void main(){
    gl_Position = Projection * ModelView * vec4(VertexPosition, 1.0);
    vec4 vertPos4 = ModelView * vec4(VertexPosition, 1.0);
    VertexPositionI = vec3(vertPos4) / vertPos4.w;
    NormalI = vec3(NormalMatrix * vec4(Normal, 0.0));
}
