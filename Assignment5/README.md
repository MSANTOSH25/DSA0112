# Assignment 5 – CO5
**Student:** M. Santosh | **Register:** 192521007 | **Course:** DSA01

## Contents
- `PartA/ANSWERS.md`: design, UML, pseudocode, vtables, exceptions and complexity.
- `Task01`: hospital bill/payer polymorphism and completed B1 TODO program.
- `Task02`: error handling, rethrowing and completed B2 TODO program.
- `Task03`: switch/virtual/variant performance benchmark.
- `Task04`: extension with MaternityBill and CorporatePayer.
- `Predictions`: three output-prediction programs.
- `REPORT`: measurement and submission checklist.

## Build
`g++ -std=c++17 -Wall -Wextra -pedantic main.cpp -o main`
Run with `./main` (Windows PowerShell: `.\main.exe`).
Benchmark: `g++ -std=c++17 -O2 -Wall -Wextra -pedantic benchmark.cpp -o benchmark`

For AddressSanitizer where supported:
`g++ -std=c++17 -Wall -Wextra -pedantic -fsanitize=address -fno-omit-frame-pointer main.cpp -o main_asan`

**Before submission:** run all files yourself, add genuine terminal screenshots, record your compiler version and actual timings, perform sanitizer tests, add your GitHub URL, and commit progressively. Sample output files are text references, not screenshots.
