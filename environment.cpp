#include <utility>
#include "environment.hpp"
#include "lisp.hpp"
environment::environment(environment* env): outer(env),
                                               buildin{{"+", &add},
	{"/", &divs},
	{"*", &mul},
	{"-", &sub},
	{"car",&car},
	{"cdr",&cdr},
	{"=",&equal},
	{"nth",&nth},
	{"eval",std::bind(evals,std::placeholders::_1,this)}}
	{}
	
environment::func_t environment::find_buildin(const std::string& name)
{
	auto f = buildin.find(name);
	if(f != buildin.end())
	   return f->second;
	if(outer)
	   return outer->find_buildin(name);
	throw std::invalid_argument("Unknown function: " + name);
}

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


