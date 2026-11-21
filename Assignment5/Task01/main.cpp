#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>
class Bill {
protected: std::string patient;
public:
 explicit Bill(std::string p):patient(std::move(p)){}
 virtual ~Bill()=default;
 virtual double total() const=0;
 virtual std::string type() const=0;
 void print() const { std::cout<<type()<<' '<<patient<<": "<<total()<<'\\n'; }
};
class OutPatient:public Bill {
 double consult,tests;
public:
 OutPatient(std::string p,double c,double t):Bill(std::move(p)),consult(c),tests(t){if(c<0||t<0)throw std::invalid_argument("negative charge");}
 double total()const override{return consult+tests;}
 std::string type()const override{return "OP";}
};
class InPatient:public Bill {
 int days; double rate,surgery;
public:
 InPatient(std::string p,int d,double r,double s):Bill(std::move(p)),days(d),rate(r),surgery(s){if(d<0||r<0||s<0)throw std::invalid_argument("negative charge");}
 double total()const override{return days*rate+surgery;}
 std::string type()const override{return "IP";}
};
class DayCare:public Bill {
 double package;
public: DayCare(std::string p,double x):Bill(std::move(p)),package(x){if(x<0)throw std::invalid_argument("negative package");}
 double total()const override{return package;} std::string type()const override{return "DAYCARE";}
};
class Emergency:public Bill {
 double base,rate; int hours;
public: Emergency(std::string p,double b,double r,int h):Bill(std::move(p)),base(b),rate(r),hours(h){if(b<0||r<0||h<0)throw std::invalid_argument("negative charge");}
 double total()const override{return base+rate*hours;} std::string type()const override{return "EMERGENCY";}
};
class Payer {public: virtual double amountCovered(const Bill&)const=0; virtual ~Payer()=default;};
class SelfPay:public Payer {public: double amountCovered(const Bill&)const override{return 0;}};
class Insurance:public Payer {
 double limit, copay;
public: Insurance(double l,double c):limit(l),copay(c){if(l<0||c<0||c>100)throw std::invalid_argument("invalid policy");}
 double amountCovered(const Bill& b)const override{return std::min(b.total(),limit)*(1-copay/100);}
};
class Government:public Payer {
 double rate;
public: explicit Government(double r):rate(r){}
 double amountCovered(const Bill& b)const override{return std::min(b.total(),rate);}
};
int main(){
 try{
  std::vector<std::unique_ptr<Bill>> bills;
  bills.push_back(std::make_unique<OutPatient>("Ravi",500,1200));
  bills.push_back(std::make_unique<InPatient>("Meena",3,2500,40000));
  bills.push_back(std::make_unique<DayCare>("Arun",18000));
  bills.push_back(std::make_unique<Emergency>("Divya",1000,500,4));
  double sum=0; for(const auto& b:bills){b->print();sum+=b->total();}
  std::cout<<"Day total: "<<sum<<'\\n';
  Insurance ins(50000,10); SelfPay self; Government gov(15000);
  std::cout<<"Self-pay covered: "<<self.amountCovered(*bills[0])<<'\\n';
  std::cout<<"Insurance covered: "<<ins.amountCovered(*bills[0])<<'\\n';
  std::cout<<"Government covered: "<<gov.amountCovered(*bills[0])<<'\\n';
 }catch(const std::exception& e){std::cerr<<"Billing error: "<<e.what()<<'\\n';return 1;}
}
