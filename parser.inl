template <typename Iter>
std::pair<sexpr,Iter> parse_helper(Iter begin, Iter end)
{
	if(begin == end)
	   throw std::invalid_argument("Syntax error");
	auto token = *begin++;
	if(token == "("){
		sexpr expr;
		while(*begin != ")"){
			auto tmp = parse_helper(begin,end);
			if(tmp.second == end)
			   throw std::invalid_argument("Syntax error");
			expr.push_back(tmp.first);
			begin = tmp.second;
		}
		return std::make_pair(expr,++begin);
	} else if (token == ")") {
		throw std::invalid_argument("Syntax error");
	} else {
		return make_pair(sexpr(token),begin);
	}
}


	
