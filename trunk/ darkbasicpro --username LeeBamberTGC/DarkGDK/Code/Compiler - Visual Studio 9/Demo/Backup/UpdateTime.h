#ifndef UPDATE_TIME_H
#define UPDATE_TIME_H

#ifdef LINUX
#include <sys/time.h>
#endif

#include <windows.h>

NxReal UpdateTime()
{
    NxReal deltaTime;
#ifndef LINUX
    static __int64 gTime,gLastTime;
    __int64 freq;
    QueryPerformanceCounter((LARGE_INTEGER *)&gTime);  // Get current count
    QueryPerformanceFrequency((LARGE_INTEGER *)&freq); // Get processor freq
    deltaTime = (double)(gTime - gLastTime)/(double)freq;
    gLastTime = gTime;
#else
	struct timeval tv;
	static struct timeval lasttv = { 0 , 0 };
	if (lasttv.tv_usec == 0 && lasttv.tv_sec == 0)
		gettimeofday(&lasttv, NULL);
	gettimeofday(&tv, NULL);
	deltaTime = (tv.tv_usec - lasttv.tv_usec)/1000000.f
		+ (tv.tv_sec - lasttv.tv_sec);
	lasttv = tv;
#endif
    return deltaTime;
}

#endif
