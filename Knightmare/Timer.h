#pragma once
class Timer
{
public:
	void Update(float deltaTime);

	void Set(float amountInSeconds);
	void Reset();
	void Reset(float amoutInSeconds);
	bool Elapsed();

private:
	float SecondsElapsed = 0.0f;
	float TimerAmount = 0.0f;
};

