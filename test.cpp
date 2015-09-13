#define BOOST_TEST_MODULE lisp_command

#include <iostream>
#include <boost/test/unit_test.hpp>
#include "parser.hpp"
#include "sexpr.hpp"
#include "lisp.hpp"
#include "environment.hpp"

//http://www.schemers.org/Documents/Standards/R5RS/HTML/
BOOST_AUTO_TEST_CASE(cons_test)
{
	// (cons 'a '()) => (a)
	auto env = std::make_unique<environment>();
	auto expr = evals(parse(tokenize("(cons (quote a) (quote()))")),env.get());
	auto l = expr.get<std::vector<sexpr>>();
	BOOST_CHECK_EQUAL(l.size(),1);
	std::stringstream buffer;
	buffer << expr;
	BOOST_CHECK_EQUAL(buffer.str(),"( a ) ");

	// (cons '(a) '(b c d)) => ((a) b c d)
	expr = evals(parse(tokenize("(cons (quote(a))  (quote(b c d))) ")),env.get());
	l = expr.get<std::vector<sexpr>>();
	BOOST_CHECK_EQUAL(l.size(),4);
	buffer.str(std::string());
	buffer.clear();
	buffer << expr;
	BOOST_CHECK_EQUAL(buffer.str(),"( ( a ) b c d ) ");

	// (cons "a" '(b c)) => ("a" b c)
	expr = evals(parse(tokenize("(cons \"a\"  (quote(b c))) ")),env.get());
	buffer.str(std::string());
	buffer.clear();
	buffer << expr;
	BOOST_CHECK_EQUAL(buffer.str(),"( \"a\" b c ) ");

	// (cons 'a 3) => (a 3)
	expr = evals(parse(tokenize("(cons (quote a) 3)")),env.get());
	buffer.str(std::string());
	buffer.clear();
	buffer << expr;
	BOOST_CHECK_EQUAL(buffer.str(),"( a 3 ) ");

	// (cons '(a b) 'c) => ( ( a b ) c)
	expr = evals(parse(tokenize("(cons (quote(a b)) (quote c))")),env.get());
	buffer.str(std::string());
	buffer.clear();
	buffer << expr;
	BOOST_CHECK_EQUAL(buffer.str(),"( ( a b ) c ) ");
}


BOOST_AUTO_TEST_CASE(cdr_test)
{
	// (cdr '((a) b c d)) => (b c d)
	auto env = std::make_unique<environment>();
	auto expr = evals(parse(tokenize("(cdr(quote((a) b c d)))")),env.get());
	std::stringstream buffer;
	buffer << expr;
	BOOST_CHECK_EQUAL(buffer.str(),"( b c d ) ");

	//(cdr '(( 1 2))) => (2) NOTE: no cons cell support
	expr = evals(parse(tokenize("(cdr (quote( 1 2)))")),env.get());
	buffer.str(std::string());
	buffer.clear();
	buffer << expr;
	BOOST_CHECK_EQUAL(buffer.str(),"( 2 ) ");

	//(cdr '()) => nil 
	expr = evals(parse(tokenize("(cdr (quote()))")),env.get());
	buffer.str(std::string());
	buffer.clear();
	buffer << expr;
	BOOST_CHECK_EQUAL(buffer.str(),"<nil>");
}

BOOST_AUTO_TEST_CASE(car_test)
{
	// (car '(a b c)) => a 
	auto env = std::make_unique<environment>();
	auto expr = evals(parse(tokenize("(car(quote(a b c d)))")),env.get());
	std::stringstream buffer;
	buffer << expr;
	BOOST_CHECK_EQUAL(buffer.str(),"a ");

	//(car '()) => <nil> 
	expr = evals(parse(tokenize("(car (quote()))")),env.get());
	buffer.str(std::string());
	buffer.clear();
	buffer << expr;
	BOOST_CHECK_EQUAL(buffer.str(),"<nil>");

	//(car '((a b) c)) => (a b)
	expr = evals(parse(tokenize("(car (quote((a b) c)))")),env.get());
	buffer.str(std::string());
	buffer.clear();
	buffer << expr;
	BOOST_CHECK_EQUAL(buffer.str(),"( a b ) ");

}

