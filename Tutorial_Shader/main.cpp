#pragma once
#include "GlobalHeader.h"
#include "Shader.h"
#include "MeshLoader.h"
#include "Light.h"

Shader *ProgramShader,*PickingShader,*LightShader;

#pragma region [-- Configuracion de ventana --]
int Width = 825, Heigth = 600;
#pragma endregion

#pragma region [-- Configuracion de Camara --]
glm::vec4 Posicion = glm::vec4 (5.0f, 0.0f, 0.0f, 1.0f);
glm::mat4 ViewMatrix = glm::lookAt ( glm::vec3(0.0f, 10.0f, 5.0f),
									 glm::vec3(0.0f, 0.0f, 0.0f),
									 glm::vec3(0.0f, 1.0f, 0.0f));	
glm::mat4 ModelMatrix = glm::mat4(1.0f);
glm::mat4 ProjectionMatrix = glm::perspective(45.0f, (float)Width/(float)Heigth, 0.1f, 100.f);
glm::mat4 ModelView = ViewMatrix * ModelMatrix;
glm::mat4 NormalMatrix = glm::mat4 (1.0f);
#pragma endregion

#pragma region [-- Configuracion de Luces --]

vector<LightSourceParameters*> lights;
LightSourceParameters *g_light = new LightSourceParameters();
Mesh *lightmesh;
float shinnes = 20.0f;
/*
 *glm::vec3 ligthposition = glm::vec3(1.0f,1.0f,1.0f);
 *glm::vec3 Kd = glm::vec3 (0.8f,0.8f,0.8f);
 *glm::vec3 Ld = glm::vec3 (1.0f,1.0f,1.0f);
 */
#pragma endregion

#pragma region [-- Configuracion --]

int main_window;
string direccion_carga  = "../files/teapot.off";

float Translation [] = { 0.0f, 0.0f, 0.0f };
float Scale [] = { 1.0f, 1.0f, 1.0f };
float g_Zoom = 1.0f;
glm::quat g_Rotation = glm::quat();

Mesh* actualModel;

vector<Mesh*> Model;
vector<glm::vec3> translate;
vector<glm::quat> rotation;
vector<float> zoom;

#pragma endregion

void Reshape ( int w, int h ) {
	
	Width = w;
	Heigth = h;

	// Cambiar tamaño del viewport
	glViewport(0, 0, Width, Heigth); 

	ProjectionMatrix = glm::perspective(45.0f, (float)Width/(float)Heigth, 0.1f, 100.f);

	glMatrixMode ( GL_PROJECTION );
	glLoadIdentity();
	glLoadMatrixf( glm::value_ptr(glm::transpose( ProjectionMatrix )) );

	glMatrixMode ( GL_MODELVIEW );

	// Send the new window size to AntTweakBar
    TwWindowSize(w, h);

	cout<<w<<" "<<h<<endl;

}

