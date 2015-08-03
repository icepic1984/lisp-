#include <utility>
#include "environment.hpp"
#include "lisp.hpp"
environment::environment(environment* env):
	outer(env), symbols{
	{"+", sexpr(sexpr::func_t(add))},
	{"/", sexpr(sexpr::func_t(divs))},
	{"*", sexpr(sexpr::func_t(mul))},
	{"-", sexpr(sexpr::func_t(sub))},
	{"car",sexpr(sexpr::func_t(car))},
	{"cdr",sexpr(sexpr::func_t(cdr))},
	{"=",sexpr(sexpr::func_t(equal))},
	{"nth",sexpr(sexpr::func_t(nth))},
	{"eval",sexpr(sexpr::func_t(std::bind(evals,std::placeholders::_1,this)))}}
	{}
	
environment::result_t environment::find_symbol(const std::string& name)
{
	auto s = symbols.find(name);
	if(s != symbols.end())
	   return s->second;
	if(outer)
	   return outer->find_symbol(name);
	return boost::none;
}

void environment::set_symbol(const std::string& name, const sexpr& expr)
{symbols.insert(std::make_pair(name,expr));}

std::ostream& operator<<(std::ostream& os, const environment& env)
{
	for(auto iter : env.symbols){
		os << iter.first << " = " <<iter.second;
	}
	if(env.outer)
	   os << "\t"<< *(env.outer);
	
	return os;
}


