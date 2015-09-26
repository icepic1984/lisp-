#include "operators.hpp"
#include "environment.hpp"
#include <memory>

eval_helper::result_t eval_helper::operator()(const std::vector<sexpr>& l,
                                              environment* env)
{
	if (l.empty())
		return sexpr(sexpr::nil_type{});

	if (l.front() == sexpr("quote")) {
		if (l.size() < 2)
			throw std::invalid_argument(
			    "eval_helper <quote>: Wrong number of arguments");
		return l[1];
	}

	if (l.front() == sexpr("cond")) {
		if (l.size() < 2)
			throw std::invalid_argument(
			    "eval_helper <cond>: Wrong number of arguments");
		for (auto iter = l.begin() + 1; iter != l.end(); ++iter) {
			auto tmp = visit(visit(*iter, nth_helper{}, size_t(0)),
			                 eval_helper{}, env);
			if (tmp) {
				return visit(visit(*iter, nth_helper{}, size_t(1)),
				             eval_helper{}, env);
			}
		}
		return sexpr(sexpr::nil_type{});
	}

	if (l.front() == sexpr("set")) {
		if (l.size() != 3)
			throw std::invalid_argument(
			    "eval_helper <set!>: Wrong number of arguments");
		return env->update_symbol(l[1].get<std::string>(),
		                          visit(l[2], eval_helper{}, env));
	}

	if (l.front() == sexpr("define")) {
		if (l.size() != 3)
			throw std::invalid_argument(
			    "eval_helper <defvar>: Wrong number of arguments");
		return env->set_symbol(l[1].get<std::string>(),
		                       visit(l[2], eval_helper{}, env));
	}

	if (l.front() == sexpr("lambda")) {
		sexpr tmp(l);
		tmp.set_type(sexpr_type::lambda_type);
		tmp.set_env(env->create());
		return tmp;
	}

	if (l.front() == sexpr("begin")) {
		for (auto iter = l.begin() + 1; iter != l.end() - 1; ++iter) {
			visit(*iter, eval_helper{}, env);
		}
		return visit(l.back(), eval_helper{}, env);
	}

	auto proc = visit(l.front(), eval_helper{}, env);
	std::vector<sexpr> exprs;
	for (auto iter = l.begin() + 1; iter != l.end(); ++iter) {
		exprs.push_back(visit(*iter, eval_helper{}, env));
	}

	if (proc.get_type() == sexpr_type::lambda_type) {
		auto param =
		    proc.get<std::vector<sexpr>>()[1].get<std::vector<sexpr>>();
		auto body = proc.get<std::vector<sexpr>>()[2];
		if (param.size() != exprs.size())
			throw std::invalid_argument(
			    "eval_helper <lambda>: Wrong number of arguments");

		auto newenv = std::make_shared<environment>(environment(
		    proc.get_env() .get())); 
		for (std::size_t i = 0; i < param.size(); ++i) {
			newenv->set_symbol(param[i].get<std::string>(), exprs[i]);
		}
		return visit(body, eval_helper{}, newenv.get());
	}
	else if (proc.get_type() == sexpr_type::function_type) {
		return proc(exprs);
	}
	throw std::invalid_argument("Not a function");
}

eval_helper::result_t eval_helper::operator()(double a, environment*)
{
	return sexpr(a);
}

eval_helper::result_t eval_helper::operator()(int a, environment*)
{
	return sexpr(a);
}

eval_helper::result_t eval_helper::operator()(const std::string& a,
                                              environment* env)
{
	auto symbol = env->find_symbol(a);
	if (symbol) {
		return *symbol;
	}
	throw std::invalid_argument("eval_helper <symbol>: Undefined symbol " + a);
}

eval_helper::result_t eval_helper::operator()(const lisp_string& a,
                                              environment* env)
{
	(void)env;
	return sexpr(a);
}

eval_helper::result_t eval_helper::operator()(sexpr::nil_type, environment*)
{
	return sexpr(sexpr::nil_type{});
}

eval_helper::result_t eval_helper::operator()(const sexpr::func_t& f,
                                              environment*)
{
	return sexpr(f);
}

eval_helper::result_t eval_helper::operator()(sexpr::invalid_type, environment*)
{
	return sexpr(sexpr::invalid_type{});
}

eval_helper::result_t eval_helper::operator()(bool b, environment*)
{
	return sexpr(b);
}