void Display () {

	glClearColor ( 80.0f/255.0f, 209.0f/255.0f, 235.0f/255.0f, 1.0f );

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	float MaxScale = lightmesh->getMaxScale();
	glm::vec3 vecScale = glm::vec3 ( 0.2f/MaxScale, 0.2f/MaxScale, 0.2f/MaxScale );
	glm::vec3 tras = glm::vec3(g_light->position.x,g_light->position.y,g_light->position.z);
	glm::vec3 Center = lightmesh->getCenter();

	ModelMatrix = glm :: translate ( glm::mat4 (1.0f), Center );
	ModelMatrix = glm :: scale ( ModelMatrix, vecScale );
	ModelMatrix = glm :: translate ( ModelMatrix, tras );

	ModelView = ViewMatrix * ModelMatrix;
	NormalMatrix = glm::transpose( glm::inverse (ModelView) );

	LightShader->Habilitar();

	LightShader->CargarMatrizUniforme( "ModelView", glm::value_ptr(ModelView) );
	LightShader->CargarMatrizUniforme( "Projection", glm::value_ptr(ProjectionMatrix));
	LightShader->CargarMatrizUniforme( "NormalMatrix", glm::value_ptr(NormalMatrix));

	LightShader->CargarUniforme(LightShader->LocacionUniforme("Color"), 3, glm::value_ptr(g_light->ambient));

	lightmesh->OpenGLDrawShader();

	lightmesh->DrawBoundingBox();

	for (int i = 0; i < 4; i++)
	{
		MaxScale = lightmesh->getMaxScale();
		vecScale = glm::vec3 ( 0.2f/MaxScale, 0.2f/MaxScale, 0.2f/MaxScale );
		tras = glm::vec3(lights[i]->position.x,lights[i]->position.y,lights[i]->position.z);
		Center = lightmesh->getCenter();

		ModelMatrix = glm :: translate ( glm::mat4 (1.0f), Center );
		ModelMatrix = glm :: scale ( ModelMatrix, vecScale );
		ModelMatrix = glm :: translate ( ModelMatrix, tras );

		ModelView = ViewMatrix * ModelMatrix;
		NormalMatrix = glm::transpose( glm::inverse (ModelView) );

		LightShader->CargarMatrizUniforme( "ModelView", glm::value_ptr(ModelView) );
		LightShader->CargarMatrizUniforme( "Projection", glm::value_ptr(ProjectionMatrix));
		LightShader->CargarMatrizUniforme( "NormalMatrix", glm::value_ptr(NormalMatrix));

		LightShader->CargarUniforme(LightShader->LocacionUniforme("Color"), 3, glm::value_ptr(g_light->ambient));

		lightmesh->OpenGLDrawShader();

	}
	LightShader->Deshabilitar();

	if(actualModel!=NULL){

		float MaxScale = actualModel->getMaxScale();
		glm::vec3 vecScale = glm::vec3 ( g_Zoom/MaxScale, g_Zoom/MaxScale, g_Zoom/MaxScale );
		glm::vec3 tras = glm::vec3(Translation[0],Translation[1],Translation[2]);
		glm::vec3 Center = actualModel->getCenter();
		glm::mat4 rot = glm::mat4_cast(g_Rotation);

		//rot = glm::transpose(rot);

		ModelMatrix = glm :: translate ( glm::mat4 (1.0f), Center );
		ModelMatrix = rot * ModelMatrix;
		ModelMatrix = glm :: scale ( ModelMatrix, vecScale );
		ModelMatrix = glm :: translate ( ModelMatrix, tras );

		ModelView = ViewMatrix * ModelMatrix;
		
		glm::mat4 MVP = ProjectionMatrix * ModelView;

		NormalMatrix = glm::transpose( glm::inverse (ModelView) );

		ProgramShader->Habilitar();

		ProgramShader->CargarMatrizUniforme( "ModelView", glm::value_ptr(ModelView) );
		ProgramShader->CargarMatrizUniforme( "Projection", glm::value_ptr(ProjectionMatrix));
		ProgramShader->CargarMatrizUniforme( "NormalMatrix", glm::value_ptr(NormalMatrix));
		ProgramShader->CargarUniforme( ProgramShader->LocacionUniforme("shinnes"), 1, &shinnes );
		/*
		ProgramShader->CargarUniforme(ProgramShader->LocacionUniforme("lightposition"), 4, glm::value_ptr(ligthposition));
		ProgramShader->CargarUniforme(ProgramShader->LocacionUniforme("Kd"), 3, glm::value_ptr(Kd));
		ProgramShader->CargarUniforme(ProgramShader->LocacionUniforme("Ld"), 3, glm::value_ptr(Ld));

		ProgramShader->CargarUniforme( ProgramShader->LocacionUniforme("Color"), 3, Color );*/

		actualModel->OpenGLDrawShader();

		
		actualModel->DrawBoundingBox();

		ProgramShader->Deshabilitar();
	}

	if ( !Model.empty() ) {
		
		for(int i = 0; i<(int)Model.size(); i++){

			float MaxScale = Model[i]->getMaxScale();
			glm::vec3 vecScale = glm::vec3 ( zoom[i]/MaxScale, zoom[i]/MaxScale, zoom[i]/MaxScale );
			glm::vec3 tras = glm::vec3(translate[i][0],translate[i][1],translate[i][2]);
			glm::vec3 Center = Model[i]->getCenter();
			/*Center.x = -Center.x;
			Center.y = -Center.y;
			Center.z = -Center.z;
			*/
			glm::mat4 rot = glm::mat4_cast(rotation[i]);

			rot = glm::transpose(rot);

			ModelMatrix = glm :: translate ( glm::mat4 (1.0f), Center );
			ModelMatrix = rot * ModelMatrix;
			ModelMatrix = glm :: scale ( ModelMatrix, vecScale );
			ModelMatrix = glm :: translate ( ModelMatrix, tras );

			ModelView = ViewMatrix * ModelMatrix;
		
			glm::mat4 MVP = ProjectionMatrix * ModelView;

			NormalMatrix = glm::transpose( glm::inverse (ModelView) );

			ProgramShader->Habilitar();

			ProgramShader->CargarMatrizUniforme( "ModelView", glm::value_ptr(ModelView) );
			ProgramShader->CargarMatrizUniforme( "Projection", glm::value_ptr(ProjectionMatrix));
			ProgramShader->CargarMatrizUniforme( "MVP", glm::value_ptr(MVP));
			ProgramShader->CargarMatrizUniforme( "NormalMatrix", glm::value_ptr(NormalMatrix));
			/*
			ProgramShader->CargarUniforme(ProgramShader->LocacionUniforme("lightposition"), 4, glm::value_ptr(ligthposition));
			ProgramShader->CargarUniforme(ProgramShader->LocacionUniforme("Kd"), 3, glm::value_ptr(Kd));
			ProgramShader->CargarUniforme(ProgramShader->LocacionUniforme("Ld"), 3, glm::value_ptr(Ld));

			ProgramShader->CargarUniforme( ProgramShader->LocacionUniforme("Color"), 3, Color );*/

			Model[i]->OpenGLDrawShader();

			ProgramShader->Deshabilitar();
		}
	}
	// Draw tweak bars
    TwDraw();

	glutSwapBuffers();

	// Recall Display at next frame
    glutPostRedisplay();
}

