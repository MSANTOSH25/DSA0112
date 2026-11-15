#!/bin/bash
# Run inside your cloned repository DSA01-192521007-Assignment2 (one commit per task)
git add README.md && git commit -m "Add README with name, register number and compile steps"
git add Q1 outputs/q1_* && git commit -m "Q1: call by value, reference and pointer"
git add Q2 outputs/q2_* && git commit -m "Q2: return by reference and dangling reference warning"
git add Q3 outputs/q3_* && git commit -m "Q3: ambiguity error and redesigned overloads"
git add Q4 outputs/q4_* && git commit -m "Q4: SavingsAccount with array inside the class"
git add Q5 outputs/q5_* && git commit -m "Q5: static members and array of objects"
git add Q6 outputs/q6_* && git commit -m "Q6: friend and inline functions with timings"
git add Q7 outputs/q7_* && git commit -m "Q7: circular-buffer redesign and validation"
git push origin main
