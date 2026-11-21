# Assignment 3 – Complete Answers (CO3)

**Name:** M. Santosh  
**Register Number:** 192521007  
**Course Code:** DSA0112

> The corresponding executable C++17 programs are in `Task01` through `Task07`. Sample outputs are saved beside each source file. Machine-specific timings and sanitizer logs must be captured by running the programs on the student's own computer.

---

# Q1 – Constructors and Constructor Overloading

## Design
A `Book` has a title and number of copies. The default constructor creates an empty record, the title-only constructor represents a donated book with one copy, and the two-argument constructor accepts full details. The first two constructors delegate to the full constructor. A validation function rejects negative copies by throwing `std::invalid_argument`.

## Constructor output
```text
Title: (empty), Copies: 0
Title: C++ Basics, Copies: 1
Title: Data Structures, Copies: 5
```

## Questions
1. **When does the compiler provide a default constructor?** If a class has no user-declared constructor, the compiler may generate a default constructor. If any constructor is declared by the programmer, the compiler does not automatically add a no-argument constructor.
2. **Why use a member initializer list?** It directly initializes members, works for `const` and reference members, and avoids default construction followed by assignment.
3. **Initialization order:** Members are initialized in the order in which they are declared in the class, not the order written in the initializer list. Write the initializer list in declaration order to avoid `-Wreorder` warnings.
4. **Object states:** `empty` has `("", 0)`; `donated` has `("C++ Basics", 1)`; `full` has `("Data Structures", 5)`.
5. **Removing the default constructor:** `Book books[3];` fails to compile if no accessible no-argument constructor exists, because each array element must be default-constructed.
6. **Negative copies:** Constructing `Book("Bad", -2)` throws `invalid_argument`; the invalid object is not completed.
7. **`explicit`:** Mark a single-argument constructor explicit when implicit conversion is undesirable. For example, with `explicit Book(std::string)`, `Book b = "Novel";` is rejected, while `Book b("Novel");` is valid. Without `explicit`, the copy-initialization may compile through an implicit conversion.

## Alternative: one constructor with default arguments
`Book(std::string title = "", int copies = 0)` can replace the overload set. It still validates `copies` in one place. Both approaches create a fixed-size object in O(1) time and O(1) space. A single constructor is shorter; overloads can communicate the different creation situations more clearly. Delegation also centralizes validation.

---

# Q2 – Copy Constructors: Shallow and Deep Copies

## Deep-copy implementation
The program in `Task02/main.cpp` implements:
- Copy constructor: allocates a new array and copies every medicine.
- Copy assignment: checks self-assignment, allocates and fills a fresh array, deletes the old array, then updates the object.
- Destructor: calls `delete[]` to release the array.

## Output
```text
Ibuprofen ORS | Paracetamol Cetirizine | Paracetamol ORS
```
The three prescriptions are independent. Changing `p` does not change `q` or `r`.

## Memory diagrams
**Shallow copy:** `p.medicines` and `q.medicines` point to the same allocated array. A change through either object affects the same data. When both destructors run, both attempt to delete the same allocation, causing undefined behaviour (often a double-free error).

**Deep copy:** `p.medicines` points to allocation A and `q.medicines` points to separate allocation B. Each object owns and deletes its own array.

## Rule of Three
If a class manually owns a resource, it usually needs all three:
1. Destructor – releases the resource.
2. Copy constructor – defines how a new object receives a copy.
3. Copy-assignment operator – defines how an existing object receives a copy.

They go together because the compiler-generated copying of a raw pointer copies only the address, not the pointed-to resource.

## When is the copy constructor called?
- Initializing an object from another object, such as `Prescription q = p`.
- Passing an object by value to a function.
- Returning an object by value may involve copying, although modern C++ often applies copy elision.

Passing by `const Prescription&` does not make a copy. Passing by value generally creates a copy unless optimized by permitted elision.

## Alternative: `std::vector<std::string>`
A vector manages its own memory and copies its elements correctly. The class can use the Rule of Zero and omit custom copy/destructor functions. Copying n medicines takes O(n) time and O(n) additional storage in either deep-copy design. A shallow pointer copy is O(1), but it is unsafe for independent ownership.

## Sanitizer and timing investigation
Compile with:
```bash
g++ -std=c++17 -Wall -Wextra -fsanitize=address -fno-omit-frame-pointer main.cpp -o main
```
The deep-copy program should finish without an ownership error. The intentionally shallow version can report double-free/use-after-free. Do not submit an invented sanitizer transcript: run both versions and paste your actual terminal output. For 10, 1,000 and 100,000 elements, measure using `std::chrono`; record your own computer's results. Self-assignment `p = p` is safe because it is checked before replacing the data.

