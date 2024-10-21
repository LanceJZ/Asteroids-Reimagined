#pragma once
class Timer
{
public:
	void Update(double deltaTime);
	void FixedUpdate(double deltaTime);

	void Set(float amountInSeconds);
	void Reset();
	void Reset(float amoutInSeconds);
	bool Elapsed();

	float GetAmount();
	float GetAmountElapsed();

private:
	double SecondsElapsed = 0.0f;
	float TimerAmount = 0.0f;
};