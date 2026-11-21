#include <iostream>
#include <memory>
#include <string>
#include <vector>

class Staff {
    std::string name;
    int id;
    double basic;
protected:
    Staff(std::string n, int i, double b)
        : name(std::move(n)), id(i), basic(b) {}
    double basicPay() const { return basic + basic * 0.42; }
public:
    virtual ~Staff() = default;
    const std::string& getName() const { return name; }
    int getId() const { return id; }
    virtual double pay() const = 0;
    virtual void print() const {
        std::cout << getName() << " (" << getId() << "): Rs "
                  << pay() << '\n';
    }
};

class Professor : public Staff {
    double research;
public:
    Professor(std::string n, int i, double b, double r)
        : Staff(std::move(n), i, b), research(r) {}
    double pay() const override { return basicPay() + research; }
};

class LabAssistant : public Staff {
public:
    LabAssistant(std::string n, int i, double b)
        : Staff(std::move(n), i, b) {}
    double pay() const override { return basicPay(); }
};

class GuestFaculty : public Staff {
    double hours, rate;
public:
    GuestFaculty(std::string n, int i, double h, double r)
        : Staff(std::move(n), i, 0), hours(h), rate(r) {}
    double pay() const override { return hours * rate; }
};

int main() {
    std::vector<std::unique_ptr<Staff>> staff;
    staff.push_back(std::make_unique<Professor>("Anita", 101, 50000, 8000));
    staff.push_back(std::make_unique<LabAssistant>("Bala", 102, 30000));
    staff.push_back(std::make_unique<GuestFaculty>("Kiran", 103, 40, 700));

    for (const auto& person : staff) person->print();
}
