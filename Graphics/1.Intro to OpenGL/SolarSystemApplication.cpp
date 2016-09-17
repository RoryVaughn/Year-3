#include "SolarSystemApplication.h"
#include "gl_core_4_4.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "Camera.h"
#include <iostream>
#include <vector>

using glm::vec3;
using glm::vec4;
using glm::mat4;

const int points = 75;
//std::vector<unsigned int> GenerateIndices(int nm, int np)
//{
//	std::vector<unsigned int> indices;
//	//j=np-1
//	//      
//	//2     5   8   11  14  17
//	//1     4   7   10  13  16
//	//0     3   6   9   12  15      
//	//  
//	for (unsigned int i = 0; i < nm; i++) //nm = 4
//	{
//		unsigned int start = i * np;
//		for (int j = 0; j < np; j++) //np = 3
//		{
//			unsigned int botR = (start + np + j);
//			unsigned int botL = (start + j);
//			indices.push_back(botL);
//			indices.push_back(botR);
//		}
//		indices.push_back(0xFFFF);
//	} //we copied the origin whenever we rotated around nm + 1 times so we dont need to get the end again
//	return indices;
//}


SolarSystemApplication::SolarSystemApplication() 
	: m_camera(nullptr),
	m_direction(false) {

}

SolarSystemApplication::~SolarSystemApplication() {
	
}

bool SolarSystemApplication::generateGrid()
{
	int slices = 4;
	int r = 4.f;
	float theta;
	float phi;
	float oldX;
	float oldZ = 0;
	double newX;
	double newZ;
	float pi = glm::pi<float>();
	Vertex Vertices[points];
	unsigned int Indices[points];
	for (unsigned int i = 0; i < points; i++)
	{
		Indices[i] = i;
	}
	
	for (int a = 0; a < points; a++)
	{
		theta = (pi * a) / (points - 1);
		oldX = (sin(theta) * r);
		oldZ = 0;
		Vertices[a].position = vec4(oldX, cos(theta) * r, oldZ, 1);
		for (int b = 0; b < slices; b++)
		{
			phi = ((pi * b) / slices);
			newX = (oldX * (cos(phi))) + (oldZ * (sin(phi)));
			newZ = (oldZ * (cos(phi))) - (oldX * (sin(phi)));
			Vertices[a].position = vec4(newX, cos(theta) * r, newZ, 1);
		}
		oldX = newX;
		oldZ = newZ;
	}
	


	//CreateHalfCircle(3,5);
	//
	//Vertices[0].color = vec4(1, 0, 0, 0);
	//Vertices[1].color = vec4(0, 1, 0, 0);
	//Vertices[2].color = vec4(0, 0, 1, 0);
	//Vertices[3].color = vec4(0, 0, 0, 1);
	//Vertices[4].color = vec4(0, 0, 0, 1);
	/*unsigned int Indices[4] = { 0,2,1,3 };
	Vertices[0].position = vec4(-5, 0, -5, 1);
	Vertices[1].position = vec4(5, 0, -5, 1);
	Vertices[2].position = vec4(-5, 0, 5, 1);
	Vertices[3].position = vec4(5, 0, 5, 1);
	Vertices[0].color = vec4(1, 0, 0, 0);
	Vertices[1].color = vec4(0, 1, 0, 0);
	Vertices[2].color = vec4(0, 0, 1, 0);
	Vertices[3].color = vec4(0, 0, 0, 1);*/
	// create and bind buffers to a vertex array object
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_IBO);

	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, points * sizeof(Vertex), Vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, points * sizeof(unsigned int), Indices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	return true;
}
bool SolarSystemApplication::CreateShader()
{

	// create shader
	const char* vsSource = "#version 410\n \
                            layout(location=0) in vec4 Position; \
                            layout(location=1) in vec4 Colour; \
                            out vec4 vColour; \
                            uniform mat4 ProjectionViewWorldMatrix; \
                            void main() { vColour = Colour; \
                            gl_Position = ProjectionViewWorldMatrix * Position; }";
	const char* fsSource = "#version 410\n \
                            in vec4 vColour; \
                            out vec4 FragColor; \
                            void main() { FragColor = vColour; }";
	int success = GL_FALSE;
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(vertexShader, 1, (const char**)&vsSource, 0);
	glCompileShader(vertexShader);
	glShaderSource(fragmentShader, 1, (const char**)&fsSource, 0);
	glCompileShader(fragmentShader);
	m_programID = glCreateProgram();
	glAttachShader(m_programID, vertexShader);
	glAttachShader(m_programID, fragmentShader);
	glLinkProgram(m_programID);
	// check that it compiled and linked correctly
	glGetProgramiv(m_programID, GL_LINK_STATUS, &success);
	if (success == GL_FALSE) {
		int infoLogLength = 0;
		glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* infoLog = new char[infoLogLength + 1];
		glGetProgramInfoLog(m_programID, infoLogLength, 0, infoLog);
		printf("Error: Failed to link shader program!\n");
		printf("%s\n", infoLog);
		delete[] infoLog;
	}
	// we don't need to keep the individual shaders around
	glDeleteShader(fragmentShader);
	glDeleteShader(vertexShader);

	return true;
}
bool SolarSystemApplication::startup() {

	// create a basic window
	createWindow("AIE OpenGL Application", 1280, 720);

	// start the gizmo system that can draw basic immediate-mode shapes


	// setup camera
	m_camera = new Camera(glm::pi<float>() * 0.25f, 16 / 9.f, 0.1f, 1000.f);
	m_camera->setLookAtFrom(vec3(10, 10, 10), vec3(0));

	
	//
	generateGrid();
	CreateShader();
	// set input callbacksetInputCallback(inputCallback);
	return true;
}

void SolarSystemApplication::shutdown() {

	// cleanup camera and gizmos
	delete m_camera;


	// destroy our window properly
	destroyWindow();
}

bool SolarSystemApplication::update(float deltaTime) {
	
	// close the application if the window closes or we press escape
	if (glfwWindowShouldClose(m_window) ||
		glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		return false;

	// update the camera's movement
	m_camera->update(deltaTime);


	// return true, else the application closes
	return true;
}

void SolarSystemApplication::draw() {
	// clear the screen for this frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// bind shader
	glUseProgram(m_programID);
	// where to send the matrix
	int matUniform = glGetUniformLocation(m_programID, "ProjectionViewWorldMatrix");
	// send the matrix
	glUniformMatrix4fv(matUniform, 1, GL_FALSE, glm::value_ptr(m_camera->getProjectionView()));
	// draw quad
	glBindVertexArray(m_VAO);
	glPointSize((5.f));
	glDrawElements(GL_POINTS, points, GL_UNSIGNED_INT, (void*)0);
}

void SolarSystemApplication::inputCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {

	SolarSystemApplication* myThis = (SolarSystemApplication*)glfwGetWindowUserPointer(window);

	if (key == GLFW_KEY_SPACE)
		if (action == GLFW_PRESS)
			myThis->m_direction = true;
		else if (action == GLFW_RELEASE)
			myThis->m_direction = false;
}