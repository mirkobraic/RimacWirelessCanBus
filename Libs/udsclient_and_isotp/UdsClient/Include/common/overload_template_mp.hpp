#pragma once

// This metaprogramming allows easier usage of std::variant with std::visit
// https://www.bfilipek.com/2018/06/variant.html#overload

template <class... Ts>
struct overload : Ts... {
  using Ts::operator()...;
};
template <class... Ts>
overload(Ts...)->overload<Ts...>;
