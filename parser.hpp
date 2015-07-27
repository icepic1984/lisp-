#ifndef PARSER_HPP
#define PARSER_HPP
#include <vector>
#include <string>
#include "sexpr.hpp"

typedef std::vector<sexpr> expressions_t;
typedef std::vector<std::string> tokens_t;

bool is_special(char c);

tokens_t tokenize(const std::string& str);

expressions_t parse(const tokens_t& tokens);

sexpr atom(const std::string& str);

template <typename Iter>
std::pair<sexpr,Iter> parse_helper(Iter begin, Iter end);

#include "parser.inl"

#endif
