#include <stdexcept>
#include <type_traits>
#include <typeinfo>
#include "operators.hpp"
#include "bind.hpp"

template <typename T>
void sexpr::push_back(const T& t){
	if (type_field == sexpr_type::invalid_type){
		set_type(sexpr_type::list_type);
		new(&l) std::vector<sexpr>();
		l.emplace_back(t); 
	} else if (type_field == sexpr_type::list_type) {
		l.emplace_back(t);
	} else {
		   throw std::invalid_argument("wrong type");
	}
}
 
template <typename F, typename... V>
typename F::result_t visit(const sexpr& expr, F f,V&&... v)
{
	switch(expr.type_field){
	case sexpr_type::invalid_type:
		return f(sexpr::invalid_type{},std::forward<V>(v)...);
		break;
	case sexpr_type::nil_type:
		return f(sexpr::nil_type{},std::forward<V>(v)...);
		break;
	case sexpr_type::integer_type:
		return f(expr.i,std::forward<V>(v)...);
		break;
	case sexpr_type::string_type:
		return f(expr.s,std::forward<V>(v)...);
		break;
	case sexpr_type::double_type:
		return f(expr.d,std::forward<V>(v)...);
		break;
	case sexpr_type::bool_type:
		return f(expr.b,std::forward<V>(v)...);
		break;
	default:
		return f(expr.l,std::forward<V>(v)...);
		break;
	}
}

template <typename F>
typename F::result_t visit(const sexpr& a, const sexpr& b, F f)
{
	switch(a.type_field){
	case sexpr_type::invalid_type:
		return visit(b,bind(f,sexpr::invalid_type{}));
		break;
	case sexpr_type::nil_type:
		return visit(b,bind(f,sexpr::nil_type{}));
		break;
	case sexpr_type::integer_type:
		return visit(b,bind(f,a.i));
		break;
	case sexpr_type::string_type:
		return visit(b,bind(f,a.s));
		break;
	case sexpr_type::double_type:
		return visit(b,bind(f,a.d));
		break;
	case sexpr_type::bool_type:
		return visit(b,bind(f,a.b));
		break;
	default:
		return visit(b,bind(f,a.l));
		break;
	}
}

template <typename T>
T sexpr::get() const
{return visit(*this,sexpr_cast<T>());}
	
