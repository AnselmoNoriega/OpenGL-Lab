#include "TestClearColor.h"

#include "Renderer.h"
#include "ImGui/imgui.h"

namespace test
{
	TestClearColor::TestClearColor() :
		mClearColor{ 0.5f, 0.2f, 0.8f, 1.0f }
	{
	}

	TestClearColor::~TestClearColor()
	{
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
	}

	void TestClearColor::OnUpdate(float dt)
	{
	}

	void TestClearColor::OnRender()
	{
		GLCall(glClearColor(mClearColor[0], mClearColor[1], mClearColor[2], mClearColor[3]));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));
	}

	void TestClearColor::OnImGuiRender()
	{
		ImGui::ColorEdit4("Clear Color", mClearColor);
	}
}
