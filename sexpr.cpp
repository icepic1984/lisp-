#include <stdexcept>
#include "sexpr.hpp"
#include "operators.hpp"

sexpr::sexpr(){
	set_type(sexpr_type::invalid_type);
}

sexpr::sexpr(nil_type){
	set_type(sexpr_type::nil_type);
}
   
sexpr::sexpr(invalid_type){
	set_type(sexpr_type::invalid_type);
}

sexpr::sexpr(int i_) : i(i_){
	set_type(sexpr_type::integer_type);
}

sexpr::sexpr(bool b_) : b(b_)
{
	set_type(sexpr_type::bool_type);
}

sexpr::sexpr(double d_) : d(d_){
	set_type(sexpr_type::double_type);
}

sexpr::sexpr(const std::string& s_) {
	new(&s)std::string(s_);
	this->set_type(sexpr_type::string_type);
}
   
sexpr::sexpr(const sexpr& expr) :
	type_field(expr.type_field) {
	switch(expr.type_field){
	case sexpr_type::string_type:
		new(&s)std::string(expr.s);
		break;
	case sexpr_type::list_type:
		new(&l) std::vector<sexpr>(expr.l);
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
 
sexpr::sexpr(sexpr&& other) noexcept :
	type_field(std::move(other.type_field)) {
	switch(type_field){
	case sexpr_type::integer_type:
		i = std::move(other.i);
		break;
	case sexpr_type::double_type:
		d = std::move(other.d);
		break;
	case sexpr_type::bool_type:
		b = std::move(other.b);
		break;
	case sexpr_type::string_type:
		new(&s) std::string(std::move(other.s));
		break;
	case sexpr_type::list_type:
		new(&l) std::vector<sexpr>(std::move(other.l));
		break;
	}
}
   
sexpr::~sexpr() {
	typedef std::string string_type;
	typedef std::vector<sexpr> list_type;
	switch(type_field){
	case sexpr_type::string_type:
		s.~string_type();
		break;
	case sexpr_type::list_type:
		l.~list_type();
		break;
	}
}

sexpr& sexpr::operator=(sexpr&& other) noexcept {
	this->~sexpr();
	type_field = std::move(other.type_field);
	switch(other.type_field){
	case sexpr_type::integer_type:
		i = std::move(other.i);
		break;
	case sexpr_type::double_type:
		d = std::move(other.d);
		break;
	case sexpr_type::bool_type:
		b = std::move(other.b);
		break;
	case sexpr_type::string_type:
		new(&s) std::string(std::move(other.s));
		break;
	case sexpr_type::list_type:
		new(&l) std::vector<sexpr>(std::move(other.l));
		break;
	}
	other.type_field = sexpr_type::invalid_type;
	return *this;
}

sexpr& sexpr::operator=(const sexpr& other){
	sexpr t(other);
	std::swap(*this,t);
	return *this;
}

void sexpr::set_type(sexpr_type::info type)
{type_field = type;}

sexpr_type::info sexpr::get_type()
{return sexpr_type::info(type_field);}

std::ostream& operator<<(std::ostream& os, const sexpr& expr)
{return visit(expr,sexpr_print{},os);}

sexpr operator+(const sexpr& a, const sexpr& b)
{return visit(a,b,sexpr_arithmetic<add_binary> {});}

sexpr operator-(const sexpr& a, const sexpr& b)
{return visit(a,b,sexpr_arithmetic<minus_binary> {});}

sexpr operator*(const sexpr& a, const sexpr& b)
{return visit(a,b,sexpr_arithmetic<mul_binary> {});}

sexpr operator/(const sexpr& a, const sexpr& b)
{return visit(a,b,sexpr_arithmetic<div_binary> {});}

sexpr operator==(const sexpr& a, const sexpr& b)
{return sexpr(visit(a,b,sexpr_is_equal {}));}
 
sexpr operator <(const sexpr& a, const sexpr& b)
{return sexpr(visit(a,b,sexpr_is_less {}));}

sexpr operator >(const sexpr& a, const sexpr& b)
{return sexpr(visit(b,a,sexpr_is_less {}));}

sexpr operator <=(const sexpr& a, const sexpr& b)
{return sexpr(visit(a,b,sexpr_is_less_equal {}));}

sexpr operator >=(const sexpr& a, const sexpr& b)
{return sexpr(visit(b,a,sexpr_is_less_equal {}));}



	
