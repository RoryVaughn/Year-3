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


SolarSystemApplication::SolarSystemApplication() 
	: m_camera(nullptr),
	m_direction(false) {

}

SolarSystemApplication::~SolarSystemApplication() {
	
}

Vertex* SolarSystemApplication::genSemiCircle(const int points)
{
	m_semi = points / m_slices;
	Vertex* Vertices = new Vertex[m_semi];

	for (int Vert = 0; Vert < m_semi; ++Vert)
	{
		float theta =  (glm::pi<float>() * Vert) / (m_slices - 1);
		Vertices[Vert].position = vec4(r * sin(theta),r * cos(theta),0,1);
		Vertices[Vert].color = vec4(0,1,255,1);
	}
	return Vertices;
}

Vertex * SolarSystemApplication::latheSphere(Vertex* Circle,int meridians)
{
	Circle = new Vertex[m_points];
	for (int currentSlice = 0; currentSlice < meridians; ++currentSlice)
	{
		float theta = glm::pi<float>() * m_points / (m_slices - 1);
		float oldX = (sin(theta) * r);
		float oldZ = 0;
		float phi = ((glm::pi<float>() * 2 * currentSlice) / meridians);
		float newX = oldX * cos(phi) + oldZ * sin(phi);
		float newZ = oldZ * cos(phi) - oldX * sin(phi);
		for (int currentPoint = 0; currentPoint < m_points ; ++currentPoint)
		{
			Circle[currentPoint].position = vec4(newX, (cos(theta) * r), newZ, 1);
		}
		phi = ((glm::pi<float>() * 2 * currentSlice) / meridians);
		newX = oldX * cos(phi) + oldZ * sin(phi);
		newZ = oldZ * cos(phi) - oldX * sin(phi);
		oldX = newX;
		oldZ = newZ;
	}
	return Circle;
}

bool SolarSystemApplication::generateGrid()
{
	for (unsigned int i = 0; i < m_semi; i++)
		m_indices[i] = i;

	Vertex * Circle = new Vertex[m_points];
	Vertex * sphereVerts = new Vertex[m_points];
	Circle = genSemiCircle(m_points);
	//sphereVerts = latheSphere(Circle,m_slices);
	

		/*for (int sliceIndex = 0; sliceIndex < slices; sliceIndex++)
		{
			
			switch (sliceIndex)
			{
				case 0: v2SliceIndex = 20;
					break;
				case 1: v2SliceIndex = 40;
					break;
				case 2: v2SliceIndex = 60 * 3;
					break;
				case 3: v2SliceIndex = 80 * 3;
					break;
				case 4: v2SliceIndex = 100 * 3;
					break;
				case 5: v2SliceIndex = 120 * 3;
					break;
				case 6: v2SliceIndex = 140 * 3;
					break;
				case 7: v2SliceIndex = 160 * 3;
					break;
				case 8: v2SliceIndex = 180 * 3;
					break;
				case 9: v2SliceIndex = 200 * 3;
					break;
				case 10: v2SliceIndex = 220 * 3;
					break;
				case 11: v2SliceIndex = 240 * 3;
					break;
				default: printf("default case reached");
					break;
			}
			phi = ((pi * 2 * sliceIndex) / slices);
			newX = (oldX * (cos(phi))) + (oldZ * (sin(phi)));
			newZ = (oldZ * (cos(phi))) - (oldX * (sin(phi)));
			Vertices[firstSlice + v2SliceIndex].position = vec4(newX * 2, (cos(theta) * r) * 2, newZ * 2, 1);
		}
		oldX = newX;
		oldZ = newZ;*/	
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);
	// create and bind buffers to a vertex array object
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_IBO);



	//Buffer Vertexes
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER,  sizeof(Vertex) * m_points, Circle , GL_STATIC_DRAW);

	//Buffer indicies
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * m_points , m_indices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), ((char*)0) + 16);

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
	glDrawElements(GL_POINTS, m_points, GL_UNSIGNED_INT, (void*)0);
	//GL_POINTS
	//GL_TRIANGLE_STRIP
}

void SolarSystemApplication::inputCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {

	SolarSystemApplication* myThis = (SolarSystemApplication*)glfwGetWindowUserPointer(window);

	if (key == GLFW_KEY_SPACE)
		if (action == GLFW_PRESS)
			myThis->m_direction = true;
		else if (action == GLFW_RELEASE)
			myThis->m_direction = false;
}