---

# Q3 – Dynamic Constructors and Destructors

## Design
`Event` allocates a row-pointer array and then allocates each row. Every seat begins as `O` (open). Booking changes a seat to `X`; cancellation changes it back to `O`. The destructor deletes each row and then the row-pointer array. Copying is disabled to prevent accidental shallow copies.

## Object lifetime
- A local object is destroyed when its scope ends.
- A block-scope object is destroyed at the closing brace of that block.
- A dynamically allocated object remains alive until `delete` is called.
- Local objects are destroyed in reverse order of completed construction.

If `delete` is forgotten, the allocation leaks. For arrays allocated with `new[]`, use `delete[]`; using scalar `delete` instead is undefined behaviour. A destructor has no arguments and no return type, and a class can have only one destructor, so it cannot be overloaded.

## Seat map memory layout
`map` points to an array of `rows` pointers. Each pointer points to a separate array of `seats` characters. This design uses `rows + 1` allocations when rows are positive.

## Logger predicted output
The exact output (including spaces) is:
```text
+A +B -B copyAc use -Ac +P -P end -A
```
Explanation:
- `a` is constructed and prints `+A`.
- `b` is constructed inside the block and prints `+B`; it is destroyed at the block end and prints `-B`.
- `use(a)` passes by value, so a copy named `Ac` is constructed, `use` prints `use`, and the parameter is destroyed after the call, printing `-Ac`.
- `p` is constructed dynamically (`+P`) and explicitly deleted (`-P`).
- `end` is printed; finally local `a` is destroyed (`-A`).

## Alternative seat-map designs
A single flat array uses one allocation for all `rows × seats` cells. Cell `(r,s)` is at index `r × seats + s`. A `vector<vector<char>>` is easier and manages memory automatically, but normally has one vector allocation per row plus its outer vector storage. Access remains O(1). A flat allocation has one allocation rather than `rows + 1`, and avoids the row-pointer array. Space for seat cells is O(rows × seats); row-pointer overhead is O(rows).

## RAII
Resource Acquisition Is Initialization (RAII) ties resource lifetime to object lifetime. When an object leaves scope, its destructor releases memory automatically, reducing leaks and making early returns and exceptions safer.

---

# Q4 – Arithmetic and Stream Operator Overloading

## Design
Money stores the complete value as integer paise, avoiding floating-point rounding. `+` creates and returns a new object; `+=` changes the current object and returns `*this` by reference. `==` and `<` compare paise. `<<` is a non-member friend because the left operand is an output stream, not a `Money` object.

## Complete-code output
```text
Rs 200.00
true
Rs 1.25
-Rs 1.00
```
The third line verifies paise carry: 75 paise + 50 paise = 125 paise = Rs 1.25.

## Operator translation
`a + b` is treated as `a.operator+(b)` for a member overload (or an eligible non-member overload). `<<` is conventionally a non-member so that `std::cout << money` works.

## Negative subtraction
This implementation permits a negative balance and prints a minus sign. Another valid design is to reject negative results with an exception, depending on billing rules.

## Engineering decision
Multiplying one Money amount by another Money amount is usually meaningless for a billing counter. Multiplication by a quantity or scalar can make sense; division by a scalar can also make sense. The units and rounding policy should be defined.

## Alternative
Friend non-member operators and named functions can all perform constant work. Addition, comparison and output are O(1) time; object storage is O(1). A non-member overload can support `100 + m` as well as `m + 100` if it accepts/converts the left operand. A member overload naturally has the class object on the left.

---

# Q5 – Increment, Comparison and Subscript Operators

## Prefix and postfix
- Prefix: `++t` increments first and returns a reference to the updated object.
- Postfix: `t++` has a dummy `int` parameter, saves the old value, increments the object, and returns the old value by copy.

## Predicted output
```text
5 7 7
```
Starting at 5, `a = t++` stores 5 and changes `t` to 6. Then `b = ++t` changes `t` to 7 and stores 7. Therefore `a=5`, `b=7`, and `t=7`.

## Playlist
A non-const `operator[]` returns `std::string&`, allowing modification. A const overload returns `const std::string&`, preventing modification through a const playlist. This implementation throws `std::out_of_range` when the index is invalid. Since `std::size_t` is unsigned, a negative index converts to a very large value and is rejected by the bounds check.

