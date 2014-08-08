#include "MeshLoader.h"

// <- Triangle ->

Triangle :: Triangle ( glm::vec3 *a, glm::vec3 *b, glm::vec3 *c ){
	vertex[0] = a;
	vertex[1] = b;
	vertex[2] = c;

	center = new glm::vec3(0.0f,0.0f,0.0f);
	center->x = ( a->x + b->x + c->x ) / 3.0f;
	center->y = ( a->y + b->y + c->y ) / 3.0f;
	center->z = ( a->z + b->z + c->z ) / 3.0f;

	normal = new glm::vec3(0.0f, 0.0f, 0.0f);

	// Normal en el centro del triangulo.
	*normal = glm::normalize ( glm::cross ( *vertex[1] - *center, *vertex[2] - *center ) );
}

// <- MeshLoader ->

Mesh :: Mesh (){  }

Mesh :: ~Mesh (){
	vertexList.clear();
	TriangleList.clear();
	this->UnsetDrawShader();
}

void Mesh :: LoadOFF (string direccion) {
	
	fstream archivo;
	archivo.open(direccion.c_str(),ios::in);	// Apertura del Archivo

	if (archivo.good()){	// Si el Archivo esta correcto.

		int nvertex, nface, nedges, i, j, nvf, a, b, c;
		string off;
		float x, y, z;

		
		archivo >> off;								// Lectura de la palabra OFF
		archivo >> nvertex >> nface >> nedges;		// Lectura de el numero de vertices, numero de caras y numero de aristas

		#pragma region [Lectura de vertices] [Calculo boundingbox]
			
			// Primer Vertice
			archivo >> x >> y >> z;
			vertexList.push_back(new glm::vec3(x,y,z) );
			MaxCorner.x=x;	MaxCorner.y=y;	MaxCorner.z=z; 
			MinCorner.x=x;	MinCorner.y=y;	MinCorner.z=z;

			// Resto de los Vertices
			for (i=1; i<nvertex; i++){
				archivo >> x >> y >> z;
				vertexList.push_back(new glm::vec3(x,y,z));
				
				MinCorner.x = min(MinCorner.x,x);
				MinCorner.y = min(MinCorner.y,y);
				MinCorner.z = min(MinCorner.z,z);

				MaxCorner.x = max(MaxCorner.x,x);
				MaxCorner.y = max(MaxCorner.y,y);
				MaxCorner.z = max(MaxCorner.z,z);

			}

		#pragma endregion [Lectura de vertices] [Calculo boundingbox]

		// Calculo del centro del modelo
		Center.x = (MinCorner.x + MaxCorner.x) / 2.0f;
		Center.y = (MinCorner.y + MaxCorner.y) / 2.0f;
		Center.z = (MinCorner.z + MaxCorner.z) / 2.0f;
		// Calculo de la escala maxima.
		MaxScale = max( max ( MaxCorner.x-MinCorner.x, MaxCorner.y-MinCorner.y ), MaxCorner.z-MinCorner.z); 

		vecScale = glm::vec3(1.0/MaxScale,1.0/MaxScale,1.0/MaxScale);

		#pragma region [Lectura de caras] [Calculo de aristas]
			string clave, simetrico;	
			
			// Lectura de las caras
			for (i=0; i<nface; i++){
				// Lectura de cantidad de vertices por cara y del primer vertice y segundo vertice.
				archivo >> nvf >> a >> b;
				
				// Division de la cara en triangulos
				for(j=2; j<nvf; j++){
					//Lectura de los demas vertices
					archivo >> c;

					TriangleList.push_back( new Triangle( vertexList[a], vertexList[b], vertexList[c] ) );

					b = c;
				}
				getline ( archivo, off );
			}
		
		#pragma endregion [Lectura de caras] [Calculo de aristas]

		archivo.close();
		
		for ( i = 0; i < TriangleList.size(); i++ ){
			triangles_values_list.push_back( (*TriangleList[i]->vertex[0]).x );
			triangles_values_list.push_back( (*TriangleList[i]->vertex[0]).y );
			triangles_values_list.push_back( (*TriangleList[i]->vertex[0]).z );
			triangles_values_list.push_back( (*TriangleList[i]->vertex[1]).x );
			triangles_values_list.push_back( (*TriangleList[i]->vertex[1]).y );
			triangles_values_list.push_back( (*TriangleList[i]->vertex[1]).z );
			triangles_values_list.push_back( (*TriangleList[i]->vertex[2]).x );
			triangles_values_list.push_back( (*TriangleList[i]->vertex[2]).y );
			triangles_values_list.push_back( (*TriangleList[i]->vertex[2]).z );

			for ( j = 0; j < 3; j++ ){
				trianglesnormal_values_list.push_back( (*TriangleList[i]->normal).x );
				trianglesnormal_values_list.push_back( (*TriangleList[i]->normal).y );
				trianglesnormal_values_list.push_back( (*TriangleList[i]->normal).z );
			}
		}
		
	}else{
		cout << "El archivo no se ha podido abrir.\n";
	}

}

