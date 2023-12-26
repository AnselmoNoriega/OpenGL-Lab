#include "TestTexture2D.h"

#include "Renderer.h"
#include "ImGui/imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace test
{
	TestTexture2D::TestTexture2D() :
		mTranslationA(0, 0, 0), mTranslationB(500, 500, 0), 
		mProj(glm::ortho(0.0f, 1000.0f, 0.0f, 750.0f, -1.0f, 1.0f)), 
		mView(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)))
	{
		float vertsPos[] = {
			 -100.0f,  100.0f, 0.0f, 1.0f,
			  100.0f,  100.0f, 1.0f, 1.0f,
			  100.0f, -100.0f, 1.0f, 0.0f,
			 -100.0f, -100.0f, 0.0f, 0.0f,
		};

		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0
		};

		mVertexBuffer = std::make_unique<VertexBuffer>(vertsPos, 4 * 4 * sizeof(float));

		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		mVertexBufferLayout.Push<float>(2);
		mVertexBufferLayout.Push<float>(2);
		mVertexArray.AddBuffer(*mVertexBuffer, mVertexBufferLayout);

		mIndexBuffer = std::make_unique<IndexBuffer>(indices, 6);

		mShader = std::make_unique<Shader>("Res/Shaders/Basic.shader");
		mShader->Bind();

		mTexture = std::make_unique<Texture>("Res/Textures/Image1.jpg");
		mTexture->Bind();
		mShader->SetUniform1i("u_Texture", 0);
	}

	TestTexture2D::~TestTexture2D()
	{
		mRenderer.Clear();
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
	}

	void TestTexture2D::OnUpdate(float dt)
	{
	}

	void TestTexture2D::OnRender()
	{
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 0.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		mShader->Bind();

		glm::mat4 modelA = glm::translate(glm::mat4(1.0f), mTranslationA);
		glm::mat4 mvpA = mProj * mView * modelA;
		mShader->SetUniformMat4f("u_MVP", mvpA);
		mRenderer.Draw(mVertexArray, *mIndexBuffer, *mShader);

		glm::mat4 modelB = glm::translate(glm::mat4(1.0f), mTranslationB);
		glm::mat4 mvpB = mProj * mView * modelB;
		mShader->SetUniformMat4f("u_MVP", mvpB);
		mRenderer.Draw(mVertexArray, *mIndexBuffer, *mShader);
	}

	void TestTexture2D::OnImGuiRender()
	{
		ImGui::SliderFloat3("Translation A", &mTranslationA.x, 0.0f, 1000.0f);
		ImGui::SliderFloat3("Translation B", &mTranslationB.x, 0.0f, 1000.0f);

		ImGui::Text("Application average %.3f ms/frame (%.1f)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
}
