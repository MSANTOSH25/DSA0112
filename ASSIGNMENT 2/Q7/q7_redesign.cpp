// Q7: Diagnose, redesign and validate - the Q4 transaction history.
// Original  : shift-left array, O(K) per transaction once full.
// Redesign  : circular buffer with a start index, O(1) per transaction.
// Both are templates on K (history length) so the SAME tests run for K = 10, 50, 500, 5000.
#include <iostream>
#include <chrono>
#include <vector>

template <int K>
class ShiftHistory {
    double txn[K]; int count = 0;
public:
    void add(double t) {
        if (count == K) { for (int i = 1; i < K; i++) txn[i - 1] = txn[i]; txn[K - 1] = t; }
        else txn[count++] = t;
    }
    int size() const { return count; }
    double at(int i) const { return txn[i]; }          // i = 0 oldest
};

template <int K>
class CircularHistory {
    double txn[K]; int start = 0, count = 0;
public:
    void add(double t) {
        if (count < K) txn[(start + count++) % K] = t;
        else { txn[start] = t; start = (start + 1 == K) ? 0 : start + 1; }
    }
    int size() const { return count; }
    double at(int i) const { return txn[(start + i) % K]; }   // i = 0 oldest
};

template <class H>
double runMs(long ops, double& check)
{
    H h;
    auto t0 = std::chrono::steady_clock::now();
    for (long i = 0; i < ops; i++) h.add((double)(i % 1000));
    auto t1 = std::chrono::steady_clock::now();
    double s = 0; for (int i = 0; i < h.size(); i++) s += h.at(i);
    check = s;
    return std::chrono::duration<double, std::milli>(t1 - t0).count();
}

template <int K>
void compare(long ops)
{
    double c1, c2;
    double a = runMs<ShiftHistory<K>>(ops, c1);
    double b = runMs<CircularHistory<K>>(ops, c2);
    std::cout << "K=" << K << "\tshift " << a << " ms\tcircular " << b << " ms\tspeed-up "
              << a / b << "x\tsame contents: " << (c1 == c2 ? "yes" : "NO") << '\n';
}

int main()
{
    // correctness: both must return identical last-K sequences
    ShiftHistory<10> s; CircularHistory<10> c;
    for (int i = 1; i <= 25; i++) { s.add(i); c.add(i); }
    bool same = true;
    for (int i = 0; i < 10; i++) if (s.at(i) != c.at(i)) same = false;
    std::cout << "last 10 of 25 identical: " << (same ? "yes" : "NO")
              << " (oldest " << s.at(0) << ", newest " << s.at(9) << ")\n";

    const long OPS = 2000000;
    compare<10>(OPS);
    compare<50>(OPS);
    compare<500>(OPS);
    compare<5000>(OPS / 10);
    std::cout << "sizeof: shift<50>=" << sizeof(ShiftHistory<50>) << " circular<50>=" << sizeof(CircularHistory<50>) << " bytes\n";
}
