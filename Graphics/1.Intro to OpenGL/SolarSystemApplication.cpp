#include "SolarSystemApplication.h"
#include "gl_core_4_4.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "Camera.h"
#include "Gizmos.h"

using glm::vec3;
using glm::vec4;
using glm::mat4;

struct Vertex {
	vec4 position;
	vec4 color;
};
// our vertex and index buffers
unsigned int m_VAO;
unsigned int m_VBO;
unsigned int m_IBO;unsigned int m_programID;
int generateGrid()
{
	Vertex vertices[4];
		unsigned int indices[4] = { 0,1,2,3 };

		vertices[0].position = vec4(-5, 0, -5, 1);
		vertices[1].position = vec4(5, 0, -5, 1);
		vertices[2].position = vec4(-5, 0, 5, 1);
		vertices[3].position = vec4(5, 0, 5, 1);

		vertices[0].color = vec4(1, 0, 0, 1);
		vertices[1].color = vec4(0, 1, 0, 1);
		vertices[2].color = vec4(0, 0, 1, 1);
		vertices[3].color = vec4(1, 1, 1, 1);

	

	// create and bind buffers to a vertex array object
	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex),
	vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
	(void*)(sizeof(vec4)));

	glBindBuffer(GL_ARRAY_BUFFER, 0);


		glGenBuffers(1, &m_IBO);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
			sizeof(unsigned int), indices, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		// Generate our GL Buffers
		// Lets move these so that they are all generated together
		glGenBuffers(1, &m_VBO);
		glGenBuffers(1, &m_IBO);
		//Add the following line to generate a VertexArrayObject
		glGenVertexArrays(1, &m_VAO);

		glBindVertexArray(m_VAO);
		// ....Code Segment here to bind and fill VBO + IBO
		glBindVertexArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	return 0;
}
int CreateShader()
{

	// create shaders
		const char* vsSource = "#version 410\n \
	layout(location=0) in vec4 position; \
	layout(location=1) in vec4 colour; \
	out vec4 vColour; \
	uniform mat4 projectionViewWorldMatrix; \
	void main() { vColour = colour; gl_Position = projectionViewWorldMatrix * position;}";

	const char* fsSource = "#version 410\n \
	in vec4 vColour; \
	out vec4 fragColor; \
	void main() { fragColor = vColour; }";

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

	glGetProgramiv(m_programID, GL_LINK_STATUS, &success);
	if (success == GL_FALSE) {
		int infoLogLength = 0;
		glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* infoLog = new char[infoLogLength];

		glGetProgramInfoLog(m_programID, infoLogLength, 0, infoLog);
		printf("Error: Failed to link shader program!\n");
		printf("%s\n", infoLog);
		delete[] infoLog;
	}
	glDeleteShader(fragmentShader);
	glDeleteShader(vertexShader);


	// create shaders
	vsSource = "#version 410\n \
	layout(location=0) in vec4 position; \
	layout(location=1) in vec4 colour; \
	out vec4 vColour; \
	uniform mat4 projectionViewWorldMatrix; \
	void main() { vColour = Colour; gl_Position = projectionViewWorldMatrix * position;}";
	fsSource = "#version 410\n \
	in vec4 vColour; \
	out vec4 fragColor; \
	void main() { fragColor = vColour; }";
	/*success = GL_FALSE;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vertexShader, 1, (const char**)&vsSource, 0);
	glCompileShader(vertexShader);

	glUseProgram(m_programID);
	unsigned int projectionViewUniform =
		glGetUniformLocation(m_programID, "projectionViewWorldMatrix");
	glUniformMatrix4fv(projectionViewUniform, 1, false,
		glm::value_ptr(m_projectionViewMatrix));

	glBindVertexArray(m_VAO);
	unsigned int indexCount = 6;
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);*/

	return 0;
}
SolarSystemApplication::SolarSystemApplication() 
	: m_camera(nullptr),
	m_direction(false) {

}

SolarSystemApplication::~SolarSystemApplication() {
	
}

bool SolarSystemApplication::startup() {

	// create a basic window
	createWindow("AIE OpenGL Application", 1280, 720);

	// start the gizmo system that can draw basic immediate-mode shapes
	Gizmos::create();

	// setup camera
	m_camera = new Camera(glm::pi<float>() * 0.25f, 16 / 9.f, 0.1f, 1000.f);
	m_camera->setLookAtFrom(vec3(10, 10, 10), vec3(0));

	// set input callback
	setInputCallback(inputCallback);
	generateGrid();	CreateShader();
	return true;
}

void SolarSystemApplication::shutdown() {

	// cleanup camera and gizmos
	delete m_camera;
	Gizmos::destroy();

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

	// clear the gizmos out for this frame
	Gizmos::clear();

	vec4 white(1, 1, 1, 0);
	vec4 black(0, 0, 0, 1);


	// for now let's add a grid to the gizmos
	
	

	
	// return true, else the application closes
	return true;
}

void SolarSystemApplication::draw() {
	// clear the screen for this frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// display the 3D gizmos
	Gizmos::draw(m_camera->getProjectionView());

	// get a orthographic projection matrix and draw 2D gizmos
	int width = 0, height = 0;
	glfwGetWindowSize(m_window, &width, &height);
	mat4 guiMatrix = glm::ortho<float>(0, 0, (float)width, (float)height);

	Gizmos::draw2D(guiMatrix);
}

void SolarSystemApplication::inputCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {

	SolarSystemApplication* myThis = (SolarSystemApplication*)glfwGetWindowUserPointer(window);

	if (key == GLFW_KEY_SPACE)
		if (action == GLFW_PRESS)
			myThis->m_direction = true;
		else if (action == GLFW_RELEASE)
			myThis->m_direction = false;
}