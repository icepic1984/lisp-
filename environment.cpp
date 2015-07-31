#include <utility>
#include "environment.hpp"
#include "lisp.hpp"
environment::environment(): buildin
    {{"+", &add},
	{"/", &divs},
	{"*", &mul},
	{"-", &sub},
	{"car",&car},
	{"cdr",&cdr},
	{"=",&equal},
	{"nth",&nth},
	{"eval",std::bind(evals,std::placeholders::_1,std::ref(*this))}} {
}
	
environment::func_t environment::find_buildin(const std::string& name)
{
	auto f = buildin.find(name);
	if(f == buildin.end())
	   throw std::invalid_argument("Unknown function: " + name);
	return f->second;
}

environment::result_t environment::find_symbol(const std::string& name)
{
	auto s = symbols.find(name);
	if(s == symbols.end())
	   return boost::none;
	return s->second;
}

void environment::set_symbol(const std::string& name, const sexpr& expr)
{symbols.insert(std::make_pair(name,expr));}
