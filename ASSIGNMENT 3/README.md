# DSA Assignment 3 – CO3

**Student:** M. Santosh  
**Register Number:** 192521007  
**Course:** DSA0112  
**Repository:** `DSA01-192521007-Assignment3`

## Contents
- `Task01`–`Task07`: one folder per assignment question.
- Each task folder contains a C++17 source file, sample output, and a short task note.
- `ANSWERS.md`: theory, design explanations, predictions, alternative approaches, and complexity analysis.
- `Assignment3_Answers.pdf` is not included; the complete editable answers are in `ANSWERS.md`.

## Compiler
Tested with **g++ (C++17)**. Check your local version with:

```bash
g++ --version
```

## Compile and run
Open a terminal in the required task folder. Example:

```bash
g++ -std=c++17 -Wall -Wextra -pedantic main.cpp -o main
./main
```

On Windows Command Prompt:

```bat
g++ -std=c++17 -Wall -Wextra -pedantic main.cpp -o main.exe
main.exe
```

For the copy/memory tasks, AddressSanitizer can be used where supported:

```bash
g++ -std=c++17 -Wall -Wextra -fsanitize=address -fno-omit-frame-pointer main.cpp -o main
./main
```

## Task summary
| Folder | Topic | Result |
|---|---|---|
| Task01 | Constructors and constructor overloading | Creates valid Book objects using delegated constructors |
| Task02 | Shallow/deep copy and Rule of Three | Copies prescription arrays independently |
| Task03 | Dynamic constructor and destructor | Allocates and releases a two-dimensional seat map |
| Task04 | Arithmetic and stream operator overloading | Performs money arithmetic in integer paise |
| Task05 | Increment, comparison and subscript operators | Implements prefix/postfix increment and checked playlist access |
| Task06 | Operator overloading rules and Text class | Manages character memory safely and overloads operators |
| Task07 | Complex-number class | Calculates series and parallel impedance |

## GitHub submission
1. Create a repository named `DSA01-192521007-Assignment3`.
2. Extract this ZIP and upload the repository contents.
3. Commit progressively, ideally one commit per task (do not make only one final commit).
4. Add the course faculty as collaborator if the repository is private.
5. Add the working GitHub repository URL to the assignment cover page and submission form.

**Important:** Review the code, compile it on your own computer, capture genuine terminal screenshots, and add them to each task folder before submission. Sanitizer timings and machine-dependent measurements should be collected on your own system; no fabricated measurements are included here.
