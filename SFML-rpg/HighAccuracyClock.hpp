#pragma once
#include <chrono>

class HighAccuracyClock
{
public:
	enum class Resolution
	{
		Nano,
		Pico
	};
public:
	HighAccuracyClock();

	__int64 restart();
	__int64 getElapsedTime();

	void setResolution(const Resolution &res);

private:
	std::chrono::steady_clock m_clock;
	std::chrono::time_point<std::chrono::steady_clock> m_old;
	Resolution m_res = Resolution::Nano;
};