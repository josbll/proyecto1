#pragma once
#include "GlobalHeader.h"
#include "Shader.h"

class LightSourceParameters
{
public:

	int type;
	bool status;

	glm::vec3 ambient;              // Aclarri   
	glm::vec3 diffuse;              // Dcli   
	glm::vec3 specular;             // Scli   
	glm::vec3 position;             // Ppli   
	
	glm::vec3 spotDirection;        // Sdli   
	float spotExponent;        // Srli   
	float spotCutoff;          // Crli                              
	                           // (range: [0.0,90.0], 180.0)   
	float spotCosCutoff;       // Derived: cos(Crli)                 
	                           // (range: [1.0,0.0],-1.0)   
	float constantAttenuation; // K0   
	float linearAttenuation;   // K1   
	float quadraticAttenuation;// K2  

	LightSourceParameters();
	~LightSourceParameters();

	void changeStatus(int);
};

