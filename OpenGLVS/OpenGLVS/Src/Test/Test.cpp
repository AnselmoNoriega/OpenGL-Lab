#include "Test.h"
#include "ImGui/imgui.h"

namespace test
{
	TestMenu::TestMenu(Test*& currntTest)
		: mCurrentTest(currntTest)
	{
	}

	TestMenu::~TestMenu() {}

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
}