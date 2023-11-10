#pragma once
#include <chrono>

class Time
{
private:
	float TotalTime;							//	time since start of the program
	float LastDeltaTime;						//	time at the end of the last frame

public:

	Time();

	float CurrentTime() const;					//	time since start of the program
	float DeltaTime() const;					//	time between frames
		
	void ResetTime();							//	reset time to zero again
	void SetTime(const float value);			//	set time to a new value

	void Tick();								//	update total time and lastdeltatime
};