# DSA01 – Assignment 2 (CO2)

- **Name:** M.Santosh
- **Register number:** 192521007
- **Compiler:** g++ (Ubuntu 13.3.0-6ubuntu2~24.04.1) 13.3.0
- **Compile command:** `g++ -std=c++17 -Wall -Wextra file.cpp -o file`
  (add `-O0` or `-O2` for the timing programs in Q4, Q6 and Q7)

## How to run

| Task | Build and run |
|------|---------------|
| Q1 | `g++ -std=c++17 -Wall -Wextra Q1/q1_three_ways.cpp -o q1 && ./q1` (also q1_predict.cpp, q1_complete.cpp) |
| Q2 | `g++ -std=c++17 -Wall -Wextra Q2/q2_marks.cpp -o q2 && ./q2`; add `-DDANGLING` to see the warning and the crash |
| Q3 | `q3_ambiguous.cpp` must FAIL to compile (that is the evidence); `q3_courier.cpp` builds and runs |
| Q4 | `g++ -std=c++17 -Wall -Wextra -O2 Q4/q4_account.cpp -o q4 && ./q4` |
| Q5 | `q5_predict.cpp`, `q5_complete.cpp`, `q5_function_test.cpp` |
| Q6 | `g++ -std=c++17 -Wall -Wextra -O2 Q6/q6_bench.cpp Q6/q6_noinline.cpp -o q6 && ./q6` (and with `-O0`); `q6_complete.cpp` builds alone |
| Q7 | `g++ -std=c++17 -Wall -Wextra -O2 Q7/q7_redesign.cpp -o q7 && ./q7` (and with `-O0`) |

## Result of each task

- **Q1** – by value leaves the fee at 45000; by reference and by pointer give 40500. Predict-the-output printed `1 12 13`, `1 100`, `16 13 6`.
- **Q2** – returning a reference to the highest mark lets `+= 2` change the array. A reference to a local gives warning `-Wreturn-local-addr` and the program crashed (segmentation fault). A vector reallocated (capacity 3 to 6), invalidating earlier references.
- **Q3** – the original overloads give an "ambiguous call" error. The redesign selects int-grams, double or Parcel as predicted; `charge(2)` selects the int (grams) overload.
- **Q4** – SavingsAccount keeps the last 10 transactions; the shifting array was 111.8 ms (-O0) and 30.4 ms (-O2) for 5,000,000 deposits, against 18.3 ms and 15.9 ms for a circular index.
- **Q5** – Token prediction `101 103 104 4` was correct. BusTicket output matches the expected output (seats 1 to 3, Dev waiting, Issued: 3). The count is not reset when tickets are created inside a function.
- **Q6** – `205 false`; tests across noon (20 true) and the same time (0 false) pass. At -O2 the inline functions were faster than the non-inline friend (14.1 ms vs 17.7 ms median for 10,000,000 calls); at -O0 inline made no difference.
- **Q7** – the Q4 shift-array history was redesigned as a circular buffer: time per transaction no longer grows with N (5.4x faster at N = 50 and 27x at N = 500, at -O2).
