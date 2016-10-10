#include "Geometry.h"
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


Geometry::Geometry() 
	: m_camera(nullptr),
	m_direction(false) {

}

Geometry::~Geometry() {
	
}

Vertex* Geometry::genSemiCircle(const int points)
{
	m_semi = points / m_slices;
	Vertex* Vertices = new Vertex[m_points];

	for (int Vert = 0; Vert <= m_semi; ++Vert)
	{
		float theta =  Vert * (glm::pi<float>() / (m_semi-1));
		Vertices[Vert].position = vec4(r * sin(theta), r * cos(theta),0,1);
		Vertices[Vert].color = vec4(0,1,255,1);
	}
	return Vertices;
}

Vertex * Geometry::latheSphere(Vertex* Sem,int meridians)
{
	m_semi = m_points / m_slices;
	Vertex * Circle = new Vertex[m_points * meridians];
	int index = 0;
	for (int currentSlice = 0; currentSlice < meridians; ++currentSlice)
	{
	
		float phi = glm::pi<float>() * 2 * ((float)currentSlice / (float)meridians);

		
		for (int currentPoint = 0; currentPoint < m_semi; ++currentPoint)
		{
			float x = (Sem[currentPoint].position.x * cos(phi)) + (Sem[currentPoint].position.z * sin(phi));
			float y = Sem[currentPoint].position.y;
			float z = (Sem[currentPoint].position.z * cos(phi)) - (Sem[currentPoint].position.x * sin(phi));
			Circle[index].position = vec4(x,y,z,1);
			Circle[index].color = vec4(2 * x, 2 * y, 2 * z, 1);
			index++;
		}
	}
	return Circle;
}

bool Geometry::generateSphere()
{
	for (unsigned int i = 0; i < m_points; i++)
		m_indices[i] = i;

	Vertex * halfCircle = new Vertex[m_points];
	Vertex * Sphere = new Vertex[m_points * 10];
	halfCircle = genSemiCircle(m_points);
	Sphere = latheSphere(halfCircle,m_slices);
	

	
	
	glGenVertexArrays(1, &m_sVAO);
	glBindVertexArray(m_sVAO);
	// create and bind buffers to a vertex array object
	glGenBuffers(1, &m_sVBO);
	glGenBuffers(1, &m_sIBO);



	//Buffer Vertexes
	glBindBuffer(GL_ARRAY_BUFFER, m_sVBO);
	glBufferData(GL_ARRAY_BUFFER,  sizeof(Vertex) * m_points, Sphere , GL_STATIC_DRAW);

	//Buffer indicies
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_sIBO);
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
bool Geometry::generateCube()
{	
	Vertex c_vertices[8];
	unsigned int c_indices[13] = { 0,1,2,3,2,1,4,6,5,7,2,4,6 };
	int dims = 5; 
	//Set the positions
	c_vertices[0].position = vec4(-dims, 0, -dims - (dims * 2.5), 1);
	c_vertices[1].position = vec4(dims, 0, -dims - (dims * 2.5), 1);
	c_vertices[2].position = vec4(-dims, 0, dims - (dims * 2.5), 1);
	c_vertices[3].position = vec4(dims, 0, dims - (dims * 2.5), 1);
	c_vertices[4].position = vec4(-dims, dims, dims - (dims * 2.5), 1);
	c_vertices[5].position = vec4(dims, dims, dims - (dims * 2.5), 1);
	c_vertices[6].position = vec4(dims, dims, -dims - (dims * 2.5), 1);
	c_vertices[7].position = vec4(-dims, dims, -dims - (dims * 2.5), 1);
	c_vertices[0].color = vec4(0, 0, 0, 1);
	c_vertices[1].color = vec4(0, 0, 1, 1);
	c_vertices[2].color = vec4(0, 0, 1, 1);
	c_vertices[3].color = vec4(0, 0, 1, 1);
	c_vertices[4].color = vec4(0, 0, 1, 1);
	c_vertices[5].color = vec4(0, 0, 1, 1);
	c_vertices[6].color = vec4(0, 0, 1, 1);
	c_vertices[7].color = vec4(0, 0, 1, 1);

	glGenVertexArrays(1, &m_cVAO);
	glBindVertexArray(m_cVAO);
	// create and bind buffers to a vertex array object
	glGenBuffers(1, &m_cVBO);
	glGenBuffers(1, &m_cIBO);



	//Buffer Vertexes
	glBindBuffer(GL_ARRAY_BUFFER, m_cVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 8 , c_vertices, GL_STATIC_DRAW);

	//Buffer indicies
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_cIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 13, c_indices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), ((char*)0) + 16);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	return true;
}
bool Geometry::generateGrid()
{
	Vertex vertices[4];
	unsigned int indices[6] = { 3,2,1,2,1,0 };
	int dims = 5;


	//Set the positions
	vertices[0].position = vec4(dims, -dims *1.5, -dims, 1);
	vertices[1].position = vec4(-dims, -dims *1.5, -dims, 1);
	vertices[2].position = vec4(dims, -dims *1.5, dims, 1);
	vertices[3].position = vec4(-dims, -dims *1.5, dims, 1);

	vertices[0].color = vec4(1, 0, 0, 1);
	vertices[1].color = vec4(0, 1, 0, 1);
	vertices[2].color = vec4(0, 0, 1, 1);
	vertices[3].color = vec4(1, 0, 0, 1);

	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);
	// create and bind buffers to a vertex array object
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_IBO);



	//Buffer Vertexes
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 4, vertices, GL_STATIC_DRAW);

	//Buffer indicies
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 6, indices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), ((char*)0) + 16);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	return true;
}
bool Geometry::CreateShader()
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
bool Geometry::startup() {

	// create a basic window
	createWindow("AIE OpenGL Application", 1280, 720);

	// start the gizmo system that can draw basic immediate-mode shapes


	// setup camera
	m_camera = new Camera(glm::pi<float>() * 0.25f, 16 / 9.f, 0.1f, 1000.f);
	m_camera->setLookAtFrom(vec3(10, 10, 10), vec3(0));

	
	generateSphere();
	generateCube();
	generateGrid();
	
	CreateShader();
	// set input callbacksetInputCallback(inputCallback);
	return true;
}