void loadLights(Shader *s){
	/*
	float type = lights[0]->type;
	string dir = "lights[" + index+"].type";
	s->CargarUniforme( s->LocacionUniforme(dir), 1, &type );
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
	float quadraticAttenuation;// K2  */
}

void initShaders () {
	ProgramShader = new Shader ();
	ProgramShader->Cargar("../Shader/vs.vert", GL_VERTEX_SHADER);
	ProgramShader->Cargar("../Shader/fs.frag", GL_FRAGMENT_SHADER);
	ProgramShader->Link();

	PickingShader = new Shader();
	PickingShader->Cargar("../Shader/selection.vert", GL_VERTEX_SHADER);
	PickingShader->Cargar("../Shader/selection.frag", GL_FRAGMENT_SHADER);
	PickingShader->Link();

	LightShader = new Shader ();
	LightShader->Cargar("../Shader/lightVs.vert", GL_VERTEX_SHADER);
	LightShader->Cargar("../Shader/lightFs.frag", GL_FRAGMENT_SHADER);
	LightShader->Link();
}

#pragma region [-- Picking --]

void renderSelection(void) {

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// use the selection shader
	PickingShader->Habilitar();

	glm::vec3 code = glm::vec3(1.0f);
	if ( !Model.empty() ) {
		
		for(int i = 0; i<(int)Model.size(); i++, code.x++){

			float MaxScale = Model[i]->getMaxScale();
			glm::vec3 vecScale = glm::vec3 ( zoom[i]/MaxScale, zoom[i]/MaxScale, zoom[i]/MaxScale );
			glm::vec3 tras = glm::vec3(translate[i][0],translate[i][1],translate[i][2]);
			glm::vec3 Center = Model[i]->getCenter();

			glm::mat4 rot = glm::mat4_cast(rotation[i]);

			rot = glm::transpose(rot);

			ModelMatrix = glm :: translate ( glm::mat4 (1.0f), Center );
			ModelMatrix = rot * ModelMatrix;
			ModelMatrix = glm :: scale ( ModelMatrix, vecScale );
			ModelMatrix = glm :: translate ( ModelMatrix, tras );

			ModelView = ViewMatrix * ModelMatrix;

			PickingShader->CargarMatrizUniforme( "ModelView", glm::value_ptr(ModelView) );
			PickingShader->CargarMatrizUniforme( "Projection", glm::value_ptr(ProjectionMatrix));

			PickingShader->CargarUniforme(PickingShader->LocacionUniforme("code"), 3, glm::value_ptr(code));
			Model[i]->OpenGLDrawShader();
		}
	}

	for(int i = 0; i<4; i++, code.x++){

		float MaxScale = lightmesh->getMaxScale();
		glm::vec3 vecScale = glm::vec3 ( 0.2f/MaxScale, 0.2f/MaxScale, 0.2f/MaxScale );
		glm::vec3 tras = glm::vec3(lights[i]->position.x,lights[i]->position.y,lights[i]->position.z);
		glm::vec3 Center = lightmesh->getCenter();
		
		ModelMatrix = glm :: translate ( glm::mat4 (1.0f), Center );
		ModelMatrix = glm :: scale ( ModelMatrix, vecScale );
		ModelMatrix = glm :: translate ( ModelMatrix, tras );
		
		ModelView = ViewMatrix * ModelMatrix;
		
		PickingShader->CargarMatrizUniforme( "ModelView", glm::value_ptr(ModelView) );
		PickingShader->CargarMatrizUniforme( "Projection", glm::value_ptr(ProjectionMatrix));
		
		PickingShader->CargarUniforme(PickingShader->LocacionUniforme("code"), 3, glm::value_ptr(code));
		lightmesh->OpenGLDrawShader();
	}
	//don't swap buffers
	//glutSwapBuffers();

	// restore clear color
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	PickingShader->Deshabilitar();
}