void Mesh :: OpenGLModelTransformation (glm::vec3 Scale, glm::vec3 Traslation, float alfa, float beta){
	glTranslatef(Traslation.x,Traslation.y,Traslation.z);
	glRotatef(alfa,0.0f,1.0f,0.0f);
	glRotatef(beta,1.0f,0.0f,0.0f);
	glScalef(Scale.x/MaxScale,Scale.y/MaxScale,Scale.z/MaxScale);
	glTranslatef(-Center.x,-Center.y,-Center.z);
}

void Mesh :: OpenGLCPUDraw ( float *color ) {
	int i;

	for ( i = 0; i < (int)TriangleList.size(); i++ ){
		glBegin( GL_TRIANGLES );
		glColor3fv( color );
		glVertex3f( (*TriangleList[i]->vertex[0]).x, (*TriangleList[i]->vertex[0]).y, (*TriangleList[i]->vertex[0]).z );
		glVertex3f( (*TriangleList[i]->vertex[1]).x, (*TriangleList[i]->vertex[1]).y, (*TriangleList[i]->vertex[1]).z );
		glVertex3f( (*TriangleList[i]->vertex[2]).x, (*TriangleList[i]->vertex[2]).y, (*TriangleList[i]->vertex[2]).z );
		glEnd();

		glPointSize(2.0f);
		glBegin( GL_POINTS );
		glColor3f( 1.0f, 0.0f, 0.0f );
		glVertex3f( (*TriangleList[i]->center).x, (*TriangleList[i]->center).y, (*TriangleList[i]->center).z );
		glEnd();
		glPointSize(1.0f);
	}
	glEnd();
}

glm::mat4 Mesh :: GenerateModelMatrix ( float alfa, float beta, glm::vec3 escala, glm::vec3 trasla ){
	glm::mat4 Transformacion = glm::mat4x4 (1.0f);

	// Traslacion al centro y escalado
	Transformacion = glm::translate(glm::mat4(1.0f),Center) * Transformacion;
	Transformacion = glm::scale(glm::mat4(1.0f),glm::vec3(escala.x/MaxScale,escala.y/MaxScale,escala.z/MaxScale)) * Transformacion;

	// Rotaciones en los ejes
	Transformacion = glm::rotate(glm::mat4(1.0f),beta,glm::vec3(1.0f,0.0f,0.0f)) * Transformacion;
	Transformacion = glm::rotate(glm::mat4(1.0f),alfa,glm::vec3(0.0f,1.0f,0.0f)) * Transformacion;

	// Ponerlo detras de la vista.
	Transformacion = glm::translate(glm::mat4(1.0f),trasla) * Transformacion;

	return Transformacion;
}

float Mesh :: getMaxScale () {
	return MaxScale;
}

glm::vec3 Mesh :: getCenter (){
	return Center;
}

