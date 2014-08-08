/*
	Para el uso de shaders es necesario tener una herramienta que se encargue de enviar los shaders
	a la tarjeta de video GLEW se encarga de hacer esto.
*/

#include <AntTweakBar.h>

#include <gl/glew.h>

#include <gl/glui.h>

// OpenGL Matemathics
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\transform2.hpp>
// glm::value_ptr
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <fstream>
#include <vector>
#include <time.h> 

using namespace std;

#define NADA 0
#define DIRECTIONALLIGHT 1
#define SPOTLIGHT 2
#define POINTLIGHT 3
#define ON true
#define OFF false