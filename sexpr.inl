#include <stdexcept>
#include <type_traits>
#include <typeinfo>
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
	case sexpr_type::symbol_type:
		return f(expr.s,std::forward<V>(v)...);
		break;
	case sexpr_type::string_type:
		return f(expr.ls,std::forward<V>(v)...);
		break;
	case sexpr_type::double_type:
		return f(expr.d,std::forward<V>(v)...);
		break;
	case sexpr_type::bool_type:
		return f(expr.b,std::forward<V>(v)...);
		break;
	case sexpr_type::function_type:
		return f(expr.f,std::forward<V>(v)...);
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
	case sexpr_type::symbol_type:
		return visit(b,bind(f,a.s));
		break;
	case sexpr_type::string_type:
		return visit(b,bind(f,a.ls));
	case sexpr_type::double_type:
		return visit(b,bind(f,a.d));
		break;
	case sexpr_type::bool_type:
		return visit(b,bind(f,a.b));
		break;
	case sexpr_type::function_type:
		return visit(b,bind(f,a.f));
	default:
		return visit(b,bind(f,a.l));
		break;
	}
}

template <typename To>
struct sexpr_cast
{
   typedef To result_t;

   template <typename From>
   To dispatch(From const& val, std::true_type) const {
	   return To(val); 
   }

   template <typename From>
   To dispatch(From const&, std::false_type) const {
	   throw std::bad_cast();
	   return To();
   }

   template <typename From>
   To operator()(From const& val) const {
	   typename  std::conditional<std::is_convertible<From, To>::value ||
	                    std::is_same<From,To>::value,
	                    std::true_type, std::false_type>::type is_convertible;
	   
	   return dispatch(val, is_convertible);
   }
};

template <typename T>
T sexpr::get() const
{return visit(*this,sexpr_cast<T>());}
	
