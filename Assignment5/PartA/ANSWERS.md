# PART A – DESIGN

## A1. Polymorphic bill hierarchy
**Common operations:** `total() const`, `type() const`, and `print() const`. Common data includes patient name/ID, bill number and date. `Bill` is abstract with pure virtual `total()` and `type()`, a normal `print()` and a virtual destructor. Derived classes: `OutPatientBill`, `InPatientBill`, `DayCareBill`, `EmergencyBill`; each overrides the pure virtual functions.

```text
                         <<abstract>> Bill
        -----------------------------------------------
        patient, billId
        total() = 0 {pure virtual}
        type()  = 0 {pure virtual}
        print() {ordinary; calls virtual functions}
        ~Bill() {virtual}
           /             |             |             \
 OutPatientBill    InPatientBill   DayCareBill   EmergencyBill
 consult, tests    days, rate,      package       base, rate,
                   procedures                      hours
```

**Pseudocode – day total**
```text
ALGORITHM DayTotal(Bills, n)
BEGIN
    sum <- 0
    FOR i <- 0 TO n-1
        IF Bills[i] != NULL THEN
            sum <- sum + Bills[i].total()
        END IF
    END FOR
    RETURN sum
END
```
**Alternative structure + switch:** store a type tag and charge fields in one record; `total()` switches on the tag. Both approaches total n bills in O(n). A tagged structure can reserve unused fields; polymorphic objects store their own fields and commonly a vptr. A new type requires editing the switch and record; polymorphism permits a new derived class without changing the processing loop.

## A2. Runtime binding
Early binding chooses a function at compile time using the declared type. Late binding selects an override at run time using the object's dynamic type. For `Bill* b = new OutPatientBill(...)`, virtual `b->total()` calls `OutPatientBill::total()`.

**Conceptual vtables** (implementation detail; not prescribed layout):
```text
Bill:          total=pure | type=pure | ~Bill
OutPatientBill: total=OP  | type=OP   | ~OutPatientBill
InPatientBill:  total=IP  | type=IP   | ~InPatientBill
b->total(): object dynamic type -> corresponding vtable entry -> override
```
A virtual destructor is essential when deleting a derived object through a base pointer. It runs the derived destructor then the base destructor. Without it, the delete expression has undefined behaviour.

**Prediction for supplied program:**
```text
500 ~OPBill ~Bill
```
`show()` itself is non-virtual, but it calls virtual `total()`, which dispatches to `OPBill::total()`. Removing `virtual` from the base destructor makes deletion through `Bill*` undefined behaviour; no particular output or sanitizer diagnostic is guaranteed.

**Alternative dynamic_cast:** test up to k derived types to select a calculation. Worst-case O(k) per bill and O(nk) for n bills, versus one virtual call per bill and O(n). A polymorphic object commonly stores a vptr; RTTI is also used by casts.

## A3. Payer interface
```cpp
class Payer {
public:
    virtual double amountCovered(const Bill&) const = 0;
    virtual ~Payer() = default;
};
```
`SelfPay` returns zero covered amount. `Insurance` stores cover limit and co-payment percentage; eligible amount is `min(bill.total(), limit)`, and payer amount is eligible amount multiplied by `(1 - coPayPercent/100)`. `GovernmentScheme` stores package rates and returns the applicable fixed rate (capped at bill total if that policy is adopted). Add `CorporateTieUp : public Payer` and override `amountCovered`; existing classes and common processing code remain unchanged.

**Alternative callable policy:** store a function pointer or `std::function<double(const Bill&)>`. A call is generally O(1), like virtual dispatch. A function pointer needs state passed separately; `std::function` can hold a lambda with captured cover limit but has wrapper/storage overhead and may allocate. A Payer object naturally stores policy state as members.

## A4. Exception strategy
Potential errors: negative charges/days; claim above cover; unknown package; missing patient record; invalid numeric text (`std::invalid_argument`); out-of-range number (`std::out_of_range`); allocation failure (`std::bad_alloc`); I/O/database failures.

```text
std::exception
  └─ std::runtime_error
       └─ BillingError
           ├─ InvalidChargeError
           ├─ ClaimError
           ├─ UnknownPackageError
           └─ MissingRecordError
```
Functions that validate, parse, allocate, look up records or calculate payer eligibility may throw. Destructors should not throw; cleanup destructors should be `noexcept`. A destructor throwing during stack unwinding can call `std::terminate`. Mark other functions `noexcept` only when their operations cannot throw.

**Return-code alternative:** return a status enum/result object and make callers check every result. Normal path avoids exception propagation; errors require explicit branches and status propagation. Result objects use some extra storage, and every caller must handle the status. Return codes are suitable when failures are frequent and expected.