void processSelection(int xx, int yy) {

	unsigned char res[4];
	GLint viewport[4]; 

	renderSelection();

	glGetIntegerv(GL_VIEWPORT, viewport);
	glReadPixels(xx, viewport[3] - yy, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &res);
	
	if(res[0]==0)
		return;

	if(!Model.empty() && res[0]<Model.size()){
		res[0]-=1;

		Model.push_back(actualModel);
		translate.push_back(glm::vec3(Translation[0],Translation[1],Translation[2]));
		rotation.push_back(g_Rotation);
		zoom.push_back(g_Zoom);

		actualModel = Model[res[0]];
		Translation[0]=translate[res[0]].x;
		Translation[1]=translate[res[0]].y;
		Translation[2]=translate[res[0]].z;
		g_Zoom = zoom[res[0]];
		g_Rotation = rotation[res[0]];
	
		Model.erase(Model.begin()+res[0]);
		translate.erase(translate.begin()+res[0]);
		rotation.erase(rotation.begin()+res[0]);
		zoom.erase(zoom.begin()+res[0]);

		return;
	}

	res[0]-=Model.size();

	lights.push_back(g_light);
	g_light = lights[res[0]];
	lights.erase(lights.begin()+res[0]);

	
}

void mouseClick(int button, int state, int x, int y){

	if(TwEventMouseButtonGLUT(button,state,x,y)) return;
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		processSelection(x, y);
   }

}

#pragma endregion

#pragma region [-- Callbaks AntTweakBar --]
// Function called to copy the content of a std::string (souceString) handled 
// by the AntTweakBar library to destinationClientString handled by our application
void TW_CALL CopyStdStringToClient(std::string& destinationClientString, const std::string& sourceLibraryString)
{
    destinationClientString = sourceLibraryString;
}

void TW_CALL Cargar(void *clientData)
{

	if(actualModel==NULL){
		const string *title = (const string *)(clientData);
		actualModel = new Mesh ();
		actualModel->LoadOFF( *title );
		actualModel->setDrawShader(ProgramShader->getProgramShader());
	}else
	{
		Model.push_back(actualModel);

		translate.push_back(glm::vec3(Translation [0],Translation [1],Translation [2]));
		rotation.push_back(g_Rotation);
		zoom.push_back(g_Zoom);

		const string *title = (const string *)(clientData);
		actualModel = new Mesh ();
		actualModel->LoadOFF( *title );
		actualModel->setDrawShader(ProgramShader->getProgramShader());

		Translation[0] = 0.0f;
		Translation[1] = 0.0f;
		Translation[2] = 0.0f;
		Scale [0] = 1.0f;
		Scale [1] = 1.0f;
		Scale [2] = 1.0f;
		g_Zoom = 1.0f;
		g_Rotation = glm::quat();
	}
}

#pragma endregion

void initLights(){

	srand (time(NULL));
	float a;

	g_light->position.x += rand() % 50 - 25;
	g_light->position.y += rand() % 50 - 25;
	g_light->position.z += rand() % 50 - 25;

	for (int i = 0; i < 4; i++)
	{
		lights.push_back( new LightSourceParameters());

		lights[i]->status = OFF;
		a = rand()%2;
		lights[i]->position.x += rand() % 50 - 25;
		lights[i]->position.y += rand() % 50 - 25;
		lights[i]->position.z += rand() % 50 - 25;

		
	}

	lightmesh = new Mesh ();
	lightmesh->LoadOFF( "../files/esfera.off" );
	lightmesh->setDrawShader(LightShader->getProgramShader());

}

