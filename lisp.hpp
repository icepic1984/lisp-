#ifndef LISP_HPP
#define LISP_HPP
#include <vector>
#include "sexpr.hpp"

class environment;

sexpr car(const std::vector<sexpr>& exprs);

sexpr cdr(const std::vector<sexpr>& exprs);

sexpr add(const std::vector<sexpr>& exprs);

sexpr mul(const std::vector<sexpr>& exprs);

sexpr divs(const std::vector<sexpr>& exprs);

sexpr sub(const std::vector<sexpr>& exprs);

sexpr equal(const std::vector<sexpr>& exprs);

sexpr nth(const std::vector<sexpr>& a);

sexpr evals(const std::vector<sexpr>& a, environment& env);

sexpr eval(const sexpr& a, environment& env);




#endif
