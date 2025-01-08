#include "stdafx.h"
#include "HighAccuracyClock.hpp"

HighAccuracyClock::HighAccuracyClock()
{
	m_old = m_clock.now();
}

__int64 HighAccuracyClock::restart()
{
	auto cur = m_clock.now();

	__int64 elapsed = 0;

	switch (m_res)
	{
	case Resolution::Nano:
	{
		std::chrono::duration<__int64, std::nano> dur = cur - m_old;
		elapsed = dur.count();
	}
		break;
	case Resolution::Pico:
	{
		std::chrono::duration<__int64, std::pico> dur = cur - m_old;
		elapsed = dur.count();
	}
		break;
	}

	m_old = m_clock.now();

	return elapsed;
}

__int64 HighAccuracyClock::getElapsedTime()
{
	auto cur = m_clock.now();

	__int64 elapsed = 0;

	switch (m_res)
	{
	case Resolution::Nano:
	{
		std::chrono::duration<__int64, std::nano> dur = cur - m_old;
		elapsed = dur.count();
	}
		break;
	case Resolution::Pico:
	{
		std::chrono::duration<__int64, std::pico> dur = cur - m_old;
		elapsed = dur.count(); 
	}
		break;
	}

	return elapsed;
}

void HighAccuracyClock::setResolution(const Resolution &res)
{
}
