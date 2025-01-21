#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include <variant>


template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
// explicit deduction guide (not needed as of C++20)
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

template <typename Val, typename... Ts>
auto match(Val val, Ts... ts) { 
    // https://en.cppreference.com/w/cpp/utility/variant/visit
    return std::visit(overloaded{ts...}, val);  
}