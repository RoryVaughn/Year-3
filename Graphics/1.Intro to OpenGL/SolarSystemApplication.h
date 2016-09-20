#pragma once

#include "BaseApplication.h"
#include <glm/vec4.hpp>
#include <vector>
//#include <glm/mat4x2.hpp>
//#include "gl_core_4_4.h"


// forward declared to reduce dependencies
class Camera;

class SolarSystemApplication : public BaseApplication {
public:
	
	SolarSystemApplication();
	virtual ~SolarSystemApplication();
	std::vector<glm::vec4> genSemiCircle(const int);
	bool generateGrid();
	bool CreateShader();
	virtual bool startup();
	virtual void shutdown();

	virtual bool update(float deltaTime);
	virtual void draw();

	static void inputCallback(GLFWwindow* window, int key, int scanline, int action, int mods);
	// our vertex and index buffers
	
private:
	struct Vertex {
		std::vector<glm::vec4> position;
		std::vector<glm::vec4> color;
	};
	Camera*		m_camera;
	
	// we can toggle the way the earth orbits the sun,
	// demonstating input callback
	bool		m_direction;
	unsigned int m_VAO;
	unsigned int m_VBO;
	unsigned int m_IBO;
	unsigned int m_programID;
};