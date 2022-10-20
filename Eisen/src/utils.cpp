#include "utils.h"
#include <iostream>

using namespace std;
using namespace chrono;

namespace Eisen
{
	void Timer::start(string name)
	{
		this->name = name;
		startTime = high_resolution_clock::now();
	}
	void Timer::display()
	{
		curTime = high_resolution_clock::now();
		chrono::duration<float> dur = curTime - startTime;
		cout << name << " Completed in " << dur.count() << "s" << endl;
	}
}
