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
 
template <typename T>
struct tag_dispatch{
   typedef sexpr::basic_type type;
};

template <>
struct tag_dispatch<sexpr::nil_type>
{
   typedef sexpr::nil_type type;
};

template <>
struct tag_dispatch<sexpr::invalid_type>
{
   typedef sexpr::invalid_type type;
};

template <>
struct tag_dispatch<bool>
{
   typedef sexpr::bool_type type;
};

template <>
struct tag_dispatch<std::vector<sexpr>>
{
	typedef sexpr::list_type type;
};

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

struct sexpr_print 
{
   typedef std::ostream& result_t;

   template <typename From>
   result_t dispatch(const From& val,std::ostream& os, sexpr::basic_type){
	   return os << val<<" ";
   }

   template <typename From>
   result_t dispatch(const From& val,std::ostream& os, sexpr::invalid_type){
	   return os <<"<invalid>";
   }

   template <typename From>
   result_t dispatch(const From& val,std::ostream& os, sexpr::nil_type){
	   return os <<"<nil>";
   }

   template <typename From>
   result_t dispatch(const From& val,std::ostream& os, sexpr::bool_type){
	   if(val){
		   return os <<"<true>"<<" ";
	   } else {
		   return os <<"<false>"<<" ";
	   }
   }

   template <typename From>
   result_t dispatch(const From& val,std::ostream& os, sexpr::list_type){
	   os << "( ";
	   for(auto &iter : val){
		   visit(iter,sexpr_print{},os);
	   }
	   os << ") ";
	   return os;
   }

   template <typename From>
   result_t operator()(const From& val, std::ostream& os) {
	   typename tag_dispatch<From>::type tag;
	   return dispatch(val,os,tag);
   }
};

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

template <typename F, typename X>
struct bind_impl 
{
   typedef typename F::result_t result_t;
   X& x; 
   F f;
   bind_impl(F f, X& x) : x(x), f(f) {}
   
   template <typename Y>
   typename F::result_t operator()(Y& y) const {
	   return f(x, y);
   }
   
   template <typename Y>
   typename F::result_t operator()(Y const& y) const {
	   return f(x, y);
   }
};

template <typename F, typename X>
bind_impl<F, X const> bind(F f, X const& x)
{
	return bind_impl<F, X const>(f, x);
}

template <typename F, typename X>
bind_impl<F, X> bind(F f, X& x)
{
	return bind_impl<F, X>(f, x);
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

struct sexpr_add
{
   typedef sexpr result_t;

   result_t dispatch(double x, double y) const{
	   return sexpr(x+y);
   }

   result_t dispatch(double x, const std::vector<sexpr>& y) const {
	   sexpr tmp(x);
	   for (auto &iter : y){
		   tmp = visit(tmp,iter,sexpr_add{});
	   }
	   return tmp;
   }

   result_t dispatch(const std::vector<sexpr>& x, double y) const {
	   return dispatch(y,x);
   }

   template <typename A, typename B>
   result_t dispatch(const A& x, const B& y) const {
	   throw std::invalid_argument("Shit");
	   return sexpr{};
   }

   template <typename A, typename B>
   result_t operator()(const A& x, const B& y) const {
	   return dispatch(x,y);
   }
};



template <typename T>
T sexpr::get() const
{return visit(*this,sexpr_cast<T>());}
	
