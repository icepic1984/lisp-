#ifndef SEXPR_HPP
#define SEXPR_HPP
#include <string>
#include <iostream>
#include <vector>
#include <functional>
#include <memory>
#include "lisp_string.hpp"

struct sexpr_type
{
   enum info
   {
	   integer_type,
	   string_type,
	   symbol_type,
	   double_type,
	   list_type,
	   nil_type,
	   invalid_type,
	   bool_type,
	   lambda_type,
	   function_type
   };
};

class environment;
using environment_ptr = std::shared_ptr<environment>;
using environment_weak = std::weak_ptr<environment>;

class sexpr 
{
public:
   using func_t = std::function<sexpr(const std::vector<sexpr>&)>;

   struct invalid_type {};
   struct nil_type {};
   struct list_type {};
   struct basic_type {};
   struct string_type {};
   struct symbol_type {};
   struct bool_type {};
   struct lambda_type{};
   struct function_type {};
   
   sexpr();
   
   explicit sexpr(nil_type);
   
   explicit sexpr(invalid_type);

   explicit sexpr(int i_);

   explicit sexpr(double d_);

   explicit sexpr(bool b_);

   explicit sexpr(const std::string& s_);

   explicit sexpr(const lisp_string& ls_);

   explicit sexpr(const char* s_);
   
   explicit sexpr(const std::vector<sexpr>& exprs);

   explicit sexpr(func_t);
   
   operator bool();

   sexpr(const sexpr& expr);
   
   sexpr(sexpr&& other) noexcept;

   ~sexpr();

   sexpr& operator=(sexpr&& other) noexcept;
  
   sexpr& operator=(const sexpr& other);

   sexpr operator()(const std::vector<sexpr>& expr);

   template <typename T>
   void push_back(const T& t);

   template <typename F, typename... V>
   friend typename F::result_t visit(const sexpr& expr, F f, V&&... v);

   template <typename F>
   friend typename F::result_t visit(const sexpr& a, const sexpr& b, F f);

   sexpr_type::info get_type() const;

   void set_type(sexpr_type::info type);

   environment_ptr get_env();

   void set_env(environment_ptr e);

   template <typename T>
   T get() const;
   
private:
   int type_field;
   environment_ptr env;
   
   union 
   {
      int i;
      double d;
      bool b;
      std::string s;
      lisp_string ls;
      std::vector<sexpr> l;
      func_t f;
   };
};

std::ostream& operator<<(std::ostream& os, const sexpr& expr);

sexpr operator+(const sexpr& a, const sexpr& b);

sexpr operator-(const sexpr& a, const sexpr& b);

sexpr operator*(const sexpr& a, const sexpr& b);

sexpr operator/(const sexpr& a, const sexpr& b);

sexpr operator==(const sexpr& a, const sexpr& b);

sexpr operator!=(const sexpr&a, const sexpr& b);

sexpr operator <(const sexpr& a, const sexpr& b);

sexpr operator >(const sexpr& a, const sexpr& b);

sexpr operator <=(const sexpr& a, const sexpr& b);

sexpr operator >=(const sexpr& a, const sexpr& b);
#include "sexpr.inl"

#endif
