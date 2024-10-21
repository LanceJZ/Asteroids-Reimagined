#include "Timer.h"

void Timer::Update(double deltaTime)
{
    SecondsElapsed += deltaTime;
}

void Timer::FixedUpdate(double deltaTime)
{
}

void Timer::Set(float amountInSeconds)
{
    TimerAmount = amountInSeconds;
}

void Timer::Reset()
{
    SecondsElapsed = 0;
}

void Timer::Reset(float amountInSeconds)
{
    Reset();
    Set(amountInSeconds);
}

bool Timer::Elapsed()
{
    return SecondsElapsed > TimerAmount;
}

float Timer::GetAmount()
{
    return TimerAmount;
}

float Timer::GetAmountElapsed()
{
    return SecondsElapsed;
}