void Geometry::shutdown() {

	// cleanup camera and gizmos
	delete m_camera;


	// destroy our window properly
	destroyWindow();
}

bool Geometry::update(float deltaTime) {
	
	// close the application if the window closes or we press escape
	if (glfwWindowShouldClose(m_window) ||
		glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		return false;

	// update the camera's movement
	m_camera->update(deltaTime);


	// return true, else the application closes
	return true;
}

void Geometry::draw() {
	// clear the screen for this frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// bind shader
	glUseProgram(m_programID);
	// where to send the matrix
	int matUniform = glGetUniformLocation(m_programID, "ProjectionViewWorldMatrix");
	// send the matrix
	glUniformMatrix4fv(matUniform, 1, GL_FALSE, glm::value_ptr(m_camera->getProjectionView()));
	// draw quad
	glBindVertexArray(m_sVAO);
	glPointSize((5.f));
	glDrawElements(GL_POINTS, 81 * 6, GL_UNSIGNED_INT, (void*)0);

	glBindVertexArray(m_cVAO);
	glPointSize((5.f));
	glDrawElements(GL_TRIANGLE_STRIP, 13, GL_UNSIGNED_INT, (void*)0);

	glBindVertexArray(m_VAO);
	glPointSize((5.f));
	glDrawElements(GL_TRIANGLE_STRIP, 6, GL_UNSIGNED_INT, (void*)0);

	//GL_POINTS
	//GL_TRIANGLE_STRIP
}

void Geometry::inputCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {

	Geometry* myThis = (Geometry*)glfwGetWindowUserPointer(window);

	if (key == GLFW_KEY_SPACE)
		if (action == GLFW_PRESS)
			myThis->m_direction = true;
		else if (action == GLFW_RELEASE)
			myThis->m_direction = false;
}