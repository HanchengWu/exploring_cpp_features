#include <iostream>
#include <cstdlib>
#include <utility>
#include <typeinfo>
#include <string>
#include <type_traits>
#include <memory>
#ifndef _MSC_VER
#   include <cxxabi.h>
#endif

#define type_name type_name2

template <typename T>
std::string
type_name1()
{
      typedef typename std::remove_reference<T>::type TR;
      //void(*)(void*) is a pointer that points to function   void func(void *p);
      std::unique_ptr<char, void(*)(void*)> own
          (
#ifndef _MSC_VER
           abi::__cxa_demangle(typeid(TR).name(), nullptr,nullptr, nullptr),
#else
           nullptr,
#endif
           std::free
          );
          std::string r = own != nullptr ? own.get() : typeid(TR).name();
          if (std::is_const<TR>::value)
                    r += " const";
          if (std::is_volatile<TR>::value)
                    r += " volatile";
          if (std::is_lvalue_reference<T>::value)
                    r += "&";
          else if (std::is_rvalue_reference<T>::value)
                    r += "&&";
          return r;
}


template <typename T>
constexpr auto type_name2()
{
      std::string_view name, prefix, suffix;
#ifdef __clang__
      name = __PRETTY_FUNCTION__;
      prefix = "auto type_name() [T = ";
      suffix = "]";
      std::cout << name << std::endl;
#elif defined(__GNUC__)
      name = __PRETTY_FUNCTION__;
      prefix = "constexpr auto type_name() [with T = ";
      suffix = "]";
      std::cout << name << std::endl;
#elif defined(_MSC_VER)
      name = __FUNCSIG__;
      prefix = "auto __cdecl type_name<";
      suffix = ">(void)";
      std::cout << name << std::endl;
#endif
      name.remove_prefix(prefix.size());
      name.remove_suffix(suffix.size());
      return name;
}  


int main(){

  const int ci = 0;
  int i = 0;
  std::cout << type_name<decltype(ci)>() << '\n';
  std::cout << type_name<decltype((i))>() << '\n';
  std::cout << type_name<decltype(i+0)>() << '\n';
return 0;
}

