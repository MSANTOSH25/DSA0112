#include <iostream>
#include <memory>
#include <string>
#include <vector>

class Vehicle {
    std::string registration;
    double dailyRate;
    bool rented = false;
protected:
    Vehicle(std::string reg, double rate)
        : registration(std::move(reg)), dailyRate(rate) {}
    double baseRate() const { return dailyRate; }
public:
    virtual ~Vehicle() = default;
    const std::string& getRegistration() const { return registration; }
    bool isRented() const { return rented; }
    bool rent() {
        if (rented) return false;
        rented = true;
        return true;
    }
    double returnVehicle(int daysLate = 0) {
        if (!rented) return 0.0;
        rented = false;
        return rentalCharge() + (daysLate > 0 ? daysLate * 200.0 : 0.0);
    }
    virtual double rentalCharge() const = 0;
    virtual std::string type() const = 0;
    virtual void print() const {
        std::cout << type() << " " << registration
                  << " daily rate Rs " << baseRate()
                  << (rented ? " [Rented]" : " [Available]") << '\n';
    }
};

class Bicycle : public Vehicle {
public:
    Bicycle(std::string reg, double rate) : Vehicle(std::move(reg), rate) {}
    double rentalCharge() const override { return baseRate(); }
    std::string type() const override { return "Bicycle"; }
};

class TwoWheeler : public Vehicle {
    double fuelLitres;
    std::string serviceDue;
public:
    TwoWheeler(std::string reg, double rate, double fuel, std::string due)
        : Vehicle(std::move(reg), rate), fuelLitres(fuel), serviceDue(std::move(due)) {}
    double rentalCharge() const override { return baseRate() + fuelLitres * 5.0; }
    std::string type() const override { return "TwoWheeler"; }
};

class Car : public Vehicle {
    int seats;
    double fuelLitres;
    std::string serviceDue;
public:
    Car(std::string reg, double rate, int s, double fuel, std::string due)
        : Vehicle(std::move(reg), rate), seats(s), fuelLitres(fuel), serviceDue(std::move(due)) {}
    double rentalCharge() const override { return baseRate() + fuelLitres * 5.0 + seats * 10.0; }
    std::string type() const override { return "Car"; }
};

class ElectricCar : public Vehicle {
    int seats;
    double batteryKwh;
public:
    ElectricCar(std::string reg, double rate, int s, double battery)
        : Vehicle(std::move(reg), rate), seats(s), batteryKwh(battery) {}
    double rentalCharge() const override { return baseRate() + batteryKwh * 3.0 + seats * 10.0; }
    std::string type() const override { return "ElectricCar"; }
};

// Extension: new vehicle type is added without editing the existing classes.
class ElectricBus : public Vehicle {
    int seats;
    double batteryKwh;
public:
    ElectricBus(std::string reg, double rate, int s, double battery)
        : Vehicle(std::move(reg), rate), seats(s), batteryKwh(battery) {}
    double rentalCharge() const override { return baseRate() + batteryKwh * 3.0 + seats * 5.0; }
    std::string type() const override { return "ElectricBus"; }
};

int main() {
    std::vector<std::unique_ptr<Vehicle>> fleet;
    fleet.push_back(std::make_unique<Bicycle>("BI-101", 100));
    fleet.push_back(std::make_unique<TwoWheeler>("TW-202", 300, 2, "2026-12-01"));
    fleet.push_back(std::make_unique<Car>("CA-303", 1500, 5, 10, "2026-12-10"));
    fleet.push_back(std::make_unique<ElectricCar>("EV-404", 1800, 5, 40));
    fleet.push_back(std::make_unique<ElectricBus>("EB-505", 3500, 30, 100));

    for (const auto& v : fleet) v->print();

    for (const auto& v : fleet) {
        if (v->rent())
            std::cout << "Rented " << v->getRegistration() << '\n';
    }

    std::cout << "Return CA-303 with 2 late days: Rs "
              << fleet[2]->returnVehicle(2) << '\n';
    std::cout << "Return EB-505: Rs " << fleet[4]->returnVehicle() << '\n';

    for (const auto& v : fleet) v->print();
}