BOOST_AUTO_TEST_CASE(compare_test)
{
	auto env = std::make_unique<environment>();
	auto expr = evals(parse(tokenize("(= nil nil)")),env.get());
	BOOST_CHECK_EQUAL(expr.get<bool>(),true);

	expr = evals(parse(tokenize("(= \"aa\" \"aa\")")),env.get());
	BOOST_CHECK_EQUAL(expr.get<bool>(),true);

	expr = evals(parse(tokenize("(= \"aa\" \"a\")")),env.get());
	BOOST_CHECK_EQUAL(expr.get<bool>(),false);
	
	expr = evals(parse(tokenize("(= t t)")),env.get());
	BOOST_CHECK_EQUAL(expr.get<bool>(),true);

	expr = evals(parse(tokenize("(= t f)")),env.get());
	BOOST_CHECK_EQUAL(expr.get<bool>(),false);

	expr = evals(parse(tokenize("(= nil (cdr (quote(10))))")),env.get());
	BOOST_CHECK_EQUAL(expr.get<bool>(),true);

	expr = evals(parse(tokenize("(= nil (cdr (quote(10 20))))")),env.get());
	BOOST_CHECK_EQUAL(expr.get<bool>(),false);

	expr = evals(parse(tokenize("(= 1 2 3)")),env.get());
	BOOST_CHECK_EQUAL(expr.get<bool>(),false) ;

	expr = evals(parse(tokenize("(= 1 1 1)")),env.get());
	BOOST_CHECK_EQUAL(expr.get<bool>(),true) ;

	expr = evals(parse(tokenize("(> 3 4)")),env.get());
	BOOST_CHECK_EQUAL(expr.get<bool>(),false) ;

	expr = evals(parse(tokenize("(> 4 4)")),env.get());
	BOOST_CHECK_EQUAL(expr.get<bool>(),false) ;

	expr = evals(parse(tokenize("(> 4 3)")),env.get());
	BOOST_CHECK_EQUAL(expr.get<bool>(),true) ;

	expr = evals(parse(tokenize("(> 4 3 2)")),env.get());
	BOOST_CHECK_EQUAL(expr.get<bool>(),true) ;

	expr = evals(parse(tokenize("(> 4 3 5)")),env.get());
	BOOST_CHECK_EQUAL(expr.get<bool>(),false) ;

	expr = evals(parse(tokenize("(< 3 4)")),env.get());
	BOOST_CHECK_EQUAL(expr.get<bool>(),true) ;

	expr = evals(parse(tokenize("(< 4 4)")),env.get());
	BOOST_CHECK_EQUAL(expr.get<bool>(),false) ;

	expr = evals(parse(tokenize("(< 4 3)")),env.get());
	BOOST_CHECK_EQUAL(expr.get<bool>(),false) ;

	expr = evals(parse(tokenize("(< 3 2 1)")),env.get());
	BOOST_CHECK_EQUAL(expr.get<bool>(),false) ;

	expr = evals(parse(tokenize("(< 1 2 3)")),env.get());
	BOOST_CHECK_EQUAL(expr.get<bool>(),true) ;

	expr = evals(parse(tokenize("(< 1 2 0)")),env.get());
	BOOST_CHECK_EQUAL(expr.get<bool>(),false) ;

	expr = evals(parse(tokenize("(<= 1 2)")),env.get());
	BOOST_CHECK_EQUAL(expr.get<bool>(),true) ;

	expr = evals(parse(tokenize("(<= 2 2)")),env.get());
	BOOST_CHECK_EQUAL(expr.get<bool>(),true) ;

	expr = evals(parse(tokenize("(<= 3 2)")),env.get());
	BOOST_CHECK_EQUAL(expr.get<bool>(),false) ;

	expr = evals(parse(tokenize("(<= 1 2 3)")),env.get());
	BOOST_CHECK_EQUAL(expr.get<bool>(),true) ;

	expr = evals(parse(tokenize("(<= 1 2 2)")),env.get());
	BOOST_CHECK_EQUAL(expr.get<bool>(),true) ;

	expr = evals(parse(tokenize("(<= 1 2 1)")),env.get());
	BOOST_CHECK_EQUAL(expr.get<bool>(),false) ;

	expr = evals(parse(tokenize("(>= 2 1)")),env.get());
	BOOST_CHECK_EQUAL(expr.get<bool>(),true) ;

	expr = evals(parse(tokenize("(>= 2 2)")),env.get());
	BOOST_CHECK_EQUAL(expr.get<bool>(),true) ;

	expr = evals(parse(tokenize("(>= 2 3)")),env.get());
	BOOST_CHECK_EQUAL(expr.get<bool>(),false) ;

	expr = evals(parse(tokenize("(>= 3 2 1)")),env.get());
	BOOST_CHECK_EQUAL(expr.get<bool>(),true) ;

	expr = evals(parse(tokenize("(>= 2 1 1)")),env.get());
	BOOST_CHECK_EQUAL(expr.get<bool>(),true) ;

	expr = evals(parse(tokenize("(>= 2 1 2)")),env.get());
	BOOST_CHECK_EQUAL(expr.get<bool>(),false) ;
}

BOOST_AUTO_TEST_CASE(arithmetric_test)
{

	auto env = std::make_unique<environment>();
	auto expr = evals(parse(tokenize("(+ 3 2 10)")),env.get());
	BOOST_CHECK_EQUAL(expr.get<int>(),15);

	expr = evals(parse(tokenize("(- 3 2 10)")),env.get());
	BOOST_CHECK_EQUAL(expr.get<int>(),-9);

	expr = evals(parse(tokenize("(* 3 2 10)")),env.get());
	BOOST_CHECK_EQUAL(expr.get<int>(),60);

	expr = evals(parse(tokenize("(/ 100 10 2)")),env.get());
	BOOST_CHECK_EQUAL(expr.get<int>(),5);
}


