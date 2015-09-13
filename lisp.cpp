#include <numeric>
#include <iostream>
#include <fstream>
#include <regex>
#include "lisp.hpp"
#include "sexpr.hpp"
#include "operators.hpp"
#include "environment.hpp"
#include "parser.hpp"

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
	   throw std::invalid_argument("<add>: Wrong number of arguments");
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
	   throw std::invalid_argument("<divs>: Wrong number of arguments");
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
	   throw std::invalid_argument("<equal>: Wrong number of arguments");
	sexpr init(exprs.front());
	for(auto iter = exprs.begin()+1; iter != exprs.end();
	    ++iter){
		if(init != *iter)
		   return sexpr(false);
		init = *iter;
	}
	return sexpr(true);
}

sexpr less(const std::vector<sexpr>& exprs)
{
	if(exprs.size() < 1)
	   throw std::invalid_argument("<equal>: Wrong number of arguments");
	sexpr init(exprs.front());
	for(auto iter = exprs.begin()+1; iter != exprs.end();
	    ++iter){
		if(init >= *iter)
		   return sexpr(false);
		init = *iter;
	}
	return sexpr(true);
}

sexpr greater(const std::vector<sexpr>& exprs)
{
	if(exprs.size() < 1)
	   throw std::invalid_argument("<equal>: Wrong number of arguments");
	sexpr init(exprs.front());
	for(auto iter = exprs.begin()+1; iter != exprs.end();
	    ++iter){
		if(init <= *iter)
		   return sexpr(false);
		init = *iter;
	}
	return sexpr(true);
}

sexpr greater_equal(const std::vector<sexpr>& exprs)
{
	if(exprs.size() < 1)
	   throw std::invalid_argument("<equal>: Wrong number of arguments");
	sexpr init(exprs.front());
	for(auto iter = exprs.begin()+1; iter != exprs.end();
	    ++iter){
		if(init < *iter)
		   return sexpr(false);
		init = *iter;
	}
	return sexpr(true);
}

sexpr less_equal(const std::vector<sexpr>& exprs)
{
	if(exprs.size() < 1)
	   throw std::invalid_argument("<equal>: Wrong number of arguments");
	sexpr init(exprs.front());
	for(auto iter = exprs.begin()+1; iter != exprs.end();
	    ++iter){
		if(init > *iter)
		   return sexpr(false);
		init = *iter;
	}
	return sexpr(true);
}

sexpr eval(const sexpr& a, environment* env)
{
	return visit(a,eval_helper {}, env);
}

sexpr evals(const std::vector<sexpr>& a, environment* env)
{
	if(a.empty())
	   throw std::invalid_argument("<evals>: Wrong number of arguments");
	
	for(auto iter = a.begin(); iter != a.end()-1;
	    ++iter){
		eval(*iter,env);
	}
	return eval(a.back(),env);
}

sexpr cons(const std::vector<sexpr>& a) 
{
	if(a.size() != 2)
	   throw std::invalid_argument("<cons>: Wrong number of arguments");
	sexpr tmp;
	if((a[0].get_type() != sexpr_type::list_type) &&
	   (a[1].get_type() != sexpr_type::list_type)) {
		tmp.push_back(a[0]);
		tmp.push_back(a[1]);
	}else if((a[0].get_type() != sexpr_type::list_type) &&
	         (a[1].get_type() == sexpr_type::list_type)) {
		tmp.push_back(a[0]);
		auto lst = a[1].get<std::vector<sexpr>>();
		for(auto &iter : lst){
			tmp.push_back(iter);
		}
	} else if((a[0].get_type() == sexpr_type::list_type) &&
	          (a[1].get_type() != sexpr_type::list_type)) {
		tmp.push_back(a[0]);
		tmp.push_back(a[1]);
	} else {
	   tmp.push_back(a[0]);
	   auto lst = a[1].get<std::vector<sexpr>>();
	   for(auto &iter : lst){
		   tmp.push_back(iter);
	   }
	}
	return tmp;
}

sexpr nth(const std::vector<sexpr>& a)
{
	if(a.size() < 2)
	   throw std::invalid_argument("<nth>: Wrong number of arguments");
	return visit(a[1],nth_helper {}, std::size_t(a[0].get<int>()));
}


sexpr atom(const std::vector<sexpr>& a)
{
	if(a.size() < 1)
	   throw std::invalid_argument("<atom>: Wrong number of arguments");
	if(a[0].get_type() == sexpr_type::list_type){
		if(a[0].get<std::vector<sexpr>>().size() == 0){
	      return sexpr(true);
		} else {
	      return sexpr(false);
		}
	} else if (a[0].get_type() == sexpr_type::lambda_type) {
		return sexpr(false);
	} else if (a[0].get_type() == sexpr_type::function_type) {
		return sexpr(false);
	} else {
		return sexpr(true);
	}
}

sexpr ls(const std::vector<sexpr>& a, environment* env)
{
	if(a.size() != 0)
	   throw std::invalid_argument("<ls>: Wrong number of arguments");
	std::cout << *env << std::endl;
	return sexpr(sexpr::nil_type {});
}

sexpr import(const std::vector<sexpr>& a, environment* env)
{
	if(a.size() < 1)
	   throw std::invalid_argument("<import>: Wrong number of arguments");
	auto path = a[0].get<lisp_string>();
	std::ifstream file(path);
	if(!file)
	   throw std::invalid_argument("<import>: File not found: "+ path);
	std::regex reg("([;]{2,}.*$)");
	std::string line;
	std::string content;
	while(std::getline(file,line)){
		content += std::regex_replace(line,reg,"");
	}
	return evals(parse(tokenize(content)),env);
}


sexpr imports(const std::vector<sexpr>& a, environment* env)
{
	std::vector<sexpr> l;
	l.push_back(sexpr(lisp_string("/home/icepic/Code/lisp++/lisp.el")));
	return import(l,env);
}

	

	
