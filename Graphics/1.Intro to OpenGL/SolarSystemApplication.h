#pragma once

#include "BaseApplication.h"
#include <glm/mat4x2.hpp>
#include "gl_core_4_4.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "Camera.h"
#include "Gizmos.h"
// forward declared to reduce dependencies
class Camera;

class SolarSystemApplication : public BaseApplication {
public:

	SolarSystemApplication();
	virtual ~SolarSystemApplication();

	virtual bool startup();
	virtual void shutdown();

	virtual bool update(float deltaTime);
	virtual void draw();

	static void inputCallback(GLFWwindow* window, int key, int scanline, int action, int mods);
	glm::mat4 m_projectionViewMatrix;
private:

	Camera*		m_camera;
	
	// we can toggle the way the earth orbits the sun,
	// demonstating input callback
	bool		m_direction;
};