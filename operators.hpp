#ifndef OPERATORS_HPP
#define OPERATORS_HPP

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



#endif


