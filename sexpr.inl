#include <stdexcept>
#include <type_traits>
#include <typeinfo>

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

template <typename F>
typename F::result_t visit(const sexpr& expr, F f)
{
	switch(expr.type_field){
	case sexpr_type::invalid_type:
		return f(sexpr::invalid_type{});
	case sexpr_type::nil_type:
		return f(sexpr::nil_type{});
		break;
	case sexpr_type::integer_type:
		return f(expr.i);
		break;
	case sexpr_type::string_type:
		return f(expr.s);
		break;
	case sexpr_type::double_type:
		return f(expr.d);
		break;
	case sexpr_type::list_type:
		return f(expr.l);
		break;
	}
}

template <typename T>
T sexpr::get() const
{return visit(*this,sexpr_cast<T>());}
	
