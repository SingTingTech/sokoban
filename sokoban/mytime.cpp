#include"mytime.h"
void cris::Timer::start()
{
	QueryPerformanceCounter(&begin);
}
void cris::Timer::stop()
{
	QueryPerformanceCounter(&end);
	timepassed = ((double)end.QuadPart - (double)begin.QuadPart) / (double)freq.QuadPart;
}
