#include "q6_time.h"
int minutesBetweenOut(const Time& a, const Time& b)
{
    return std::abs((a.h * 60 + a.m) - (b.h * 60 + b.m));
}
