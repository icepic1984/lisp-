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

   sexpr(){}

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
   	   switch(expr.type_field){
	   case sexpr_type::string_type:
		   new(&s)std::string(expr.s);
   	   	   break;
	   case sexpr_type::list_type:
		   new(&l) std::vector<sexpr>();
		   break;
	   case sexpr_type::double_type:
		   d = expr.d;
		   break;
	   case sexpr_type::integer_type:
		   i = expr.i;
		   break;
   	   }
   } 
   
   sexpr& operator=(const sexpr&) = delete;
   sexpr(sexpr&&) = delete;
   sexpr& operator=(sexpr&&) = delete;
   ~sexpr() {
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
		os <<expr.s;
		break;
	case sexpr_type::double_type:
		os <<expr.d;
		break;
	}

	return os;
}
	
int main()
{
	sexpr exp(100.0);

	sexpr f(exp);
	std::cout <<f << std::endl;
	using boost::spirit::utree;
	// utree val;
	// val.push_back(123);
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
