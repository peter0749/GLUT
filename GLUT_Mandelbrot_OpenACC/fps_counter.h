#ifndef __WIN32
#ifndef FPSCOUNTLIB
#include <time.h>
#include <sys/time.h>

double fpsCount(void){
    const unsigned skipFrames = 3;
    const double usScale = 1e6f;
    static int Frame=0;
    static double framePerSec=0.0;
    struct timeval tv;
    struct timezone tz;
    static double old_us=0.0f;
    double new_us;
    if(Frame<skipFrames){
        ++Frame;
    }else{
        gettimeofday(&tv, &tz);
        new_us = (double)tv.tv_sec*usScale+(double)tv.tv_usec;
        framePerSec = usScale/(new_us - old_us);
        old_us = new_us;
        Frame=0;
    }
    return framePerSec*(double)skipFrames;
}
#define FPSCOUNTLIB
#endif
#endif
