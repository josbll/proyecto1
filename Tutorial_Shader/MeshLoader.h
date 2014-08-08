#pragma once
#include "GlobalHeader.h"
#include "Shader.h"

struct Triangle {
	glm::vec3 *vertex [3];
	glm::vec3 *center;
	glm::vec3 *normal;

	Triangle ( glm::vec3 *a, glm::vec3 *b, glm::vec3 *c );
};

class Mesh {
private:
	vector<glm::vec3*> vertexList;
	vector<Triangle*> TriangleList;
	vector<float> triangles_values_list;
	vector<float> trianglesnormal_values_list;
	
	glm::vec3 MaxCorner, MinCorner, Center, vecScale;
	float MaxScale;
	int xd,yd,zd;

	GLuint vaoHandle;
	GLuint vboHandles [2];

public:
	Mesh ();
	~Mesh();
	void LoadOFF (string);
	void OpenGLModelTransformation(glm::vec3 Scale, glm::vec3 Traslation, float alfa, float beta);
	void OpenGLCPUDraw ( float *Color );
	
	void OpenGLCPUDrawPoints ( float *Color );
	glm::mat4 GenerateModelMatrix ( float alfa, float beta, glm::vec3 escala, glm::vec3 trasla );
	void MeshRender ();

	float getMaxScale ();
	glm::vec3 getCenter ();

	void setTraslation(glm::vec3);
	void setRotation(glm::quat);
	void setScale(float *);

	void setDrawShader ( GLuint ProgramShader );
	void UnsetDrawShader ();
	void OpenGLDrawShader ();
	void DrawBoundingBox();


};