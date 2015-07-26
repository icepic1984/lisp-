#ifndef PARSER_HPP
#define PARSER_HPP
#include <vector>
#include <string>
#include "sexpr.hpp"

using expressions_t = std::vector<sexpr>;
using tokens_t = std::vector<std::string>;

bool is_special(char c);

tokens_t tokenize(const std::string& str);

expressions_t parse(const tokens_t& tokens);

template <typename Iter>
std::pair<sexpr,Iter> parse_helper(Iter begin, Iter end);

#include "parser.inl"

#endif
