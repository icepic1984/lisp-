#include <stdexcept>
#include "sexpr.hpp"

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
	case sexpr_type::string_type:
		new(&s) std::string(std::move(other.s));
		break;
	case sexpr_type::list_type:
		new(&l) std::vector<sexpr>(std::move(other.l));
		break;
	}
}
   
sexpr::~sexpr() {
	using string_type = std::string;
	using list_type = std::vector<sexpr>;
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

std::ostream& operator<<(std::ostream& os, const sexpr& expr)
{
	switch(expr.type_field){
	case sexpr_type::nil_type:
		os << sexpr::nil_type{};
		break;
	case sexpr_type::invalid_type:
		os << sexpr::invalid_type{};
		break;
	case sexpr_type::integer_type:
		os <<expr.i;
		break;
	case sexpr_type::string_type:
		os << "\"" <<expr.s<<"\"";
		break;
	case sexpr_type::double_type:
		os <<expr.d;
		break;
	case sexpr_type::list_type:
		os << "( ";
		for(auto &iter : expr.l){
			os << iter << " ";
		}
		os << ")";
		break;
	}
	return os;
}

std::ostream& operator<<(std::ostream& os, const sexpr::nil_type& nil)
{
	os <<"<nil>";
	return os;
}

std::ostream& operator<<(std::ostream& os, const sexpr::invalid_type& invalid)
{
	os << "<invalid>";
	return os;
}

sexpr_type::info sexpr::get_type()
{return sexpr_type::info(type_field);}