void Mesh :: setDrawShader ( GLuint ProgramShader ){

	/*
		Si el shader no posee identificacion de los layout de cada atributo entonces con
		glBindAttribLocation se encarga de definir los identificadores de cada atributo usando
		el nombre que poseen en el shader.

	glBindAttribLocation(ProgramShader, 0, "VertexPosition");
	glBindAttribLocation(ProgramShader, 1, "Normal");
	*/

	/*	glGenBuffers( GLsizei cant, GLuint  *buffers)
		se encarga de crear cant buffers y asigna los identificadores correspondientes en el arreglo buffers
		en este momento ya existen en la memoria del hardware grafico 2 buffers.
	*/
	glGenBuffers(2, vboHandles);

	/*	glBindBuffer( GLenum target, GLuint buffer );
		enlaza al buffer object que indica el identificador "buffer" de tal manera que las siguientes operaciones
		de buffers que sean especificadas van a afectar a ese buffer
	*/
	glBindBuffer(GL_ARRAY_BUFFER, vboHandles[0]);
	/*	void glBufferData( GLenum target, GLsizeiptr size, const GLvoid * data,	GLenum usage);
		crea un nuevo conjunto de datos para el buffer que se encuentra actualmente enlazado. Si existe alguna data
		almacenada esta es borrada. Los nuevos datos son almacenados con el tamaño (size) en bytes y con el uso (usage)
		especificado.	
	*/
	glBufferData(GL_ARRAY_BUFFER, triangles_values_list.size() * sizeof(float), triangles_values_list.data(), GL_STATIC_DRAW);
	/* el uso de glBindBuffer con parametro 0 hace que el ultimo buffer enlazado deje de estar enlazado*/
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, vboHandles[1]);
	glBufferData(GL_ARRAY_BUFFER, trianglesnormal_values_list.size() * sizeof(float), trianglesnormal_values_list.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/*	Gerneracion de un Vertex Array Object (VAO) que se encarga de encapsular toda la data que es necesaria para especificar los
		datos del vertice. 
		glGenVertexArrays( int n , GLuint *buf ): genera 'n' VAOs y coloca los identificadores en el arreglo 'buf' */
	glGenVertexArrays( 1, &vaoHandle );
	
	/* glBindVertexArray ( GLuint idVAO )	 enlaza el VAO 'idVAO' */
	glBindVertexArray(vaoHandle);
	/*	glEnableVertexAttribArray( GLuint array) habilita el arreglo de atributo 'array' especificado del VAO enlazado 
	actualmente	*/
	glEnableVertexAttribArray(0); // VertexPosition
	glEnableVertexAttribArray(1); // Normal

	glBindBuffer(GL_ARRAY_BUFFER, vboHandles[0]);
	/* glVertexAttribPointer( GLuint index​, GLint size​, GLenum type​, GLboolean normalized​, GLsizei stride​, const void *offset​):
	indica como se tomaran los datos del VBO que se encuentran enlazado para el atributo 'index'  */
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, vboHandles[1]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 0, (GLubyte*)NULL);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	// Desenlaza al VAO
	glBindVertexArray ( 0 );

	cout << "VBO 1 - " << vboHandles[0] << endl << "VBO 2 - " << vboHandles[1] << endl << "VAO - " << vaoHandle << endl ;

}

void Mesh :: OpenGLDrawShader ( ){
	/* Enlaza el VAO para ser usado*/
	glBindVertexArray(vaoHandle);
	// Despliega la primitiva definida comenzando de 0 y pinta la ultima cantidad de primitivas
	glDrawArrays(GL_TRIANGLES, 0, trianglesnormal_values_list.size()/3);
	glBindVertexArray(0);
}

void Mesh :: UnsetDrawShader (){
	/* Eliminacion de VAO y de los VBO*/
	glDeleteVertexArrays( 1 , &vaoHandle );
	glDeleteBuffers( 2 , vboHandles );
}

void Mesh::DrawBoundingBox(){

	
	glColor3f(1.0,1.0,1.0);
	glBegin(GL_LINE_LOOP);
		glColor3i( 255,0,255 );
		
		glVertex3f(MinCorner.x,MinCorner.y,MaxCorner.z);
		glVertex3f(MinCorner.x,MaxCorner.y,MaxCorner.z);//glVertex3f(min.x,max.y,max.z);
		glVertex3f(MaxCorner.x,MaxCorner.y,MaxCorner.z);			 //glVertex3f(max.x,max.y,max.z);
		glVertex3f(MaxCorner.x,MinCorner.y,MaxCorner.z);// glVertex3f(max.x,min.y,max.z);
	glEnd();

	glBegin(GL_LINE_LOOP);
		glColor3i( 255,255,255 );

		glVertex3f(MinCorner.x,MinCorner.y,MinCorner.z);
		glVertex3f(MinCorner.x,MaxCorner.y,MinCorner.z);
		glVertex3f(MaxCorner.x,MaxCorner.y,MinCorner.z); 
		glVertex3f(MaxCorner.x,MinCorner.y,MinCorner.z);
    glEnd();

	glBegin(GL_LINE_LOOP);
		glColor3i( 255,255,255 );

		glVertex3f(MinCorner.x,MaxCorner.y,MinCorner.z);
		glVertex3f(MinCorner.x,MinCorner.y,MinCorner.z);
		glVertex3f(MinCorner.x,MinCorner.y,MaxCorner.z);		
		glVertex3f(MinCorner.x,MaxCorner.y,MaxCorner.z);
	 glEnd();

	glBegin(GL_LINE_LOOP);
		glColor3i( 255,255,255 );

	    glVertex3f(MaxCorner.x,MaxCorner.y,MinCorner.z);
	    glVertex3f(MaxCorner.x,MinCorner.y,MinCorner.z);
		glVertex3f(MaxCorner.x,MinCorner.y,MaxCorner.z);
		glVertex3f(MaxCorner.x,MaxCorner.y,MaxCorner.z);
	glEnd();

}