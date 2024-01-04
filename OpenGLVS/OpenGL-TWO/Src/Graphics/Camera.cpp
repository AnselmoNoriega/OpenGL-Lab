#include "Camera.h"
#include "Shader.h"
#include <glad/glad.h>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>

Camera::Camera(int width, int height, glm::vec3 position) :
	mWidth(width), mHeight(height), mPos(position)
{
}

void Camera::Matrix(float FOVdeg, float nearPlane, float farPlane, Shader& shader, const char* uniform)
{
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	view = glm::lookAt(mPos, mPos + mOrientation, up);
	projection = glm::perspective(glm::radians(FOVdeg), (float)(mWidth / mHeight), nearPlane, farPlane);

	int MVP_ID = glGetUniformLocation(shader.GetID(), uniform);
	glUniformMatrix4fv(MVP_ID, 1, GL_FALSE, glm::value_ptr(projection * view));
}

void Camera::Inputs(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		mPos += mSpeed * mOrientation;
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		mPos += mSpeed * (-glm::normalize(glm::cross(mOrientation, up)));
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		mPos += mSpeed * (-mOrientation);
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		mPos += mSpeed * glm::normalize(glm::cross(mOrientation, up));
	}

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		mPos += mSpeed * up;
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		mPos += mSpeed * (-up);
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		mSpeed = 0.4;
	}
	else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
	{
		mSpeed = 0.1;
	}

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		if (firstClick)
		{
			glfwSetCursorPos(window, (mWidth / 2), (mHeight / 2));
			firstClick = false;
		}

		double mousePosX;
		double mousePosY;
		glfwGetCursorPos(window, &mousePosX, &mousePosY);

		float rotX = mSensitivity * (float)(mousePosY - (mHeight / 2)) / mHeight;
		float rotY = mSensitivity * (float)(mousePosX - (mWidth / 2)) / mWidth;

		glm::vec3 newOrientation = glm::rotate(mOrientation, glm::radians(-rotX), glm::normalize(glm::cross(mOrientation, up)));
		if (!(glm::angle(newOrientation, up) <= glm::radians(5.0f) or glm::angle(newOrientation, -up) <= glm::radians(5.0f)))
		{
			mOrientation = newOrientation;
		}

		mOrientation = glm::rotate(mOrientation, glm::radians(-rotY), up);
		glfwSetCursorPos(window, (mWidth / 2), (mHeight / 2));
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		firstClick = true;
	}
}
