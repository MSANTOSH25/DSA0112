#include <iostream>
#include <string>

class Address {
    std::string city;
    int pin;
public:
    Address(const std::string& c, int p) : city(c), pin(p) {
        std::cout << "Address+ ";
    }
    ~Address() { std::cout << "Address- "; }
    std::string str() const { return city + "-" + std::to_string(pin); }
};

class Person {
protected:
    std::string name;
    int age;
    Address home;
public:
    Person(const std::string& n, int a, const std::string& city, int pin)
        : name(n), age(a), home(city, pin) { std::cout << "Person+ "; }
    virtual ~Person() { std::cout << "Person- "; }
};

class Course {
    std::string title;
public:
    explicit Course(const std::string& t) : title(t) { std::cout << "Course+ "; }
    ~Course() { std::cout << "Course- "; }
};

class Student : virtual public Person {
    long long regNo;
    Course course;
public:
    Student(const std::string& n, int a, const std::string& city, int pin,
            long long reg, const std::string& c)
        : Person(n, a, city, pin), regNo(reg), course(c) { std::cout << "Student+ "; }
    ~Student() override { std::cout << "Student- "; }
};

class Staff : virtual public Person {
    std::string department;
public:
    Staff(const std::string& n, int a, const std::string& city, int pin,
          const std::string& d)
        : Person(n, a, city, pin), department(d) { std::cout << "Staff+ "; }
    ~Staff() override { std::cout << "Staff- "; }
};

class TeachingAssistant : public Student, public Staff {
public:
    TeachingAssistant(const std::string& n, int a, const std::string& city, int pin,
                      long long reg, const std::string& course, const std::string& dept)
        : Person(n, a, city, pin),
          Student(n, a, city, pin, reg, course),
          Staff(n, a, city, pin, dept) { std::cout << "TA+ "; }
    ~TeachingAssistant() override { std::cout << "TA- "; }
};

int main() {
    std::cout << "Student: ";
    { Student s("Kavya", 19, "Salem", 636001, 192311001, "C++"); }
    std::cout << "\nStaff: ";
    { Staff s("Ravi", 35, "Chennai", 600001, "CSE"); }
    std::cout << "\nTeachingAssistant: ";
    { TeachingAssistant ta("Meena", 22, "Madurai", 625001, 192311002, "AI", "CSE"); }
    std::cout << '\n';
}
