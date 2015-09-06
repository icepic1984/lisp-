#ifndef OPERATORS_HPP
#define OPERATORS_HPP
#include "sexpr.hpp"

class environment;

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
struct tag_dispatch<sexpr::func_t>
{
   typedef sexpr::function_type type;
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

template <>
struct tag_dispatch<lisp_string>
{
   typedef sexpr::string_type type;
};

template <>
struct tag_dispatch<std::string>
{
   typedef sexpr::symbol_type type;
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
	   (void)val; //Disable compiler warning
	   return os <<"<invalid>";
   }

   template <typename From>
   result_t dispatch(const From& val,std::ostream& os, sexpr::nil_type){
	   (void)val; //Disable compiler warning
	   return os <<"<nil>";
   }

   template <typename From>
   result_t dispatch(const From&, std::ostream& os, sexpr::function_type){
	   return os<<"<function>";
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
   result_t dispatch(const From& val, std::ostream& os, sexpr::symbol_type){
	   os << val;
	   return os;
   }

   template <typename From>
   result_t dispatch(const From& val, std::ostream& os, sexpr::string_type){
	   os << "\"" <<val <<"\"";
	   return os;
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

struct sexpr_is_equal
{
   typedef bool result_t;

   template<typename A, typename B>
   bool operator()(const A& a, const B& b) const {
	   typename std::conditional<std::is_integral<A>::value &&
	                             std::is_integral<B>::value,
	                             std::true_type,
	                             std::false_type>::type select;
	   return dispatch(a,b,select);
   }

   bool dispatch(const std::string& a, const std::string& b,
                 std::false_type) const {
	   return a == b;
   }

   bool dispatch(const lisp_string& a, lisp_string& b,
                 std::false_type) const {
	   return a == b;
   }

	   
   template <typename A, typename B>
   bool dispatch(const A&, const B&, std::false_type) const {
	   return false;
   }

   template <typename A, typename B>
   bool dispatch(const A& a, const B& b, std::true_type) const {
	   return a == b;
   }
   
   bool operator()(sexpr::nil_type, sexpr::nil_type ) const {
	   return true;
   }
   
   bool operator()(sexpr::invalid_type, sexpr::invalid_type) const {
	   return true;
   } 
   
};
	   
struct sexpr_not_equal
{
   typedef bool result_t;

   template<typename A, typename B>
   bool operator()(const A& a, const B& b) const {
	   typename std::conditional<std::is_integral<A>::value &&
	                             std::is_integral<B>::value,
	                             std::true_type,
	                             std::false_type>::type select;
	   return dispatch(a,b,select);
   }

   bool dispatch(const std::string& a, const std::string& b,
                 std::false_type) const {
	   return a != b;
   }

   bool dispatch(const lisp_string& a, const lisp_string& b,
                 std::false_type) const {
	   return a != b;
   }
	   
   template <typename A, typename B>
   bool dispatch(const A& a, const B& b, std::false_type) const {
	   return true;
   }

   template <typename A, typename B>
   bool dispatch(const A& a, const B& b, std::true_type) const {
	   return a != b;
   }
   
   bool operator()(sexpr::nil_type, sexpr::nil_type ) const {
	   return false;
   }
   
   bool operator()(sexpr::invalid_type, sexpr::invalid_type) const {
	   return false;
   } 
   
};

struct sexpr_is_less
{
   typedef bool result_t;

   template<typename A, typename B>
   bool operator()(const A& a, const B& b) const {
	   typename std::conditional<std::is_arithmetic<A>::value &&
	                             std::is_arithmetic<B>::value,
	                             std::true_type,
	                             std::false_type>::type select;
	   return dispatch(a,b,select);
   }

   template <typename A, typename B>
   bool dispatch(const A&, const B&, std::false_type) const {
	   throw std::invalid_argument("Wrong type argument");
	   return false;
   }

   template <typename A, typename B>
   bool dispatch(const A& a, const B& b, std::true_type) const {
	   return a < b;
   }
   
   bool operator()(sexpr::nil_type, sexpr::nil_type ) const {
	   throw std::invalid_argument("Wrong type argument");
	   return false;
   }
   
   bool operator()(sexpr::invalid_type, sexpr::invalid_type) const {
	   throw std::invalid_argument("Wrong type argument");
	   return false;
   } 
   
};

struct sexpr_is_less_equal
{
   typedef bool result_t;

   template<typename A, typename B>
   bool operator()(const A& a, const B& b) const {
	   typename std::conditional<std::is_arithmetic<A>::value &&
	                             std::is_arithmetic<B>::value,
	                             std::true_type,
	                             std::false_type>::type select;
	   return dispatch(a,b,select);
   }

   template <typename A, typename B>
   bool dispatch(const A&, const B&, std::false_type) const {
	   throw std::invalid_argument("Wrong type argument");
	   return false;
   }

   template <typename A, typename B>
   bool dispatch(const A& a, const B& b, std::true_type) const {
	   return a <= b;
   }
   
   bool operator()(sexpr::nil_type, sexpr::nil_type ) const {
	   throw std::invalid_argument("Wrong type argument");
	   return false;
   }
   
   bool operator()(sexpr::invalid_type, sexpr::invalid_type) const {
	   throw std::invalid_argument("Wrong type argument");
	   return false;
   } 
   
};

template <typename Base>
struct sexpr_arithmetic 
{
   typedef sexpr result_t;

   template <typename A, typename B>
   sexpr dispatch(const A&, const B&, std::false_type) const {
	   throw std::invalid_argument("Wrong type argument!");
	   return sexpr();
   }

   template <typename A, typename B>
   sexpr dispatch(const A& a, const B& b, std::true_type) const {
	   return Base::eval(a,b);
   }

   template <typename A, typename B>
   sexpr operator()(const A& a, const B& b) const {

	   typename std::conditional<std::is_arithmetic<A>::value &&
	                    std::is_arithmetic<B>::value,
	                    std::true_type,
	                    std::false_type>::type select;
	   
	   return dispatch(a,b,select);
   }

   template <typename A>
   sexpr dispatch (const A& a, std::true_type) const {
	   return Base::eval(a);
   }

   template <typename A>
   sexpr dispatch (const A& a, std::false_type) const {
	   throw std::invalid_argument("Wrong type argument!");
	   (void)a;
	   return sexpr{};
   }

   template <typename A>
   sexpr operator()(const A& a) const {
	   typename std::conditional<std::is_arithmetic<A>::value,
	                             std::true_type,
	                             std::false_type>::type select;
	   return dispatch(a,select);
   }
};

struct add_binary
{
   template <typename A, typename B>
   static sexpr eval(const A& a, const B& b) {
	   return sexpr(a+b); 
   }
};

struct minus_binary
{
   template <typename A, typename B>
   static sexpr eval(const A& a, const B& b) {
	   return sexpr(a-b); 
   }
};

struct mul_binary
{
   template <typename A, typename B>
   static sexpr eval(const A& a, const B& b){
	   return sexpr(a*b);
   }
};

struct div_binary
{
   template <typename A, typename B>
   static sexpr eval(const A& a, const B& b){
	   return sexpr(a/b);
   }
};

struct add_unary
{
   template <typename A>
   static sexpr eval(const A& a) {
	   return sexpr(+a); 
   }
};

struct minus_unary
{
   template <typename A>
   static sexpr eval(const A& a) {
	   return sexpr(-a); 
   }
};

struct car_helper
{
   typedef sexpr result_t;
   
   result_t operator() (const std::vector<sexpr>& l){
	   if (l.empty())
	      return sexpr(sexpr::nil_type {});
	   else
	      return l.front();
   }

   result_t operator() (sexpr::nil_type){
	   return sexpr(sexpr::nil_type {});
   }

   template <typename A>
   result_t operator()(const A&){
	   throw std::invalid_argument("Wrong argument type");
	   return sexpr();
   }
};

struct cdr_helper
{
   typedef sexpr result_t;

   result_t operator() (const std::vector<sexpr>& l){
	   if (l.empty())
	      return sexpr(sexpr::nil_type {});
	   else if (l.size() == 1)
	      return sexpr(sexpr::nil_type {});	
	   else 
		   return sexpr(std::vector<sexpr>(l.begin()+1,l.end()));
   }

   result_t operator() (sexpr::nil_type){
	   return sexpr(sexpr::nil_type {});
   }

   template <typename A>
   result_t operator()(const A&){
	   throw std::invalid_argument("Wrong argument type");
	   return sexpr();
   }
};

struct nth_helper
{
   typedef sexpr result_t;

   result_t operator() (const std::vector<sexpr>& l,std::size_t n){
	   if(n > l.size()-1){
		   throw std::invalid_argument("n_helper: List has wrong size");
	   }
	   return l[n];
   }

   template <typename A>
   result_t operator()(const A&, std::size_t){
	   throw std::invalid_argument("n_helper: Wrong argument type");
	   return sexpr();
   }
};
	   
struct eval_helper 
{
   typedef sexpr result_t;

   result_t operator() (const std::vector<sexpr>& l,environment* env);
   
   result_t operator() (double a, environment*);

   result_t operator() (int a, environment*);

   result_t operator() (const std::string& a, environment* );

   result_t operator() (const lisp_string& a, environment* );

   result_t operator() (const sexpr::func_t& f, environment*);

   result_t operator() (sexpr::nil_type, environment*);

   result_t operator() (sexpr::invalid_type, environment*);

   result_t operator() (bool b, environment*);
   
};


#endif

