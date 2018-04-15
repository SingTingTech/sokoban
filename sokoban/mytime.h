#pragma once
#include"stdafx.h"
namespace cris {
	class Timer
	{
		LARGE_INTEGER begin;
		LARGE_INTEGER end;
		LARGE_INTEGER freq;

	public:
		Timer() 
		{
			QueryPerformanceFrequency(&freq);
		}
		double timepassed;

		void start();

		void stop();

	};
}