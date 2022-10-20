#pragma once
#include <chrono>
#include <string>

using namespace std;
namespace Eisen
{
	class Timer
	{
	public:
		void start(string name);
		void display();
	private:
		chrono::steady_clock::time_point startTime;
		chrono::steady_clock::time_point curTime;
		string name;
	};
}
