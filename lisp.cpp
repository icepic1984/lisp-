#include <numeric>
#include "lisp.hpp"
#include "sexpr.hpp"
#include "operators.hpp"

sexpr car(const std::vector<sexpr>& exprs)
{
	return visit(exprs[0],car_helper {});
}

sexpr cdr(const std::vector<sexpr>& exprs)
{
	return visit(exprs[0],cdr_helper {});
}

sexpr add(const std::vector<sexpr>& exprs){
	if(exprs.size() < 1)
	   throw std::invalid_argument("Wrong number of arguments");
	return std::accumulate(exprs.begin(),exprs.end(),sexpr(0),
	                       [](const sexpr& a, const sexpr& b) {
		                       return a + b;
	                       });
}

sexpr mul(const std::vector<sexpr>& exprs)
{
	if(exprs.size() < 1)
	   throw std::invalid_argument("Wrong number of arguments");
	return std::accumulate(exprs.begin(),exprs.end(),sexpr(1.0),
	                       [](const sexpr& a, const sexpr& b) {
		                       return a * b;
	                       });
}

sexpr divs(const std::vector<sexpr>& exprs)
{
	if(exprs.size() < 1)
	   throw std::invalid_argument("Wrong number of arguments");
	auto init = exprs.front();
	for(auto iter = exprs.begin() + 1; iter != exprs.end(); ++iter) {
		init = init / *iter;
	}
	return init;
}

sexpr sub(const std::vector<sexpr>& exprs)
{
	if(exprs.size() < 1)
	   throw std::invalid_argument("Wrong number of arguments");
	sexpr init(exprs.front());
	return std::accumulate(exprs.begin()+1,exprs.end(),init,
	                       [](const sexpr& a, const sexpr& b) {
		                       return a - b;
	                       });
}

sexpr equal(const std::vector<sexpr>& exprs)
{
	if(exprs.size() < 1)
	   throw std::invalid_argument("Wrong number of arguments");
	sexpr init(exprs.front());
	for(auto iter = exprs.begin()+1; iter != exprs.end();
	    ++iter){
		if(init != *iter)
		   return sexpr(false);
	}
	return sexpr(true);
}

sexpr eval(const sexpr& a, environment& env)
{
	return visit(a,eval_helper {}, env);
}

sexpr evals(const std::vector<sexpr>& a, environment& env)
{
	if(a.empty())
	   throw std::invalid_argument("Wrong number of arguments");
	
	for(auto iter = a.begin(); iter != a.end()-1;
	    ++iter){
		eval(*iter,env);
	}
	return eval(a.back(),env);
}

sexpr nth(const std::vector<sexpr>& a)
{
	if(a.size() < 2)
	   throw std::invalid_argument("nth: Wrong number of arguments");
	return visit(a[1],nth_helper {}, std::size_t(a[0].get<int>()));
}




	
