#include <cctype>
#include <iostream>
#include <string>
#include "sexpr.hpp"
#include <functional>
#include "parser.hpp"
#include "lisp.hpp"
#include <unordered_map>
#include <vector>
#include <memory>
#include "environment.hpp"

//#include <boost/spirit/include/support_utree.hpp>
void repl(const std::string & prompt, environment * env)
{
    for (;;) {
        std::cout << prompt;
        std::string line;
        std::getline(std::cin, line);
        try {
	        std::cout << evals(parse(tokenize(line)), env) << '\n';
        } catch (std::invalid_argument& e){
	        std::cout << e.what() << std::endl;
        }
    }
}

int main()
{


	// auto t = tokenize("(define countdown (lambda (n) (cond ((= n 0) n) (t (countdown(- n 1))))))(countdown 10)");

	auto env = std::make_unique<environment>();

	 // auto t = tokenize("(define count (lambda (n) (lambda () (set n (- n 1)))))\
     //                     (define count-3 (count 3)) \
     //                     (define count-4 (count 4)) \
     //                     (count-3)(count-3)(count-4)(count-4)");

	 // auto t = tokenize("(define count (lambda (n) (lambda () (set n (- n 1)))))\
     //                     (define count-3 (count 3))	  \
     //                     ");


	//auto t = tokenize("((lambda (x y) (+ x y)) 10 20)");
	// auto s = parse(t);
	// std::cout<<evals(s,env.get())<<std::endl;
	// std::cout << *env << std::endl;
	repl("LISP> ",env.get());
	
	
	// using boost::spirit::utree;
	// utree a(100);
	// utree b(100);
	// utree::visit(a,b, add{});
	
	 // std::cout << val2 << std::endl;c
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
