#include <chrono>
#include <iostream>
#include <memory>
#include <type_traits>
#include <variant>
#include <vector>
struct Rec{int type;double a,b;int days;};
struct OP{double c,t;}; struct IP{int d;double r,s;}; struct DC{double p;};
using Var=std::variant<OP,IP,DC>;
struct Base{virtual double total()const=0;virtual ~Base()=default;};
struct VOP:Base{double a,b;VOP(double x,double y):a(x),b(y){}double total()const override{return a+b;}};
struct VIP:Base{int d;double r,s;VIP(int x,double y,double z):d(x),r(y),s(z){}double total()const override{return d*r+s;}};
struct VDC:Base{double p;explicit VDC(double x):p(x){}double total()const override{return p;}};
double sw(const Rec&x){switch(x.type){case 0:return x.a+x.b;case 1:return x.days*x.a+x.b;default:return x.a;}}
double vis(const Var&v){return std::visit([](const auto&x)->double{using T=std::decay_t<decltype(x)>;if constexpr(std::is_same_v<T,OP>)return x.c+x.t;else if constexpr(std::is_same_v<T,IP>)return x.d*x.r+x.s;else return x.p;},v);}
template<class F> double run(const char*n,std::size_t count,F f){auto a=std::chrono::steady_clock::now();double s=f();auto b=std::chrono::steady_clock::now();std::cout<<n<<" total="<<s<<" ms="<<std::chrono::duration<double,std::milli>(b-a).count()<<'\\n';return s;}
int main(int argc,char**argv){std::size_t n=argc>1?std::stoull(argv[1]):1000000;std::vector<Rec>r;r.reserve(n);std::vector<std::unique_ptr<Base>>v;v.reserve(n);std::vector<Var>x;x.reserve(n);
 for(std::size_t i=0;i<n;++i){int t=i%3;if(t==0){r.push_back({0,500,1200,0});v.push_back(std::make_unique<VOP>(500,1200));x.push_back(OP{500,1200});}else if(t==1){r.push_back({1,2500,40000,3});v.push_back(std::make_unique<VIP>(3,2500,40000));x.push_back(IP{3,2500,40000});}else{r.push_back({2,18000,0,0});v.push_back(std::make_unique<VDC>(18000));x.push_back(DC{18000});}}
 double a=run("switch",n,[&]{double s=0;for(auto&q:r)s+=sw(q);return s;});
 double b=run("virtual",n,[&]{double s=0;for(auto&q:v)s+=q->total();return s;});
 double c=run("variant",n,[&]{double s=0;for(auto&q:x)s+=vis(q);return s;});
 std::cout<<"totals_match="<<((a==b&&b==c)?"yes":"no")<<'\\n';
}
