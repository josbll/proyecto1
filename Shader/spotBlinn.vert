#version 110

uniform mat4 ModelView;
uniform mat4 Projection;
uniform mat4 NormalMatrix;
uniform vec3 lightposition;
uniform vec3 Ld;	
uniform vec3 Kd;


layout ( location = 0 ) in vec3 VertexPosition;
layout ( location = 1 ) in vec3 Normal;

flat out vec3 lightintensity;
flat out vec3 normal;
flat out vec3 pos;

vec2 blinnPhongDir(vec3 lightDir, float lightInt, float Ka, float Kd, float Ks, float shininess,float att)
{
  vec3 s = normalize(lightDir);
  vec3 v = normalize(-VertexPosition);
  vec3 n = normalize(Normal);
  vec3 h = normalize(v+s);
  float diffuse = Ka + Kd * lightInt * max(0.0, dot(n, s));
  float spec =  Ks * pow(max(0.0, dot(n,h)), shininess);
  return vec2(att*diffuse, att*spec);

}
  
void main () 
{
	vec4 p = ModelView * vec4(VertexPosition, 1.0);
	pos = p.xyz;
	
	  float spotEffect=1.2;
	  float spotExponent=3.0;
	      float Effect=pow(spotEffect,spotExponent);
		  
	   float att=Effect/2.0; //Igual que la formula de atenuación
	   
	//float att=1.0/0.1;
	vec3 lightDir=lightposition;
	vec2 light=blinnPhongDir(lightDir,6.0,0.2,0.2,0.3,5.0,att);
	  lightintensity=vec3(light,1.0);
	normal = normalize(mat3(NormalMatrix) * Normal);
	mat4 MVP = Projection * ModelView;
	gl_Position = MVP * vec4(VertexPosition,1.0);
}






