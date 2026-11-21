#include <iostream>
#include <string>

class Person {
protected:
    std::string name;
    int id;
    std::string phone;
public:
    Person(std::string n = "", int i = 0, std::string p = "")
        : name(std::move(n)), id(i), phone(std::move(p)) {}
    void printPerson() const {
        std::cout << name << " ID:" << id << " Phone:" << phone;
    }
};

class Student : virtual public Person {
protected:
    std::string programme;
    double cgpa;
public:
    Student(std::string n, int i, std::string p, std::string prog, double c)
        : Person(std::move(n), i, std::move(p)),
          programme(std::move(prog)), cgpa(c) {}
};

class Staff : virtual public Person {
protected:
    std::string department;
    double salary;
public:
    Staff(std::string n, int i, std::string p, std::string dept, double s)
        : Person(std::move(n), i, std::move(p)),
          department(std::move(dept)), salary(s) {}
};

class TeachingAssistant : public Student, public Staff {
public:
    TeachingAssistant(std::string n, int i, std::string p,
                      std::string prog, double c, std::string dept, double s)
        : Person(n, i, p), Student(n, i, p, prog, c),
          Staff(n, i, p, dept, s) {}
    void print() const {
        printPerson();
        std::cout << " Programme:" << programme << " CGPA:" << cgpa
                  << " Department:" << department << " Salary:" << salary << '\n';
    }
};

class ResearchScholar : public Student {
    double fellowship;
public:
    ResearchScholar(std::string n, int i, std::string p,
                    std::string prog, double c, double f)
        : Person(n, i, p),
          Student(n, i, p, std::move(prog), c), fellowship(f) {}
    void print() const {
        printPerson();
        std::cout << " Programme:" << programme << " CGPA:" << cgpa
                  << " Fellowship:" << fellowship << '\n';
    }
};

int main() {
    TeachingAssistant ta("Kavya", 201, "9876543210", "BSc CS", 8.7, "CSE", 25000);
    ResearchScholar rs("Arun", 202, "9123456780", "MSc AI", 9.1, 18000);
    ta.print();
    rs.print();
}
