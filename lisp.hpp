#ifndef LISP_HPP
#define LISP_HPP

class sexpr;

sexpr car(const sexpr& exp);

sexpr cdr(const sexpr& exp);

sexpr add(const sexpr& expr);

sexpr mul(const sexpr& expr);

sexpr divs(const sexpr& expr);

sexpr sub(const sexpr& expr);

#endif
