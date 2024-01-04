#pragma once
#include <glm/gtc/matrix_transform.hpp>

class Shader;
struct GLFWwindow;

class Camera
{
public:
	Camera(int width, int height, glm::vec3 position);
	
	void Matrix(float FOVdeg, float nearPlane, float farPlane, Shader& shader, const char* uniform);
	void Inputs(GLFWwindow* window);

private:
	glm::vec3 mPos;
	glm::vec3 mOrientation = glm::vec3(0.0f, 0.0f, -1.0f);

	int mWidth;
	int mHeight;

	float mSpeed = 0.1f;
	float mSensitivity = 100.0f;

	bool firstClick = true;
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
};

