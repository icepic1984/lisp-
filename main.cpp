#include <iostream>
#include <string>
#include <boost/spirit/include/support_utree.hpp>

struct sexpr_type
{
   enum info
   {
	   integer_type,
	   string_type,
	   double_type,
	   list_type,
	   nil_type,
	   invalid_type
   };
};

class sexpr 
{
public:
   struct invalid_type {};
   struct nil_type {};

   sexpr(){
	   set_type(sexpr_type::invalid_type);
   }

   sexpr(nil_type){
	   set_type(sexpr_type::nil_type);
   }
   
   sexpr(invalid_type){
	   set_type(sexpr_type::invalid_type);
   }

   sexpr(int i_) : i(i_){
	   set_type(sexpr_type::integer_type);
   }

   sexpr(double d_) : d(d_){
	   set_type(sexpr_type::double_type);
   }

   sexpr(const std::string& s_) {
	   new(&s)std::string(s_);
	   set_type(sexpr_type::string_type);
   }
   
   sexpr(const sexpr& expr) :
   	   type_field(expr.type_field) {
	   std::cout << "copy" << std::endl;
   	   switch(expr.type_field){
	   case sexpr_type::string_type:
		   new(&s)std::string(expr.s);
   	   	   break;
	   case sexpr_type::list_type:
		   new(&l) std::vector<sexpr>(expr.l);
		   break;
	   case sexpr_type::double_type:
		   d = expr.d;
		   break;
	   case sexpr_type::integer_type:
		   i = expr.i;
		   break;
   	   }
   }
   sexpr& operator=(sexpr&& other){
	   std::cout << "assign move" << std::endl;
	   this->~sexpr();
	   type_field = std::move(other.type_field);
	   switch(other.type_field){
	   case sexpr_type::integer_type:
		   i = std::move(other.i);
		   break;
	   case sexpr_type::double_type:
		   d = std::move(other.d);
		   break;
	   case sexpr_type::string_type:
		   new(&s) std::string(std::move(other.s));
		   break;
	   case sexpr_type::list_type:
		   new(&l) std::vector<sexpr>(std::move(other.l));
		   break;
	   }
	   other.type_field = sexpr_type::invalid_type;
	   return *this;
   }

   sexpr& operator=(const sexpr& other){
	   sexpr t(other);
	   std::swap(*this,t);
	   return *this;
   }
  
   sexpr(sexpr&& other) noexcept{
	   std::cout << "move" << std::endl;
	   *this = std::move(other);
   }
   
   ~sexpr() {
	   std::cout << "dest" << std::endl;
	   using string_type = std::string;
	   using list_type = std::vector<sexpr>;
	   switch(type_field){
	   case sexpr_type::string_type:
		    s.~string_type();
		    break;
	   case sexpr_type::list_type:
		   l.~list_type();
		   break;
	   }
   }
   template <typename T>
   void push_back(const T& t){
	   if (type_field == sexpr_type::invalid_type){
		   set_type(sexpr_type::list_type);
		   new(&l) std::vector<sexpr>();
		   l.emplace_back(t); 
	   } else if (type_field == sexpr_type::list_type) {
		   l.emplace_back(t);
	   } else {
		   throw std::invalid_argument("wrong type");
	   }
   }
 
   friend std::ostream& operator<<(std::ostream&os, const sexpr& expr);
   
private:
   void set_type(sexpr_type::info type)
      {type_field = type;}
   
   int type_field;
   union 
   {
      int i;
      std::string s;
      double d;
      std::vector<sexpr> l;
   };
};


std::ostream& operator<<(std::ostream& os, const sexpr& expr)
{
	switch(expr.type_field){
	case sexpr_type::nil_type:
		os <<"<nil>";
		break;
	case sexpr_type::invalid_type:
		os <<"<invalid>";
		break;
	case sexpr_type::integer_type:
		os <<expr.i;
		break;
	case sexpr_type::string_type:
		os << "\"" <<expr.s<<"\"";
		break;
	case sexpr_type::double_type:
		os <<expr.d;
		break;
	case sexpr_type::list_type:
		os << "( ";
		for(auto &iter : expr.l){
			os << iter << " ";
		}
		os << ")";
		break;
	}
	return os;
}
	
int main()
{
	// sexpr a;
	// sexpr b;
	// b.push_back(10);
	// b.push_back(1000);
	// b.push_back(std::string("fucj"));
	// a.push_back(std::string("test"));
	// a.push_back(std::string("abla"));
	// b.push_back(a);

	// sexpr test = std::move(b);
	// std::cout << test << std::endl;	
	// std::cout << b << std::endl;

	sexpr a(std::string("ballo"));
	sexpr b(std::string("bla"));
	sexpr c;
	c.push_back(std::string("test"));
//	c.push_back(b);
//	c.push_back(a);
	sexpr d;
	
	a = std::move(b);
	
	std::cout << a << std::endl;
	std::cout << b << std::endl;
	std::cout << "ende" << std::endl;
	// using boost::spirit::utree;
	// utree val;
	// utree val2(100);
	// std::cout << val << std::endl;
	// val.push_back(std::string("hallo"));
	// val.push_back(100);
	// val.push_back(100);
	// val.push_back(val2);
	// std::cout << val<< std::endl;
	// std::cout << val2 << std::endl;
	// val.push_back("Chuckie");
	// utree val2;
	// val2.push_back(123.456);
	// val2.push_back("Mah Doggie");
	// val.push_back(val2);
	// std::cout << val << std::endl;
	// utree val3(utree::invalid_type{});
	// utree val4;
	// val4.push_back(val3);
	
	// std::cout << val3 << std::endl;
	// std::cout << val4 << std::endl;
}
