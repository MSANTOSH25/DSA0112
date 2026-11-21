#include <algorithm>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>
class Bill{public:virtual double total()const=0;virtual std::string type()const=0;virtual ~Bill()=default;};
class OP:public Bill{double c,t;public:OP(double x,double y):c(x),t(y){}double total()const override{return c+t;}std::string type()const override{return "OP";}};
class IP:public Bill{int d;double r,s;public:IP(int x,double y,double z):d(x),r(y),s(z){}double total()const override{return d*r+s;}std::string type()const override{return "IP";}};
// New bill type
class MaternityBill:public Bill{double package,medicines;public:MaternityBill(double p,double m):package(p),medicines(m){}double total()const override{return package+medicines;}std::string type()const override{return "MATERNITY";}};
class Payer{public:virtual double covered(double amount)const=0;virtual ~Payer()=default;};
class Insurance:public Payer{double limit;public:explicit Insurance(double x):limit(x){}double covered(double a)const override{return std::min(a,limit);}};
// New payer type
class CorporatePayer:public Payer{double percent;public:explicit CorporatePayer(double p):percent(p){}double covered(double a)const override{return a*percent/100.0;}};
int main(){std::vector<std::unique_ptr<Bill>>b;b.push_back(std::make_unique<OP>(500,1200));b.push_back(std::make_unique<IP>(3,2500,40000));b.push_back(std::make_unique<MaternityBill>(60000,5000));for(auto&x:b)std::cout<<x->type()<<" total="<<x->total()<<'\\n';CorporatePayer corp(80);Insurance ins(50000);std::cout<<"Corporate covers maternity: "<<corp.covered(b.back()->total())<<'\\n'<<"Insurance covers maternity: "<<ins.covered(b.back()->total())<<'\\n';}
