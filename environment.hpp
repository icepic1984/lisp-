#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP
#include <functional>
#include <vector>
#include <unordered_map>
#include <boost/optional.hpp>
#include <memory>
#include "sexpr.hpp"


using environment_ptr = std::shared_ptr<environment>;
using environment_weak = std::weak_ptr<environment>;

class environment : public std::enable_shared_from_this<environment>
{
public:
   using symb_map_t = std::unordered_map<std::string,sexpr>;

   using result_t = boost::optional<sexpr>;

   environment_ptr create();

   environment(environment* env = nullptr);

   result_t find_symbol(const std::string& name);

   sexpr update_symbol(const std::string& name, const sexpr& expr);
   
   sexpr set_symbol(const std::string& name, const sexpr& expr);

   friend std::ostream& operator<<(std::ostream& os, const environment& expr);
private:

   void init();
   
   environment* get_environment(const std::string& name);
   
   environment* outer;

   symb_map_t symbols;
};

   
#endif
