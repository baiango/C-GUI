#pragma once
#ifdef _WIN32
	#include <windows.h>
	typedef LARGE_INTEGER timeint64_t;
#else
	#include <sys/time.h>
	typedef struct timeval timeint64_t;
#endif


timeint64_t get_high_res_time()
{	timeint64_t time;
	#ifdef _WIN32
	QueryPerformanceCounter(&time);
	#else
	gettimeofday(&time, NULL);
	#endif
	return time; }

double get_elapsed_time(timeint64_t start, timeint64_t end)
{
	#ifdef _WIN32
	LARGE_INTEGER frequency;
	QueryPerformanceFrequency(&frequency);
	return (end.QuadPart - start.QuadPart)
	    / frequency.QuadPart;
	#else
	return (end.tv_sec - start.tv_sec) +
	    (end.tv_usec - start.tv_usec) / 1000000.0;
	#endif
}

//#include <stdio.h>
//int main()
//{	timeint64_t start = get_high_res_time();
//
//	// Code to profile
//
//	timeint64_t end = get_high_res_time();
//	printf("Execution Time: %f seconds\n", get_elapsed_time(start, end)); }
