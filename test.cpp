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

	expr = evals(parse(tokenize("(= t t)")),env.get());
	BOOST_CHECK_EQUAL(expr.get<bool>(),true);

	expr = evals(parse(tokenize("(= t f)")),env.get());
	BOOST_CHECK_EQUAL(expr.get<bool>(),false);

	expr = evals(parse(tokenize("(= nil (cdr (quote(10))))")),env.get());
	BOOST_CHECK_EQUAL(expr.get<bool>(),true);

	expr = evals(parse(tokenize("(= nil (cdr (quote(10 20))))")),env.get());
	BOOST_CHECK_EQUAL(expr.get<bool>(),false);

}

// (cons 'a '(b c))
// (cons '(a) '(b c))
// (cons '(b c) 'a)