BOOST_AUTO_TEST_CASE(begin_test)
{
	auto env = std::make_unique<environment>();
	auto expr = evals(parse(tokenize("(begin (quote a) (quote b))")),env.get());
	std::stringstream buffer;
	buffer << expr;
	BOOST_CHECK_EQUAL(buffer.str(), "b ");
}

BOOST_AUTO_TEST_CASE(define_test)
{
	auto env = std::make_unique<environment>();
	auto expr = evals(parse(tokenize("(define x 100)")),env.get());
	BOOST_CHECK_EQUAL(expr.get<int>(),100);
}

// https://stackoverflow.com/questions/526082/in-scheme-whats-the-point-of-set
BOOST_AUTO_TEST_CASE(set_test)
{
	auto env = std::make_unique<environment>();
	auto expr = evals(parse(tokenize("(define x 100)")),env.get());
	BOOST_CHECK_EQUAL(expr.get<int>(),100);
	expr = evals(parse(tokenize("(define foo (lambda () (define x 120) x))")),env.get());
	expr = evals(parse(tokenize("(foo)")),env.get());
	BOOST_CHECK_EQUAL(expr.get<int>(),120);
	expr = evals(parse(tokenize("(define bar (lambda () (set x 4) x))")),env.get());
	expr = evals(parse(tokenize("x")),env.get());
	BOOST_CHECK_EQUAL(expr.get<int>(),100);
	expr = evals(parse(tokenize("(bar)")),env.get());
	BOOST_CHECK_EQUAL(expr.get<int>(),4);
	expr = evals(parse(tokenize("x")),env.get());
	BOOST_CHECK_EQUAL(expr.get<int>(),4);
}

BOOST_AUTO_TEST_CASE(cond)
{
	 auto env = std::make_unique<environment>();
	 auto expr = evals(parse(tokenize("(cond ((> 3 2) (quote greater)) \
                                             ((< 3 2) (quote less)))")),env.get());
	 std::stringstream buffer;
	 buffer << expr;
	 BOOST_CHECK_EQUAL(buffer.str(),"greater ");

	 expr = evals(parse(tokenize("(cond ((< 3 2) (quote less)) \
                                        ((> 3 2) (quote greater)))")),env.get());
	 buffer.str(std::string());
	 buffer.clear();
	 buffer << expr;
	 BOOST_CHECK_EQUAL(buffer.str(),"greater ");

	 expr = evals(parse(tokenize("(cond (f 1) \
                                 (f 2) \
                                 (f 3) \
                                 (t 4) \
                                 (f 5))")),env.get());
	 BOOST_CHECK_EQUAL(expr.get<int>(),4);
}
	
BOOST_AUTO_TEST_CASE(lambda_test)
{
	auto env = std::make_unique<environment>();
	auto expr = evals(parse(tokenize("(define plus (lambda (x y) (+ x y)))")),
	                  env.get());
	expr = evals(parse(tokenize("(plus 20 20)")), env.get());
	BOOST_CHECK_EQUAL(expr.get<int>(),40);
	// Test recursion

	expr = evals(parse(tokenize("(define countdown (lambda (n)\
	                                (cond ((= n 0) n)\
		                                   (t (countdown(- n 1))))))")), env.get());
	expr = evals(parse(tokenize("(countdown 10)")), env.get());
	BOOST_CHECK_EQUAL(expr.get<int>(),0);
}

BOOST_AUTO_TEST_CASE(lexicalscope_test)
{
	auto env = std::make_unique<environment>();
	auto expr = evals(parse(tokenize("(define count (lambda (n) (lambda () (set n (- n 1)))))")),env.get());
	evals(parse(tokenize("(define count-3 (count 3))")),env.get());
	evals(parse(tokenize("(define count-4 (count 4))")),env.get());

	expr = evals(parse(tokenize("(count-3)")),env.get());
	BOOST_CHECK_EQUAL(expr.get<int>(),2);
	expr = evals(parse(tokenize("(count-3)")),env.get());
	BOOST_CHECK_EQUAL(expr.get<int>(),1);
	
	expr = evals(parse(tokenize("(count-4)")),env.get());
	BOOST_CHECK_EQUAL(expr.get<int>(),3);
	expr = evals(parse(tokenize("(count-4)")),env.get());
	BOOST_CHECK_EQUAL(expr.get<int>(),2);


	evals(parse(tokenize("(define set-hidden 0)")),env.get());
	evals(parse(tokenize("(define get-hidden 0)")),env.get());
	evals(parse(tokenize("((lambda () (begin (define hidden 0)\
                                      (set set-hidden (lambda (n) (set hidden n)))\
                                      (set get-hidden (lambda () hidden)))))")),env.get());

	expr = evals(parse(tokenize("(get-hidden)")),env.get());
	BOOST_CHECK_EQUAL(expr.get<int>(),0);
	evals(parse(tokenize("(set-hidden 100)")),env.get());
	expr = evals(parse(tokenize("(get-hidden)")),env.get());
	BOOST_CHECK_EQUAL(expr.get<int>(),100);
	BOOST_CHECK_THROW(evals(parse(tokenize("hidden")),env.get()),
	                  std::invalid_argument);
}







