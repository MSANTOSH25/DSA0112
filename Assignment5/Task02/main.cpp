#include <iostream>
#include <stdexcept>
#include <string>
class ClaimError:public std::runtime_error{public:using std::runtime_error::runtime_error;};
double approve(double billed,double cover){
 if(billed<0||cover<0)throw std::invalid_argument("negative amount");
 if(billed>cover)throw ClaimError("exceeds cover"); return billed;
}
double settle(double billed,double cover){
 try{return approve(billed,cover);}
 catch(const ClaimError& e){std::cout<<"audit: "<<e.what()<<'\\n';throw;}
}
struct Guard{std::string name;explicit Guard(std::string n):name(std::move(n)){}~Guard()noexcept{std::cout<<"destroyed "<<name<<'\\n';}};
int main(){
 double cases[][2]={{40000,50000},{60000,50000},{-1,50000}};
 for(const auto& c:cases)try{Guard g("claim");std::cout<<"approved "<<settle(c[0],c[1])<<'\\n';}
 catch(const ClaimError&e){std::cout<<"claim rejected: "<<e.what()<<'\\n';}
 catch(const std::exception&e){std::cout<<"error: "<<e.what()<<'\\n';}
 try{throw std::bad_alloc();}catch(const std::bad_alloc&){std::cout<<"caught bad_alloc"<<'\\n';}
}
