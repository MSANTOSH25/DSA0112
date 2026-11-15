// Q4: SavingsAccount with a private array of the last 10 transactions
#include <iostream>
#include <string>
#include <chrono>

class SavingsAccount {
    static const int N = 10;
    long accNo;
    double balance;
    double txn[N];       // + deposit, - withdrawal
    int count;           // transactions currently stored (<= N)
public:
    SavingsAccount(long no, double bal) : accNo(no), balance(bal), count(0) {}

    // defined inside the class
    double getBalance() const { return balance; }
    double sumStored() const { double s = 0; for (int i = 0; i < count; i++) s += txn[i]; return s; }

    // defined outside with ::
    void deposit(double amt);
    bool withdraw(double amt);
    void miniStatement() const;
private:
    void record(double t);
};

void SavingsAccount::record(double t)
{
    if (count == N) {                       // 11th transaction: shift left, O(N)
        for (int i = 1; i < N; i++) txn[i - 1] = txn[i];
        txn[N - 1] = t;
    } else {
        txn[count++] = t;
    }
}
void SavingsAccount::deposit(double amt) { balance += amt; record(+amt); }
bool SavingsAccount::withdraw(double amt)
{
    if (amt > balance) return false;
    balance -= amt; record(-amt);
    return true;
}
void SavingsAccount::miniStatement() const
{
    std::cout << "Mini statement for " << accNo << " (last " << count << ")\n";
    for (int i = 0; i < count; i++)
        std::cout << "  " << (txn[i] >= 0 ? "CR " : "DR ") << (txn[i] >= 0 ? txn[i] : -txn[i]) << '\n';
    std::cout << "  Balance: " << balance << '\n';
}

// Circular-buffer version for comparison (O(1) per add)
class CircAccount {
    static const int N = 10;
    double txn[N]; int start = 0, count = 0; double balance = 0;
public:
    void add(double t) {
        if (count < N) txn[(start + count++) % N] = t;
        else { txn[start] = t; start = (start + 1) % N; }
        balance += t;
    }
    double last() const { return txn[(start + count - 1) % N]; }
    double sumStored() const { double s = 0; for (int i = 0; i < count; i++) s += txn[(start + i) % N]; return s; }
};

int main()
{
    SavingsAccount a(100200300, 5000);
    for (int i = 1; i <= 12; i++) {
        if (i % 3 == 0) a.withdraw(100 * i); else a.deposit(100 * i);
    }
    a.miniStatement();

    // timing: 5,000,000 transactions, shift vs circular
    const int OPS = 5000000;
    SavingsAccount s(1, 0);
    auto t0 = std::chrono::steady_clock::now();
    for (int i = 0; i < OPS; i++) s.deposit(1);
    auto t1 = std::chrono::steady_clock::now();
    CircAccount c;
    for (int i = 0; i < OPS; i++) c.add(1);
    auto t2 = std::chrono::steady_clock::now();
    std::cout << "shift   : " << std::chrono::duration<double, std::milli>(t1 - t0).count() << " ms (sum " << s.sumStored() << ")\n";
    std::cout << "circular: " << std::chrono::duration<double, std::milli>(t2 - t1).count() << " ms (sum " << c.sumStored() << ")\n";
}
