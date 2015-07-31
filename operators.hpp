#ifndef OPERATORS_HPP
#define OPERATORS_HPP
#include "environment.hpp"

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

template <>
struct tag_dispatch<std::string>
{
   typedef sexpr::string_type type;
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
	   (void)val; //Disable compiler warning
	   return os <<"<invalid>";
   }

   template <typename From>
   result_t dispatch(const From& val,std::ostream& os, sexpr::nil_type){
	   (void)val; //Disable compiler warning
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
   result_t dispatch(const From& val, std::ostream& os, sexpr::string_type){
	   os << "\""<<val<<"\" ";
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
   environment env;

   result_t operator() (const std::vector<sexpr>& l){
	   if (l.empty())
		   return sexpr(sexpr::nil_type {});

	   if(l.front() == sexpr("quote")){
	      if(l.size() < 2)
	         throw std::invalid_argument("eval_helper <quoute>: Wrong number of arguments");
	      return l[1];
	   }

	   if(l.front() == sexpr("cond")){
		   if(l.size() < 2)
		      throw std::invalid_argument("eval_helper <cond>: Wrong number of arguments");
		   for(auto iter = l.begin()+1; iter != l.end(); ++iter){
			   auto tmp = visit(visit(*iter,nth_helper{},size_t(0)),
			                    eval_helper {});
			   if(tmp)
			      return visit(visit(*iter,nth_helper {}, size_t(1)),
			                   eval_helper {});
			   
		   } 
		   return sexpr(sexpr::nil_type{});
	   }
	   
	   auto f = env.find(l.front().get<std::string>());
	   std::vector<sexpr> exprs;
	   for(auto iter = l.begin()+1; iter != l.end();
	       ++iter){
		   exprs.push_back(visit(*iter,eval_helper{}));
	   }
	   return f(exprs);
   }

   result_t operator() (double a) {
	   return sexpr(a);
   }

   result_t operator() (int a) {
	   return sexpr(a);
   }

   result_t operator() (const std::string& a){
	   return sexpr(a);
   }

   result_t operator() (sexpr::nil_type){
	   return sexpr(sexpr::nil_type {});
   }

   result_t operator() (sexpr::invalid_type){
	   return sexpr(sexpr::invalid_type {});
   }

   result_t operator() (bool b){
	   return sexpr(b);
   }
};


#endif


