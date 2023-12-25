#include "Test.h"
#include "ImGui/imgui.h"

namespace test
{
	TestMenu::TestMenu(Test*& currntTest)
		: mCurrentTest(currntTest)
	{
	}

	TestMenu::~TestMenu()
	{
		mCurrentTest = nullptr;
	}

	void TestMenu::OnImGuiRender()
	{
		for (auto& test : mTests)
		{
			if (ImGui::Button(test.first.c_str()))
			{
				mCurrentTest = test.second();
			}
		}
	}
	void TestMenu::Delete()
	{
		delete mCurrentTest;
	}
}