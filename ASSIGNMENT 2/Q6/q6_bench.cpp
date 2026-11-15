// Q6 growth investigation: 10,000,000 calls; friend (inline / non-inline), member, getters.
// The non-inline versions live in a separate translation unit (q6_noinline.cpp)
// so the compiler cannot inline them even at -O2.
#include <iostream>
#include <chrono>
#include "q6_time.h"

int main()
{
    const int N = 10000000;
    Time a(9, 45), b(13, 10);
    long long sum = 0;
    using clk = std::chrono::steady_clock;
    auto ms = [](clk::time_point s, clk::time_point e) {
        return std::chrono::duration<double, std::milli>(e - s).count(); };

    auto t0 = clk::now();
    for (int i = 0; i < N; i++) { Time x(i % 24, i % 60); sum += minutesBetweenInline(a, x); }
    auto t1 = clk::now();
    for (int i = 0; i < N; i++) { Time x(i % 24, i % 60); sum += minutesBetweenOut(a, x); }
    auto t2 = clk::now();
    for (int i = 0; i < N; i++) { Time x(i % 24, i % 60); sum += a.minutesTo(x); }
    auto t3 = clk::now();
    for (int i = 0; i < N; i++) { Time x(i % 24, i % 60); sum += std::abs((a.getH()*60 + a.getM()) - (x.getH()*60 + x.getM())); }
    auto t4 = clk::now();

    std::cout << "friend inline     : " << ms(t0, t1) << " ms\n";
    std::cout << "friend non-inline : " << ms(t1, t2) << " ms\n";
    std::cout << "member inline     : " << ms(t2, t3) << " ms\n";
    std::cout << "getters inline    : " << ms(t3, t4) << " ms\n";
    std::cout << "checksum " << sum << '\n';
}
