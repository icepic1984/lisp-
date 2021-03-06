#include <utility>
#include "environment.hpp"
#include "lisp.hpp"

environment::environment(environment* env)
    : symbols(
          {{"+", sexpr(sexpr::func_t(add))},
           {">", sexpr(sexpr::func_t(greater))},
           {"<", sexpr(sexpr::func_t(less))},
           {"<=", sexpr(sexpr::func_t(less_equal))},
           {">=", sexpr(sexpr::func_t(greater_equal))},
           {"/", sexpr(sexpr::func_t(divs))},
           {"*", sexpr(sexpr::func_t(mul))},
           {"-", sexpr(sexpr::func_t(sub))},
           {"car", sexpr(sexpr::func_t(car))},
           {"cdr", sexpr(sexpr::func_t(cdr))},
           {"=", sexpr(sexpr::func_t(equal))},
           {"nth", sexpr(sexpr::func_t(nth))},
           {"atom", sexpr(sexpr::func_t(atom))},
           {"eval", sexpr(sexpr::func_t(
                        std::bind(evals, std::placeholders::_1, this)))},
           {"ls",
            sexpr(sexpr::func_t(std::bind(ls, std::placeholders::_1, this)))},
           {"cons", sexpr(sexpr::func_t(cons))},
           {"import", sexpr(sexpr::func_t(
                          std::bind(import, std::placeholders::_1, this)))},
           {"imports", sexpr(sexpr::func_t(
                           std::bind(imports, std::placeholders::_1, this)))}}),
      outer(env)
{
}

environment_ptr environment::create()
{return shared_from_this();}

environment::result_t environment::find_symbol(const std::string& name)
{
	auto s = symbols.find(name);
	if (s != symbols.end())
		return s->second;
	if (outer)
		return outer->find_symbol(name);
	return boost::none;
}

sexpr environment::update_symbol(const std::string& name, const sexpr& expr)
{
	auto env = get_environment(name);
	if (env)
		env->symbols[name] = expr;
	else
		this->symbols[name] = expr;
	return expr;
}

sexpr environment::set_symbol(const std::string& name, const sexpr& expr)
{
	symbols.insert(std::make_pair(name, expr));
	return expr;
}

std::ostream& operator<<(std::ostream& os, const environment& env)
{
	for (auto iter : env.symbols) {
		os << iter.first << " = " << iter.second << "\n";
	}
	if (env.outer)
		os << "****OUTER****"
		   << "\n" << *(env.outer);

	return os;
}

environment* environment::get_environment(const std::string& name)
{
	auto s = symbols.find(name);
	if (s != symbols.end())
		return this;
	if (outer)
		return outer->get_environment(name);
	return nullptr;
}
