#pragma once
#include "GlobalHeader.h"

class Shader {
 
private:

	GLuint Vertex, Fragment;

	GLuint ProgramaShader; 

	char* CargarShaderAString ( string Direccion );

	GLint ComprobarErrores (GLuint shader);

public:

	Shader ();
	bool Cargar ( string Direccion, GLuint tipo ); 
	bool Link ();
	bool Cargar_Link (string DireccionVertex, string DireccionFragment);
	void ListarAtributos ();
	void ListarUniformes ();
	int LocacionUniforme (string nombre);
	int LocacionAtributo (string nombre);
	void CargarUniforme( int locacion, int count, float *value );
	void CargarMatrizUniforme( string Nombre, GLfloat *Matriz );
	void CargarTexturaUniforme ( GLuint Tex, string texname );
	void Habilitar ();
	void Deshabilitar ();
	GLuint getProgramShader();
};