#ifndef SEXPR_HPP
#define SEXPR_HPP
#include <string>
#include <iostream>
#include <vector>

struct sexpr_type
{
   enum info
   {
	   integer_type,
	   string_type,
	   double_type,
	   list_type,
	   nil_type,
	   invalid_type,
	   bool_type
   };
};

class sexpr 
{
public:
   struct invalid_type {};
   struct nil_type {};
   struct list_type {};
   struct basic_type {};
   struct string_type {};
   struct bool_type {};
   struct function_type {};
   
   sexpr();
   
   explicit sexpr(nil_type);
   
   explicit sexpr(invalid_type);

   explicit sexpr(int i_);

   explicit sexpr(double d_);

   explicit sexpr(bool b_);

   explicit sexpr(const std::string& s_);

   explicit sexpr(const char* s_);
   
   explicit sexpr(const std::vector<sexpr>& exprs);

   operator bool();

   sexpr(const sexpr& expr);

   sexpr& operator=(sexpr&& other) noexcept;
  
   sexpr& operator=(const sexpr& other);
  
   sexpr(sexpr&& other) noexcept;
   
   ~sexpr();

   template <typename T>
   void push_back(const T& t);

   template <typename F, typename... V>
   friend typename F::result_t visit(const sexpr& expr, F f, V&&... v);

   template <typename F>
   friend typename F::result_t visit(const sexpr& a, const sexpr& b, F f);

   sexpr_type::info get_type();

   template <typename T>
   T get() const;
   
private:
   void set_type(sexpr_type::info type);
   
   int type_field;
   union 
   {
      int i;
      double d;
      bool b;
      std::string s;
      std::vector<sexpr> l;
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
