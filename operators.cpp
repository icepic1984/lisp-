#include "operators.hpp"
#include "environment.hpp"

eval_helper::result_t eval_helper::operator() (const std::vector<sexpr>& l,environment& env){
	if (l.empty())
	   return sexpr(sexpr::nil_type {});
	
	if(l.front() == sexpr("quote")){
		if(l.size() < 2)
		   throw std::invalid_argument("eval_helper <quote>: Wrong number of arguments");
		return l[1];
	}

	if(l.front() == sexpr("cond")){
		if(l.size() < 2)
		   throw std::invalid_argument("eval_helper <cond>: Wrong number of arguments");
		for(auto iter = l.begin()+1; iter != l.end(); ++iter){
			auto tmp = visit(visit(*iter,nth_helper{},size_t(0)),
			                 eval_helper{}, env);
			if(tmp)
			   return visit(visit(*iter,nth_helper {}, size_t(1)),
			                eval_helper{}, env);
			   
		} 
		return sexpr(sexpr::nil_type{});
	}

	if(l.front() == sexpr("define")){
		if(l.size() != 3)
			throw std::invalid_argument("eval_helper <defvar>: Wrong number of arguments");
		env.set_symbol(l[1].get<std::string>(),l[2]);
		return l[2];
	}

	if(l.front() == sexpr("atom")){
		if(l.size() != 2)
		   throw std::invalid_argument("eval_helper <atom>: Wrong number of arguments");
		auto tmp = visit(l[1],eval_helper {}, env);
		if(tmp.get_type() != sexpr_type::list_type)
		   return sexpr(true);
		return sexpr(false);
	}

	auto f = env.find_buildin(l.front().get<std::string>());
	std::vector<sexpr> exprs;
	for(auto iter = l.begin()+1; iter != l.end();
	    ++iter){
		exprs.push_back(visit(*iter,eval_helper{},env));
	}
	return f(exprs);
}

eval_helper::result_t eval_helper::operator() (double a, environment&) {
	   return sexpr(a);
}

eval_helper::result_t eval_helper::operator() (int a, environment&) {
	return sexpr(a);
}

eval_helper::result_t eval_helper::operator() (const std::string& a, environment& env ){
	auto symbol = env.find_symbol(a);
	if(symbol){
		return *symbol;
	}
	throw std::invalid_argument("eval_helper "+a+" undefinded symbol");
}

eval_helper::result_t eval_helper::operator() (sexpr::nil_type, environment&){
	return sexpr(sexpr::nil_type {});
}

eval_helper::result_t eval_helper::operator() (sexpr::invalid_type, environment&){
	return sexpr(sexpr::invalid_type {});
}

eval_helper::result_t eval_helper::operator() (bool b, environment&){
	return sexpr(b);
}


