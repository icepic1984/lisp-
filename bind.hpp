#ifndef BIND_HPP
#define BIND_HPP
template <typename F, typename X>
struct bind_impl 
{
   typedef typename F::result_t result_t;
   X& x_; 
   F f_;
   bind_impl(F f, X& x) : x_(x), f_(f) {}
   
   template <typename Y>
   typename F::result_t operator()(Y& y) const {
	   return f_(x_, y);
   }
   
   template <typename Y>
   typename F::result_t operator()(Y const& y) const {
	   return f_(x_, y);
   }
};

template <typename F, typename X>
bind_impl<F, X const> bind(F f, X const& x)
{
	return bind_impl<F, X const>(f, x);
}

template <typename F, typename X>
bind_impl<F, X> bind(F f, X& x)
{
	return bind_impl<F, X>(f, x);
}

#endif 