## Alternative access
`at(i)` conventionally throws `std::out_of_range`. An unchecked subscript may have undefined behaviour for an invalid index. All valid accesses are O(1); checked access adds a constant-time comparison. The check does not change Big-O complexity.

## Why prefer prefix?
For class types, postfix normally creates a temporary copy to preserve the old value. Prefix avoids that extra copy and is usually preferred when the old value is not needed.

---

# Q6 – Rules of Operator Overloading and Text Class

## Implemented class
`Text` owns a dynamically allocated character array. It defines a constructor, deep-copy constructor, copy assignment, destructor, concatenation, equality, inequality and stream output. Assignment checks self-assignment.

## Operators that cannot be overloaded
`::` (scope resolution), `.` (member access), `.*` (pointer-to-member access), `?:` (conditional operator), and `sizeof` cannot be overloaded.

## Rules
- Overloading cannot change an operator's precedence.
- Overloading cannot change its number of operands (arity).
- At least one operand must be a user-defined type.
- The assignment operator `=` must be a member function.
- A friend is useful when an operator needs access to private data but the left operand is not the class, as with stream insertion `<<`.

## Copy count for `a + b + c`
The expression is grouped as `(a + b) + c` because `+` is left-associative. Each concatenation creates a result. Actual copy-constructor calls may be reduced by copy elision and depend on the implementation and C++ version. Returning by value is the natural interface.

## Alternative: `std::string`
`std::string` already manages memory, copying, assignment and concatenation safely, so it is preferred for normal applications. A custom `Text` class is useful for learning ownership or implementing special storage/encoding rules. Concatenating lengths n1, n2 and n3 requires copying characters proportional to the intermediate and final string sizes; a straightforward two-step `+` can copy O(n1+n2+n3) characters, with temporary storage for intermediate results. Repeated concatenation in a loop can become costly; reserving capacity or using append can reduce reallocations.

---

# Q7 – Complex Number Class for AC Impedance

## Stage 1 – Requirements
Data: real and imaginary parts, both stored as `double`. Operations: addition, subtraction, multiplication, division, equality, printing and magnitude. Constructors: default `(0,0)`, real-only `(r,0)`, and real-plus-imaginary `(r,i)`.

Measurable requirements:
- Correctly calculate the four arithmetic operations.
- Print rectangular form.
- Calculate magnitude using `sqrt(real² + imag²)`.
- Detect division by zero impedance and report an error.

## Stage 2 – Design and formulas
For `z1 = a + jb` and `z2 = c + jd`:
- Addition: `(a+c) + j(b+d)`
- Subtraction: `(a-c) + j(b-d)`
- Multiplication: `(ac-bd) + j(ad+bc)`
- Division: `[(ac+bd) + j(bc-ad)] / (c²+d²)`
- Magnitude: `sqrt(a²+b²)`
- Equality: compare both components (the sample uses a small tolerance).

Arithmetic operators are members. Stream insertion is a friend non-member because the left operand is `std::ostream`.

## Stage 3 – Example verification
The program uses:
- Resistor: `10 + j0`
- Inductor example: `0 + j5`
- Capacitor example: `0 - j10`

Series total:
`(10+j0) + (0+j5) + (0-j10) = 10-j5`

Magnitude:
`sqrt(10² + (-5)²) = sqrt(125) ≈ 11.1803`

Parallel combination of the resistor and inductor:
`(10 × j5) / (10 + j5) = (j50)/(10+j5) = 2+j4`

Division by zero is tested and handled with `std::domain_error`.

## Stage 4 – Evaluation
`std::complex<double>` provides a standard, tested implementation of complex arithmetic and is preferable in production. A custom class demonstrates operator overloading and allows a custom error policy. Both representations store two doubles and each basic operation takes O(1) time and O(1) space. Floating-point calculations have finite precision; exact equality is generally unsuitable, so a tolerance is used. One limitation of this simple class is that it does not model frequency-dependent component values or validate physical circuit inputs.

## Alternative: `std::complex<double>`
Use `<complex>`, construct `std::complex<double> z(real, imag)`, and use `+`, `-`, `*`, `/`, `std::abs(z)` and stream output. Complexity remains O(1) per operation and storage remains two doubles in typical implementations. The custom class requires manual division-by-zero handling; standard complex arithmetic follows the library's floating-point rules, so an application should still decide how to handle a zero denominator.