## A5. Recovery and rethrowing
Validate close to the rule and throw a specific exception. A service/logging boundary catches by `const&`, logs context, and uses `throw;` to rethrow the same exception. The application boundary reports a clear message. `catch(...)` is a final safety net, not a replacement for specific handlers.

During stack unwinding, automatic objects are destroyed in reverse construction order. RAII types (`string`, `vector`, `unique_ptr`, file streams) release resources in their destructors, including when exceptions leave a scope.

**Prediction for supplied Guard program:**
```text
~f ~main caught 42
```
The local guard in `f` is destroyed first; then the `main` guard is destroyed while leaving the try block. The `not printed` statement is skipped because control transfers to the handler.

**Manual delete alternative:** every catch path must know which resources were acquired and release them. Complexity is repeated across error paths; each new path risks a missed delete/close. RAII centralises cleanup.

## B1. Implementation and validation
Use the abstract `Bill` interface and concrete classes. Expected supplied TODO output:
```text
OP Ravi: 1700
IP Meena: 47500
Total 49200
```
Hand check: 500+1200=1700; 3×2500+40000=47500; total=49200. Removing `virtual` from pure virtual `total()` makes the base declaration no longer pure virtual; the base class becomes abstract only if another pure virtual remains. In the supplied TODO code, `type()` is virtual but not pure, so `Bill` may become instantiable, while calls through `Bill*` use the base `total()` if it has a definition; because it has no definition, linking may fail. Keep `total()` virtual and pure.

**Templates alternative:** separate containers per concrete type and template functions to process each container. Each calculation avoids virtual dispatch; all totals remain O(n). The cost is separate containers and loss of one naturally heterogeneous `vector<Bill*>`-style collection.

## B2. Engineer error conditions
Test negative charges, claim above cover, unknown package, non-numeric text with `stod`, and an impossible allocation. Catch `std::bad_alloc` specifically; some oversized requests can instead throw `std::length_error`, so handle that too. A top-level `catch(...)` prevents an uncaught exception from terminating the normal control path, while RAII destructors demonstrate cleanup.

**Prediction for validate/process program:**
```text
ok done caught negative [log too large] caught too large
```
The first value passes and prints `ok done`. Negative input throws `invalid_argument`, which `process` does not catch, so main catches it. The large value throws `out_of_range`, logged by `process`, then rethrown with `throw;` and caught by main. Only `out_of_range` is logged because only that type has a handler in `process`.

**Completed TODO prediction:**
```text
approved 40000
audit: exceeds cover
claim rejected: exceeds cover
error: negative amount
```
`throw;` preserves the original exception type/object. `throw e;` throws a new exception based on the static type of `e` (here `ClaimError` because the catch parameter is `ClaimError`); in a catch of a base type, throwing the parameter can slice a derived exception. Rethrow with `throw;`.

**optional alternative:** return `std::optional<double>` where empty means rejected. For one million claims, both designs do O(1) work per claim and O(n) overall. Return values are predictable for common invalid cases; exception-based code is often efficient on the success path but expensive when many exceptions are thrown. Each optional result stores a value plus an engaged/disengaged state; exception handling uses runtime exception machinery and exception objects on failure.

## B3. Cost comparison
Run identical data through virtual and switch versions and verify equal totals for 10 bills. Measure 1,000, 1,000,000 and 10,000,000 bills with `-O2`; repeat several times and report median. For exception cost, compare return-code and exception validation at 0%, 1% and 50% invalid rates across one million claims. Do not invent timing values: record actual machine/compiler results.

**Engineering trade-off:** virtual functions support open-ended extension and heterogeneous collections; switch is simple for a small fixed set. Exceptions separate rare failures from normal flow; return codes/optional are often better for frequent expected invalid input.

**variant alternative:** use `std::variant<OutPatientBill, InPatientBill, ...>` and `std::visit`. It supports heterogeneous values in one container, has no base-class vptr per element, and each element occupies enough storage for the largest alternative plus a discriminator/padding. It can be fast but adding a new alternative may require updating visitors.

## B4. Final engineering decision
Add `MaternityBill` and `CorporatePayer` as new derived classes. Existing classes and the generic processing loop need not change. Run all previous tests and sanitizer tests again. For 15 hospitals, growing bill/payer types and 200,000 bills/day, polymorphism is a suitable maintainable design; exceptions suit exceptional system failures, while ordinary user validation can use explicit results. Confirm performance with the measured benchmark and production profiling.

**Plug-in/rule alternative:** load rates and rules from validated configuration. Per bill, rule lookup/interpreting may be more work than a virtual call, but rates can change without recompilation. Rules require storage and validation; plug-ins/configuration add security, versioning and operational risks. C++ classes offer compile-time checks but require a software release for each new type.
