// Q2: Return by reference and its dangers
#include <iostream>
#include <vector>

int& highest(int arr[], int n)
{
    int idx = 0;
    for (int i = 1; i < n; i++) if (arr[i] > arr[idx]) idx = i;
    return arr[idx];                 // reference to an element that outlives the call
}
const int& highestConst(const int arr[], int n)
{
    int idx = 0;
    for (int i = 1; i < n; i++) if (arr[i] > arr[idx]) idx = i;
    return arr[idx];                 // read-only reference
}
int highestCopy(const int arr[], int n)
{
    int idx = 0;
    for (int i = 1; i < n; i++) if (arr[i] > arr[idx]) idx = i;
    return arr[idx];                 // independent copy
}
int highestIndex(const int arr[], int n)
{
    int idx = 0;
    for (int i = 1; i < n; i++) if (arr[i] > arr[idx]) idx = i;
    return idx;
}
int* highestPtr(int arr[], int n)
{
    int idx = 0;
    for (int i = 1; i < n; i++) if (arr[i] > arr[idx]) idx = i;
    return &arr[idx];
}

#ifdef DANGLING
int& broken()
{
    int local = 99;
    return local;                    // reference to a local variable: UB
}
#endif

int main()
{
    int marks[5] = {56, 78, 91, 64, 70};
    highest(marks, 5) += 2;          // moderation applied directly
    highest(marks, 5) = highest(marks, 5);   // call usable on the left of =
    std::cout << "after +2 moderation: ";
    for (int m : marks) std::cout << m << ' ';
    std::cout << '\n';

    int copy = highestCopy(marks, 5);
    copy += 100;                     // does not touch the array
    std::cout << "copy changed, array max still " << highestConst(marks, 5) << '\n';
    // highestConst(marks, 5) += 2;  // error: assignment of read-only reference

    marks[highestIndex(marks, 5)] += 1;   // index version
    *highestPtr(marks, 5) += 1;           // pointer version
    std::cout << "after index and pointer versions: " << marks[2] << '\n';

#ifdef DANGLING
    int& r = broken();
    std::cout << "dangling value: " << r << '\n';
#endif

    // Changed condition: growing vector
    std::vector<int> v = {56, 78, 91};
    int& ref = v[2];
    int* before = &v[0];
    std::cout << "capacity before: " << v.capacity() << '\n';
    v.push_back(10); v.push_back(11);    // forces reallocation
    int* after = &v[0];
    std::cout << "capacity after : " << v.capacity() << '\n';
    std::cout << "buffer moved   : " << (before != after ? "yes" : "no") << '\n';
    (void)ref;                       // using ref now would be undefined behaviour
    int idx = 2;                     // an index stays valid
    std::cout << "by index still valid: " << v[idx] << '\n';
}
