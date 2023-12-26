#pragma once
#include "Test.h"

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "Texture.h"
#include "Shader.h"

#include "memory"

namespace test
{
	class TestTexture2D :public Test
	{
	public:
		TestTexture2D();
		~TestTexture2D() override;

		void OnUpdate(float dt) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		std::unique_ptr<VertexBuffer> mVertexBuffer;
		std::unique_ptr<IndexBuffer> mIndexBuffer;

		std::unique_ptr<Shader> mShader;
		std::unique_ptr<Texture> mTexture;

		VertexArray mVertexArray;
		VertexBufferLayout mVertexBufferLayout;

		Renderer mRenderer;

		glm::vec3 mTranslationA, mTranslationB;

		glm::mat4 mProj;
		glm::mat4 mView;
	};
}

