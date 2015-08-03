#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP
#include <functional>
#include <vector>
#include <unordered_map>
#include <boost/optional.hpp>
#include <memory>
#include "sexpr.hpp"

class environment 
{
public:
   using symb_map_t = std::unordered_map<std::string,sexpr>;
   using result_t = boost::optional<sexpr>;
   
   environment(environment* env = nullptr);

   // sexpr::func_t find_buildin(const std::string& name);

   result_t find_symbol(const std::string& name);

   void set_symbol(const std::string& name, const sexpr& expr);

   friend std::ostream& operator<<(std::ostream& os, const environment& expr);
 
private:
   environment* outer;
   symb_map_t symbols;
};

   
#endif
