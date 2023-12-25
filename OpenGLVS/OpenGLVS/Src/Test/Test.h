#pragma once
#include <map>
#include <functional>
#include <string>

namespace test
{
	class Test
	{
	public:
		Test() {};
		virtual ~Test() {};

		virtual void OnUpdate(float dt) {}
		virtual void OnRender() {}
		virtual void OnImGuiRender() {}
	};

	class TestMenu : public Test
	{
	public:
		TestMenu(Test*& currntTest);
		virtual ~TestMenu() override;

		void OnImGuiRender() override;

		template<typename T>
		void RegisterTest(const std::string& name)
		{
			mTests.emplace(name, []() {return new T(); });
		}

		void Delete();

	private:
		Test*& mCurrentTest;
		std::map<std::string, std::function<Test* ()>> mTests;
	};
}