#include "Light.h"

LightSourceParameters::LightSourceParameters()
{
	type = DIRECTIONALLIGHT;
	status = ON;

	ambient = glm::vec3(0.8f,0.0f,0.0f);    
	diffuse = glm::vec3(0.0f,0.0f,0.8f);          
	specular = glm::vec3(0.0f,0.8f,0.0f);        
	position = glm::vec3(0.8f,0.8f,0.8f);        
    
	spotDirection = glm::vec3(8.0f,-5.0f,0.0f);   
	spotExponent = 0;        
	spotCutoff = 0;                                       
	spotCosCutoff = 0;                        
	constantAttenuation = 0; 
	linearAttenuation = 0;   
	quadraticAttenuation = 0;
}

LightSourceParameters::~LightSourceParameters()
{
}

void LightSourceParameters::changeStatus(int s){

	//if(s=

}