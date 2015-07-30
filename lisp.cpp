#include <numeric>
#include "lisp.hpp"
#include "sexpr.hpp"

sexpr car(const sexpr& exp)
{return visit(exp,car_helper{});}

sexpr cdr(const sexpr& exp)
{return visit(exp,cdr_helper{});}

sexpr add(const sexpr& expr)
{
	if(expr == sexpr(sexpr::nil_type{}))
	   throw std::invalid_argument("Wrong number of arguments");
	auto tmp = expr.get<std::vector<sexpr>>();
	if(tmp.empty())
		throw std::invalid_argument("Wrong number of arguments");
	return std::accumulate(tmp.begin(),tmp.end(),sexpr(0),
	                       [](const sexpr& a, const sexpr& b) {
		                       return a + b;
	                       });
}

sexpr mul(const sexpr& expr)
{
	if(expr == sexpr(sexpr::nil_type{}))
		throw std::invalid_argument("Wrong number of arguments");
	auto tmp = expr.get<std::vector<sexpr>>();
	if(tmp.empty())
		throw std::invalid_argument("Wrong number of arguments");
	return std::accumulate(tmp.begin(),tmp.end(),sexpr(1.0),
	                       [](const sexpr& a, const sexpr& b) {
		                       return a * b;
	                       });
}

sexpr sub(const sexpr& expr)
{
	if(expr == sexpr(sexpr::nil_type{}))
		throw std::invalid_argument("Wrong number of arguments");
	auto tmp = expr.get<std::vector<sexpr>>();
	if(tmp.empty())
		throw std::invalid_argument("Wrong number of arguments");
	sexpr init(tmp.front());
	return std::accumulate(tmp.begin()+1,tmp.end(),init,
	                       [](const sexpr& a, const sexpr& b) {
		                       return a - b;
	                       });
}
	
sexpr divs(const sexpr& expr)
{
	if(expr == sexpr(sexpr::nil_type{}))
		throw std::invalid_argument("Wrong number of arguments");
	auto tmp = expr.get<std::vector<sexpr>>();
	if(tmp.empty())
		throw std::invalid_argument("Wrong number of arguments");
	auto init = tmp.front();
	for(auto iter = tmp.begin() + 1; iter != tmp.end(); ++iter) {
		init = init / *iter;
	}
	return init;
}

