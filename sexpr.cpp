#include <stdexcept>
#include "sexpr.hpp"
#include "operators.hpp"

sexpr::sexpr()
{
	set_type(sexpr_type::invalid_type);
}

sexpr::sexpr(nil_type)
{
	set_type(sexpr_type::nil_type);
}

sexpr::sexpr(invalid_type)
{
	set_type(sexpr_type::invalid_type);
}

sexpr::sexpr(int i_) : i(i_)
{
	set_type(sexpr_type::integer_type);
}

sexpr::sexpr(bool b_) : b(b_)
{
	set_type(sexpr_type::bool_type);
}

sexpr::sexpr(double d_) : d(d_)
{
	set_type(sexpr_type::double_type);
}

sexpr::sexpr(sexpr::func_t f_)
{
	new (&f) sexpr::func_t(f_);
	set_type(sexpr_type::function_type);
}

sexpr::sexpr(const char* s_)
{
	new (&s) std::string(s_);
	set_type(sexpr_type::symbol_type);
}

sexpr::sexpr(const std::string& s_)
{
	new (&s) std::string(s_);
	this->set_type(sexpr_type::symbol_type);
}

sexpr::sexpr(const lisp_string& ls_)
{
	new (&ls) lisp_string(ls_);
	this->set_type(sexpr_type::string_type);
}

sexpr::sexpr(const std::vector<sexpr>& exprs)
{
	new (&l) std::vector<sexpr>(exprs);
	this->set_type(sexpr_type::list_type);
}

sexpr::sexpr(const sexpr& expr) : type_field(expr.type_field), env(expr.env)
{
	switch (expr.type_field) {
	case sexpr_type::string_type:
		new (&ls) lisp_string(expr.ls);
		break;
	case sexpr_type::symbol_type:
		new (&s) std::string(expr.s);
		break;
	case sexpr_type::list_type:
	case sexpr_type::lambda_type:
		new (&l) std::vector<sexpr>(expr.l);
		break;
	case sexpr_type::function_type:
		new (&f) func_t(expr.f);
		break;
	case sexpr_type::double_type:
		d = expr.d;
		break;
	case sexpr_type::integer_type:
		i = expr.i;
		break;
	case sexpr_type::bool_type:
		b = expr.b;
		break;
	}
}

sexpr::sexpr(sexpr&& other) noexcept : type_field(std::move(other.type_field)),
                                       env(std::move(other.env))
{
	switch (type_field) {
	case sexpr_type::integer_type:
		i = std::move(other.i);
		break;
	case sexpr_type::double_type:
		d = std::move(other.d);
		break;
	case sexpr_type::bool_type:
		b = std::move(other.b);
		break;
	case sexpr_type::symbol_type:
		new (&s) std::string(std::move(other.s));
		break;
	case sexpr_type::string_type:
		new (&ls) lisp_string(std::move(other.ls));
		break;
	case sexpr_type::list_type:
	case sexpr_type::lambda_type:
		new (&l) std::vector<sexpr>(std::move(other.l));
		break;
	case sexpr_type::function_type:
		new (&f) func_t(std::move(other.f));
		break;
	}
}

sexpr::~sexpr()
{
	typedef std::string string_type;
	typedef lisp_string lispstring_type;
	typedef std::vector<sexpr> list_type;

	switch (type_field) {
	case sexpr_type::symbol_type:
		s.~string_type();
		break;
	case sexpr_type::string_type:
		ls.~lispstring_type();
		break;
	case sexpr_type::list_type:
	case sexpr_type::lambda_type:
		l.~list_type();
		break;
	case sexpr_type::function_type:
		f.~func_t();
		break;
	}
}

sexpr& sexpr::operator=(sexpr&& other) noexcept
{
	this->~sexpr();
	type_field = std::move(other.type_field);
	env = std::move(other.env);
	switch (other.type_field) {
	case sexpr_type::integer_type:
		i = std::move(other.i);
		break;
	case sexpr_type::double_type:
		d = std::move(other.d);
		break;
	case sexpr_type::bool_type:
		b = std::move(other.b);
		break;
	case sexpr_type::symbol_type:
		new (&s) std::string(std::move(other.s));
		break;
	case sexpr_type::string_type:
		new (&ls) lisp_string(std::move(other.ls));
		break;
	case sexpr_type::list_type:
	case sexpr_type::lambda_type:
		new (&l) std::vector<sexpr>(std::move(other.l));
		break;
	case sexpr_type::function_type:
		new (&f) func_t(std::move(other.f));
	}
	other.type_field = sexpr_type::invalid_type;
	return *this;
}

sexpr& sexpr::operator=(const sexpr& other)
{
	sexpr t(other);
	std::swap(*this, t);
	return *this;
}

sexpr::operator bool()
{
	if (type_field == sexpr_type::bool_type)
		return b;
	if (type_field == sexpr_type::nil_type)
		return false;
	throw std::invalid_argument("Can't cast to bool");
}

sexpr sexpr::operator()(const std::vector<sexpr>& expr)
{
	if (type_field != sexpr_type::function_type)
		throw std::invalid_argument("Not a function object");
	return f(expr);
}

void sexpr::set_type(sexpr_type::info type)
{
	type_field = type;
}

environment_ptr sexpr::get_env()
{
	return env;
}

void sexpr::set_env(environment_ptr e)
{
	env = e;
}

sexpr_type::info sexpr::get_type() const
{
	return sexpr_type::info(type_field);
}

std::ostream& operator<<(std::ostream& os, const sexpr& expr)
{
	return visit(expr, sexpr_print{}, os);
}

sexpr operator+(const sexpr& a, const sexpr& b)
{
	return visit(a, b, sexpr_arithmetic<add_binary>{});
}

sexpr operator-(const sexpr& a, const sexpr& b)
{
	return visit(a, b, sexpr_arithmetic<minus_binary>{});
}

sexpr operator*(const sexpr& a, const sexpr& b)
{
	return visit(a, b, sexpr_arithmetic<mul_binary>{});
}

sexpr operator/(const sexpr& a, const sexpr& b)
{
	return visit(a, b, sexpr_arithmetic<div_binary>{});
}

sexpr operator==(const sexpr& a, const sexpr& b)
{
	return sexpr(visit(a, b, sexpr_is_equal{}));
}

sexpr operator<(const sexpr& a, const sexpr& b)
{
	return sexpr(visit(a, b, sexpr_is_less{}));
}

sexpr operator>(const sexpr& a, const sexpr& b)
{
	return sexpr(visit(a, b, sexpr_is_greater{}));
}

sexpr operator<=(const sexpr& a, const sexpr& b)
{
	return sexpr(visit(a, b, sexpr_is_less_equal{}));
}

sexpr operator>=(const sexpr& a, const sexpr& b)
{
	return sexpr(visit(a, b, sexpr_is_greater_equal{}));
}

sexpr operator!=(const sexpr& a, const sexpr& b)
{
	return sexpr(visit(a, b, sexpr_not_equal{}));
}
