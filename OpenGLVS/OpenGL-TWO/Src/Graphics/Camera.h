#pragma once
#include <glm/gtc/matrix_transform.hpp>

class Shader;
struct GLFWwindow;

class Camera
{
public:
	Camera(int width, int height, glm::vec3 position);
	
	void Update(Shader& shader, const char* uniform);
	void Inputs(GLFWwindow* window);

	void SetMatrix(float FOVdeg, float nearPlane, float farPlane);

	glm::vec3 GetPos();

private:
	glm::vec3 mPos;
	glm::vec3 mOrientation = glm::vec3(0.0f, 0.0f, -1.0f);

	glm::mat4 mView;
	glm::mat4 mProjection;


	int mWidth;
	int mHeight;

	float mSpeed = 0.1f;
	float mSensitivity = 100.0f;

	bool firstClick = true;
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
};

