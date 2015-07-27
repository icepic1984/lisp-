#include <cctype>
#include <iostream>
#include <string>
#include "sexpr.hpp"
#include "parser.hpp"
#include <boost/spirit/include/support_utree.hpp>
   
int main()
{

	auto t = tokenize("(defun hello (n) (if (= n 0) n (hello (- n 1))))(hello 10)");
	auto s = parse(t);
	std::cout << s[0] << std::endl;
	std::cout << atom(".9E+11")<<std::endl;
	std::cout << atom("100")<<std::endl;
	std::cout << atom("t") << std::endl;
	std::cout << atom("f") << std::endl;

	sexpr a;
	sexpr b(100.0);
	a.push_back(100.0);
	a.push_back(200.0);	
	std::cout<<visit(a,b,sexpr_add {})<<std::endl;
	                
	// using boost::spirit::utree;
	// utree a(100);
	// utree b(100);
	// utree::visit(a,b, add{});
	
	 // std::cout << val2 << std::endl;
	 // std::cout<< val2.get<std::string>()<<std::endl;
	// val.push_back(std::string("hallo"));

	



	
	// visit(s[0],[](auto t)
	//         {
	// 	        std::cout << t << std::endl;
	//         });
	
//	tokenize(std::string("  hallo"));
	// sexpr a;
	// a.push_back("hallo");
	// a.push_back("de");
	// a.push_back("s");
	// sexpr b;
	// b.push_back("erer");
	// sexpr c;
	// sexpr d ;
	// c.push_back("erere");
	// d.push_back("erer");
	// b.push_back(c);
	// b.push_back(d);
	// a.push_back(b);
	
	// sexpr e = std::move(d);
	// std::cout << a << std::endl;
	
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

// 	sexpr a(std::string("ballo"));
// 	sexpr b(std::string("bla"));
// 	sexpr c;
// 	c.push_back(std::string("test"));
// //	c.push_back(b);
// //	c.push_back(a);
	// sexpr d;
	
	// a = std::move(b);
	
	// std::cout << a << std::endl;
	// std::cout << b << std::endl;
	// std::cout << "ende" << std::endl;
	 // using boost::spirit::utree;
	 // utree val;
	 // utree val2(100);
	 // std::cout << val2 << std::endl;
	 // std::cout<< val2.get<std::string>()<<std::endl;
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
