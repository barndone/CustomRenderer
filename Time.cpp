#include "Time.h"
#include "GLFW/glfw3.h"
#include <iostream>


Time::Time()
{
    ResetTime();
    LastDeltaTime = 0.0f;
}

float Time::CurrentTime() const
{
    return TotalTime;
}

float Time::DeltaTime() const
{
    return TotalTime - LastDeltaTime;
}

void Time::ResetTime()
{
    TotalTime = 0.0f;
}

void Time::SetTime(const float value)
{
    TotalTime = value;
}

void Time::Tick()
{
    LastDeltaTime = TotalTime;
    TotalTime = glfwGetTime();
}