int main ( int argc, char *argv[] ) {

	#pragma region [-- FreeGLUT --]
	
	glutInit (&argc, argv);
	glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
	glutInitWindowSize( Width, Heigth );
	main_window = glutCreateWindow ("Shaders");
	
	#pragma endregion

	#pragma region [-- GLEW --]
	
	// Inicializacion de GLEW
	GLenum glew_err = glewInit ();

	if ( GLEW_OK != glew_err ){
		cout << "No es posible inicializar GLEW\n";
		return 1;
	}

	

	cout << "Usando GLEW Version: " << glewGetString ( GLEW_VERSION ) << "\n\n";

	#pragma endregion

	initShaders();
	initLights();

	#pragma region [-- AntTweakBar --]
	TwBar *bar; // Pointer to the tweak bar
	TwBar *barLights;
	// Initialize AntTweakBar
    TwInit(TW_OPENGL, NULL);

	glutMouseFunc(mouseClick);

	// Set GLUT event callbacks
    // - Directly redirect GLUT mouse button events to AntTweakBar
    //glutMouseFunc((GLUTmousebuttonfun)TwEventMouseButtonGLUT);
    // - Directly redirect GLUT mouse motion events to AntTweakBar
    glutMotionFunc((GLUTmousemotionfun)TwEventMouseMotionGLUT);
    // - Directly redirect GLUT mouse "passive" motion events to AntTweakBar (same as MouseMotion)
    glutPassiveMotionFunc((GLUTmousemotionfun)TwEventMouseMotionGLUT);
    // - Directly redirect GLUT key events to AntTweakBar
    glutKeyboardFunc((GLUTkeyboardfun)TwEventKeyboardGLUT);
    // - Directly redirect GLUT special key events to AntTweakBar
    glutSpecialFunc((GLUTspecialfun)TwEventSpecialGLUT);
    // - Send 'glutGetModifers' function pointer to AntTweakBar;
    //   required because the GLUT key event functions do not report key modifiers states.
    TwGLUTModifiersFunc(glutGetModifiers);

	bar = TwNewBar("Objetos");
    TwDefine(" Objetos size='225 350' color='25 255 0' "); // change default tweak bar size and color 96 216 224

	// Define the required callback function to copy a std::string (see TwCopyStdStringToClientFunc documentation)
    TwCopyStdStringToClientFunc(CopyStdStringToClient);

	TwAddVarRW(bar, "Cargar figura", TW_TYPE_STDSTRING, &direccion_carga, 
               " label='direccion' group=Cargar help='Define a title for the new tweak bar.' ");

	// Add a button to create a new bar using the title
    TwAddButton(bar, "NewBarCreate", Cargar, &direccion_carga, 
                " label='--> Create' group=Cargar key=c help='Create a new tweak bar.' ");

	TwAddVarRW(bar, "ObjRotation1", TW_TYPE_QUAT4F, &g_Rotation, 
               " label='Object rotation' opened=true help='Change the object orientation.' ");

	// Add 'g_Zoom' to 'bar': this is a modifable (RW) variable of type TW_TYPE_FLOAT. Its key shortcuts are [m] and [M].
    TwAddVarRW(bar, "Zoom", TW_TYPE_FLOAT, &g_Zoom, 
               " min=0.01 max=2.5 step=0.01 keyIncr=m keyDecr=M help='Scale the object (1=original size).' ");
	// Add 'g_Zoom' to 'bar': this is a modifable (RW) variable of type TW_TYPE_FLOAT. Its key shortcuts are [m] and [M].
    TwAddVarRW(bar, "Shinnes", TW_TYPE_FLOAT, &shinnes, 
               " min=0.00 max=200 step=0.1 keyIncr=m keyDecr=M help='Scale the object (1=original size).' ");

	// Add 'Translate' to 'bar': this is a modifable (RW) variable of type TW_TYPE_FLOAT. Its key shortcuts are [xyz] and [xyz].
    TwAddVarRW(bar, "TranslateX", TW_TYPE_FLOAT, &Translation[0], 
               " min=-20.0 max=20 step=0.01 keyIncr=x keyDecr=X help='Scale the object (1=original size).' group='Translation' ");
	TwAddVarRW(bar, "TranslateY", TW_TYPE_FLOAT, &Translation[1], 
               " min=-20.0 max=20 step=0.01 keyIncr=y keyDecr=Y help='Scale the object (1=original size).' group='Translation' ");
	TwAddVarRW(bar, "TranslateZ", TW_TYPE_FLOAT, &Translation[2], 
               " min=-20.0 max=20 step=0.01 keyIncr=z keyDecr=Z help='Scale the object (1=original size).' group='Translation' ");

	barLights = TwNewBar("Luces");
    TwDefine(" Luces size='225 350' position='580 16' color='25 255 0' "); // change default tweak bar size and color 96 216 224

	{
        // ShapeEV associates Shape enum values with labels that will be displayed instead of enum values
        TwEnumVal lightType[3] = { {DIRECTIONALLIGHT, "Direccional"}, {SPOTLIGHT, "Reflector"}, {POINTLIGHT, "Puntual"} };
        // Create a type for the enum lightType
        TwType shapeType = TwDefineEnum("lightType", lightType, 3);
        // add 'g_CurrentShape' to 'bar': this is a variable of type ShapeType. Its key shortcuts are [<] and [>].
		TwAddVarRW(barLights, "Shape", shapeType, &g_light->status, " keyIncr='<' keyDecr='>' help='Change object shape.' ");
    }

	// Add 'g_LightDirection' to 'bar': this is a variable of type TW_TYPE_DIR3F which defines the light direction
	TwAddVarRW(barLights, "LightDir", TW_TYPE_DIR3F, &g_light->spotDirection, 
               " label='Light direction' opened=true help='Change the light direction.' ");

	// Add 'g_MatAmbient' to 'bar': this is a variable of type TW_TYPE_COLOR3F (3 floats color, alpha is ignored)
    // and is inserted into a group named 'Material'.
	TwAddVarRW(barLights, "Ambient", TW_TYPE_COLOR3F, &g_light->ambient, " group='Material' ");

    // Add 'g_MatDiffuse' to 'bar': this is a variable of type TW_TYPE_COLOR3F (3 floats color, alpha is ignored)
    // and is inserted into group 'Material'.
	TwAddVarRW(barLights, "Diffuse", TW_TYPE_COLOR3F, &g_light->diffuse, " group='Material' ");

	// Add 'g_MatDiffuse' to 'bar': this is a variable of type TW_TYPE_COLOR3F (3 floats color, alpha is ignored)
    // and is inserted into group 'Material'.
	TwAddVarRW(barLights, "specular", TW_TYPE_COLOR3F, &g_light->specular, " group='Material' ");

	TwAddVarRW(barLights, "TranslateX", TW_TYPE_FLOAT, &g_light->position.x, 
               " min=-20.0 max=20 step=0.01 keyIncr=x keyDecr=X help='Scale the object (1=original size).' group='Translation' ");
	TwAddVarRW(barLights, "Translatey", TW_TYPE_FLOAT, &g_light->position.y, 
               " min=-20.0 max=20 step=0.01 keyIncr=y keyDecr=Y help='Scale the object (1=original size).' group='Translation' ");
	TwAddVarRW(barLights, "Translatez", TW_TYPE_FLOAT, &g_light->position.z, 
               " min=-20.0 max=20 step=0.01 keyIncr=z keyDecr=Z help='Scale the object (1=original size).' group='Translation' ");

	TwAddVarRW(barLights, "spotExponent", TW_TYPE_FLOAT, &g_light->spotExponent, 
               " min=0.0 max=10 step=1.0 help='Scale the object (1=original size).' ");
	TwAddVarRW(barLights, "spotCutoff", TW_TYPE_FLOAT, &g_light->spotCutoff, 
               " min=0.0 max=60 step=1.0 help='Scale the object (1=original size).' ");
	TwAddVarRW(barLights, "constantAttenuation", TW_TYPE_FLOAT, &g_light->constantAttenuation, 
               " min=0.0  step=1.0 help='Scale the object (1=original size).' ");
	TwAddVarRW(barLights, "linearAttenuation", TW_TYPE_FLOAT, &g_light->linearAttenuation, 
               " min=0.0  step=1.0 help='Scale the object (1=original size).' ");
	TwAddVarRW(barLights, "quadraticAttenuation", TW_TYPE_FLOAT, &g_light->quadraticAttenuation, 
               " min=0.0 max=60 step=1.0 help='Scale the object (1=original size).' ");

	#pragma endregion

	#pragma region [-- Callbacks --]

	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);

	#pragma endregion
	
	glEnable(GL_DEPTH_TEST);

	
	
	glutMainLoop ();

	return 0;
}