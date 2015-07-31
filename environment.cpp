#include "environment.hpp"
#include "lisp.hpp"

environment::environment(): map
      {{"+", &add},
	   {"/", &divs},
	   {"*", &mul},
	   {"-", &sub},
	   {"car",&car},
	   {"cdr",&cdr},
	   {"=",&equal},
	   {"nth",&nth},
	   {"evals",&evals}}{}
	
environment::func_t environment::find(const std::string& name)
{
	auto f = map.find(name);
	if(f == map.end())
	   throw std::invalid_argument("Unknown function: "+name);
	return f->second;
}

