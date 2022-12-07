#pragma once
#include <chrono>
#include <iostream>
#include <string>

namespace Eisen {
    class Timer {
    public:
        void start(std::string name);
        void display();

    private:
        std::chrono::steady_clock::time_point startTime;
        std::chrono::steady_clock::time_point curTime;
        std::string name;
    };
}  // namespace Eisen
