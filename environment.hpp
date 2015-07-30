#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP
#include <functional>
#include <vector>
#include <unordered_map>

class sexpr;

class environment 
{
public:
   using func_t = std::function<sexpr(const std::vector<sexpr>&)>;

   using map_t = std::unordered_map<std::string, func_t>; 

   environment();

   func_t find(const std::string& name);
   
private:
   map_t map;
};

   
#endif
