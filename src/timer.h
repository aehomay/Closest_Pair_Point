#include <time.h>

static struct timeval tm1;

static inline void start_watch()
{
    gettimeofday(&tm1, NULL);
}

static inline unsigned long long stop_watch()
{
    struct timeval tm2;
    gettimeofday(&tm2, NULL);

    unsigned long long t = 1000 * (tm2.tv_sec - tm1.tv_sec) + (tm2.tv_usec - tm1.tv_usec) / 1000;
    return t;
}
