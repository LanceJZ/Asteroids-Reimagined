#include "Timer.h"

void Timer::Update(float deltaTime)
{
    SecondsElapsed += deltaTime;
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
