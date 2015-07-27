#include "parser.hpp"
#include <regex>

bool is_special(char c)
{
	if(c == '('){
		return true;
	} else if (c == ')') {
		return true;
	} else if (c == '\'') {
		return true;
	} else {
		return false;
	}
}

tokens_t tokenize(const std::string& str)
{
	tokens_t tokens;
	auto start = str.begin();
	while(start != str.end()){
		if(std::isspace(*start)){
			++start;
			continue;
		} else if (is_special(*start)) {
			tokens.push_back(std::string(1,*start));
		} else {
			std::string tmp;
			while((start != str.end()) &&
			      !(is_special(*start)) &&
			      !(std::isspace(*start))){
			   tmp += *start;
			   ++start;
			}
			tokens.push_back(tmp);
			--start;
		}
		++start;
	}	
	return tokens;
}

expressions_t parse(const tokens_t& tokens)
{
	expressions_t expressions;
	auto start = tokens.begin();
	auto end = tokens.end();
	while(start != tokens.end()){
		auto result = parse_helper(start,end);
		expressions.push_back(result.first);
		start = result.second;
	}
	return expressions;
}

sexpr atom(const std::string& str)
{
	std::regex floating_point("[+-]?[0-9]?\\.[0-9]+([eE][-+]?[0-9]+)?",
	                          std::regex::extended);
	std::regex integer("[-+]?[0-9]+",std::regex::extended);
	if(str == "nil"){
		return sexpr(sexpr::nil_type{});
	} else if (str == "t") {
		return sexpr(true);
	} else if (str == "f") {
		return sexpr(false);
	} else if (std::regex_match(str,floating_point)) {
		std::cout << "double" << std::endl;
		return sexpr(stod(str));
	} else if (std::regex_match(str,integer)) {
		std::cout << "int" << std::endl;
	 	return sexpr(stoi(str));
	} else {
		return sexpr(str);
	}
	// Is never reached
	return sexpr(sexpr::invalid_type{});
}


	

