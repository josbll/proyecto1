#include "Shader.h"

Shader :: Shader () {

}

bool Shader::Cargar (string Direccion , GLuint tipo ){

	cout << "Carga del Shader: " << Direccion << endl;

	// Creacion de un shader
	GLuint shader = glCreateShader(tipo);

	if ( 0 == shader ){
		cout << "Error creando el shader (" << tipo << ")\n" << endl;
		return false;
	}

	// Carga en string del shader en un string
	const GLchar *shaderCode = CargarShaderAString (Direccion);
	if (shaderCode == NULL) return false;


	glShaderSource(shader, 1, &shaderCode , NULL );

	//Compilar el shader
	cout << "Compilando el Shader" << endl;
	glCompileShader( shader );

	if (ComprobarErrores(shader) == GL_FALSE ) return false;

	switch ( tipo ){
		case GL_VERTEX_SHADER:
			Vertex = shader;
			break;

		case GL_FRAGMENT_SHADER:
			Fragment = shader;
			break;
	}

	cout << "Finalizada la carga del Shader: " << Direccion << endl;
	return true;
}

char* Shader::CargarShaderAString (string Direccion){
	
	ifstream archivo;
	char *contenido = NULL;
	int tamano;
	
	archivo.open (Direccion);

	if ( archivo.good() ){
		
		//Se calcula la longitud del archivo
		archivo.seekg(0, ios::beg);//Establece el apuntador al texto al inicio del archivo
		archivo.seekg(0, ios::end);//Obtiene el apuntador al texto de la posicion indicada, en este caso declaro como inicio el final, y el desplazamiento nulo, obteniendo el apuntador al ultimo byte del archivo.
		tamano = (int)archivo.tellg();//obtengo la posicion del apuntador al texto actual, lo cual seria el tamano del archivo en bytes
		archivo.seekg(0, ios::beg);//Establece el apuntador al texto al inicio del archivo
		
		//Guardando el contenido del archivo en codigo
		contenido = new char [tamano+1];
		contenido[tamano] = '\0';
		archivo.read(contenido,tamano);//leo el archivo y lo guardo en contenido
	
		archivo.close();
	}else{
		printf("El archivo %s no se pudo abrir\n",Direccion);
	}
	
	return contenido;
}

GLint Shader::ComprobarErrores (GLuint Elemento) {
	GLint resultado;
	
	// Solicitud del estado de lacompilacion del elemento
	glGetShaderiv( Elemento, GL_COMPILE_STATUS, &resultado );

	if ( resultado == GL_FALSE ){
		GLint LogLength;
		glGetShaderiv ( Elemento, GL_INFO_LOG_LENGTH, &LogLength );
		if ( LogLength > 0 ){
			char * log = (char *)malloc(LogLength);
			GLsizei written;
			glGetShaderInfoLog(Elemento, LogLength, &written, log);
			cout << "Shader log:\n" << log << endl ;
			free(log);
		}
	}

	return resultado;
} 

bool Shader :: Link (){

	ProgramaShader = glCreateProgram();

	if ( ProgramaShader == 0 ){
		cout << "Error al crear el Programa \n";
		return false;
	}

	glAttachShader( ProgramaShader, Vertex );
	glAttachShader( ProgramaShader, Fragment );

	glLinkProgram( ProgramaShader );

	if ( ComprobarErrores(ProgramaShader) == GL_FALSE ) return false;

	return true;

}

bool Shader::Cargar_Link (string DireccionVertex , string DireccionFragment ) {
	// Carga del Vertex y Fragment Shader + Link
	return (Cargar(DireccionVertex, GL_VERTEX_SHADER) && Cargar (DireccionFragment, GL_FRAGMENT_SHADER) && Link());

}

void Shader :: ListarAtributos () {

	cout << "Lista de atributos activos" << endl;

	GLint maxLength, nAttribs;
	glGetProgramiv(ProgramaShader, GL_ACTIVE_ATTRIBUTES,&nAttribs);
	glGetProgramiv(ProgramaShader, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH,&maxLength);

	GLchar * name = new GLchar [maxLength+1];

	GLint written, size, location;
	GLenum type;
	cout<< " Index | Name\n";
	cout << "------------------------------------------------" << maxLength << endl;
	for( int i = 0; i < nAttribs; i++ ) {
		glGetActiveAttrib( ProgramaShader, i, maxLength, &written,&size, &type, name );
		location = glGetAttribLocation(ProgramaShader, name);
		printf(" %-5d | %s\n",location, name);
	}
	delete(name);

	cout << "------------------------------------------------\n";
	cout << "Fin de lista de atributos activos" << endl;
}

void Shader :: ListarUniformes (){
	char *name;
	GLint active_uniforms, max_length;
	GLint size;
	GLenum type;
  
	glGetProgramiv(ProgramaShader,GL_ACTIVE_UNIFORMS, &active_uniforms);
	glGetProgramiv(ProgramaShader,GL_ACTIVE_UNIFORM_MAX_LENGTH,&max_length);
  
	name = new char[max_length + 1];

	cout << "Lista de uniformes activos" << endl;
	cout<< " Index | Name\n";
	cout << "------------------------------------------------\n";

	for (int i = 0; i < active_uniforms; i++) {
 
		glGetActiveUniform(ProgramaShader, i, max_length + 1, NULL,&size, &type, name);
		printf(" %-5d | %s\n",glGetUniformLocation(ProgramaShader, name), name);
	}
	cout << "------------------------------------------------\n";
	delete name;
}

void Shader :: CargarMatrizUniforme( string Nombre, GLfloat *Matriz ){
	glUniformMatrix4fv(glGetUniformLocation(ProgramaShader,Nombre.c_str()),1,GL_FALSE,Matriz);
}

void Shader :: CargarTexturaUniforme ( GLuint Tex, string texname ){
	glActiveTexture(GL_TEXTURE0+Tex);
	glBindTexture(GL_TEXTURE_2D,Tex);
	glUniform1i(glGetUniformLocation(ProgramaShader,texname.c_str()),Tex);
}

void Shader::Habilitar(){
	glUseProgram(ProgramaShader);ComprobarErrores(ProgramaShader);
}

void Shader::Deshabilitar(){
	glUseProgram(0);
}

void Shader::CargarUniforme ( int locacion, int count, float *value ) {
	switch(count){
		case 1:
			glUniform1f( locacion, value[0] );
			break;
		case 2:
			glUniform2f( locacion, value[0], value[1] );
			break;
		case 3:
			glUniform3f( locacion, value[0], value[1], value[2] );
			break;
		case 4:
			glUniform4f( locacion, value[0], value[1], value[2], value[3] );
			break;
	}
	//glUniform3fv(locacion, 1, value);
}

int Shader::LocacionUniforme(string nombre){
	return glGetUniformLocation(ProgramaShader,nombre.c_str());
}

int Shader::LocacionAtributo (string nombre){
	return glGetAttribLocation (ProgramaShader,nombre.c_str());
}

GLuint Shader::getProgramShader(){
	return ProgramaShader;
}