/* Copyright 2016 Joaquin M Lopez Munoz.
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/poly_collection for library home page.
 */

#ifndef BOOST_POLY_COLLECTION_TEST_TEST_UTILITIES_HPP
#define BOOST_POLY_COLLECTION_TEST_TEST_UTILITIES_HPP

#if defined(_MSC_VER)
#pragma once
#endif

#include <array>
#include <boost/core/lightweight_test.hpp>
#include <boost/iterator/iterator_adaptor.hpp>
#include <boost/type_traits/has_equal_to.hpp>
#include <iterator>
#include <memory>
#include <type_traits>
#include <typeindex>

namespace test_utilities{

template<typename... Values>
void do_(Values...){}

template<typename Exception,typename F>
void check_throw_(F f)
{
  try{
    (void)f();
    BOOST_TEST(false);
  }
  catch(const Exception&){}
  catch(...){BOOST_TEST(false);}
}

template<typename Exception,typename... Fs>
void check_throw(Fs... f)
{
  do_((check_throw_<Exception>(f),0)...);
}

template<typename F1,typename F2>
struct compose_class
{
  F1 f1;
  F2 f2;

  template<typename T,typename... Args>
  auto operator()(T&& x,Args&&... args)
    ->decltype((this->f2)((this->f1)(
      std::forward<T>(x)),std::forward<Args>(args)...))
  {
    return f2(f1(std::forward<T>(x)),std::forward<Args>(args)...);
  }
};

template<typename F1,typename F2>
compose_class<F1,F2> compose(F1 f1,F2 f2)
{
  return {f1,f2};
}

template<typename F1,typename F2>
struct compose_all_class
{
  F1 f1;
  F2 f2;

  template<typename... Args>
  auto operator()(Args&&... args)
    ->decltype((this->f2)((this->f1)(std::forward<Args>(args))...))
  {
    return f2(f1(std::forward<Args>(args))...);
  }
};

template<typename F1,typename F2>
compose_all_class<F1,F2> compose_all(F1 f1,F2 f2)
{
  return {f1,f2};
}

using std::is_default_constructible;

using std::is_copy_constructible;

template<typename T>
using is_not_copy_constructible=std::integral_constant<
  bool,
  !std::is_copy_constructible<T>::value
>;

template<typename T>
using is_constructible_from_int=std::is_constructible<T,int>;

using std::is_copy_assignable;

template<typename T>
using is_not_copy_assignable=std::integral_constant<
  bool,
  !std::is_copy_assignable<T>::value
>;

template<typename T>
using is_equality_comparable=std::integral_constant<
  bool,
  boost::has_equal_to<T,T>::value
>;

template<typename T>
using is_not_equality_comparable=std::integral_constant<
  bool,
  !is_equality_comparable<T>::value
>;

template<
  typename T,
  typename std::enable_if<is_not_copy_constructible<T>::value>::type* =nullptr
>
typename std::remove_reference<T>::type&& constref_if_copy_constructible(T&& x)
{
  return std::move(x);
}

template<
  typename T,
  typename std::enable_if<is_copy_constructible<T>::value>::type* =nullptr
>
const T& constref_if_copy_constructible(T&& x)
{
  return x;
}

template<template<typename> class... Traits>
struct constraints;

template<>
struct constraints<>
{
  template<typename T>
  struct apply:std::true_type{};
};

template<
  template <typename> class Trait,
  template <typename> class... Traits
>
struct constraints<Trait,Traits...>
{
  template<typename T>
  struct apply:std::integral_constant<
    bool,
    Trait<T>::value&&constraints<Traits...>::template apply<T>::value
  >{};
};

template<typename... Ts>struct type_list{};

template<
  typename Constraints,template <typename...> class Template,
  typename TypeList,
  typename... Ts
>
struct instantiate_with_class;

template<
  typename Constraints,template <typename...> class Template,
  typename... Us
>
struct instantiate_with_class<Constraints,Template,type_list<Us...>>
{using type=Template<Us...>;};

template<
  typename Constraints,template <typename...> class Template,
  typename... Us,
  typename T,typename... Ts
>
struct instantiate_with_class<
  Constraints,Template,type_list<Us...>,T,Ts...
>:instantiate_with_class<
  Constraints,Template,
  typename std::conditional<
    Constraints::template apply<T>::value,
    type_list<Us...,T>,
    type_list<Us...>
  >::type,
  Ts...
>{};

template<
  typename Constraints,template <typename...> class Template,
  typename... Ts
>
using instantiate_with=typename instantiate_with_class<
  Constraints,Template,type_list<>,Ts...
>::type;

template<
  template <typename...> class Template,typename... Ts
>
using only_eq_comparable=instantiate_with<
  constraints<is_equality_comparable>,
  Template, Ts...
>;

template<typename T> struct identity{using type=T;};

template<typename Constraints,typename... Ts>
struct first_of_s{};

template<typename Constraints,typename T,typename... Ts>
struct first_of_s<Constraints,T,Ts...>:std::conditional<
  Constraints::template apply<T>::value,
  identity<T>,
  first_of_s<Constraints,Ts...>
>::type{};

template<typename Constraints,typename... Ts>
using first_of=typename first_of_s<Constraints,Ts...>::type;

template<
  typename Constraints,typename... Ts,
  typename PolyCollection,typename ValueFactory
>
void fill(PolyCollection& p,ValueFactory& v,int n)
{
  for(int i=0;i<n;++i){
    do_(
    (Constraints::template apply<Ts>::value?
      (p.insert(v.template make<Ts>()),0):0)...);
  }
}

template<typename PolyCollection>
bool is_first(
  const PolyCollection& p,typename PolyCollection::const_iterator it)
{
  return it==p.begin();
};

template<typename PolyCollection,typename Iterator>
bool is_first(const PolyCollection& p,std::type_index index,Iterator it)
{
  return &*it==&*p.begin(index);
};

template<typename PolyCollection,typename Iterator>
bool is_last(const PolyCollection& p,std::type_index index,Iterator it)
{
  return &*it==&*(p.end(index)-1);
};

template<typename T,typename PolyCollection,typename Iterator>
bool is_first(const PolyCollection& p,Iterator it)
{
  return &*it==&*p.template begin<T>();
};

template<typename T,typename PolyCollection,typename Iterator>
bool is_last(const PolyCollection& p,Iterator it)
{
  return &*it==&*(p.template end<T>()-1);
};

template<typename Iterator>
struct external_iterator_:
  public boost::iterator_adaptor<external_iterator_<Iterator>,Iterator>
{
  external_iterator_(const Iterator& it):
    external_iterator_::iterator_adaptor_{it}{}
};

template<typename Iterator>
external_iterator_<Iterator> external_iterator(Iterator it)
{
  return it;
}

template<typename Iterator>
struct unwrap_iterator_:public boost::iterator_adaptor<
  unwrap_iterator_<Iterator>,
  Iterator,
  typename std::iterator_traits<Iterator>::value_type::type
>
{
  unwrap_iterator_(const Iterator& it):
    unwrap_iterator_::iterator_adaptor_{it}{}
};

template<typename Iterator>
unwrap_iterator_<Iterator> unwrap_iterator(Iterator it)
{
  return it;
}

struct auto_increment
{
  template<typename T>
  T make(){return T(n++);}

  int n=0;
};

struct jammed_auto_increment
{
  template<typename T>
  T make(){return T(n++/10);}

  int n=0;
};

template<typename T>
struct rooted_allocator:std::allocator<T>
{
  template<typename U>
  struct rebind{using other=rooted_allocator<U>;};

  explicit rooted_allocator(int):root{this}{}
  template<typename U>
  rooted_allocator(const rooted_allocator<U>& x):root{x.root}{}

  const void* root;
};

template<typename PolyCollection,template<typename> class Allocator>
struct realloc_poly_collection_t;

template<typename PolyCollection,template<typename> class Allocator>
using realloc_poly_collection=
  typename realloc_poly_collection_t<PolyCollection,Allocator>::type;

template<
  template<typename,typename> class PolyCollection,
  typename T,typename OriginalAllocator,
  template<typename> class Allocator
>
struct realloc_poly_collection_t<
  PolyCollection<T,OriginalAllocator>,Allocator
>
{
  using value_type=typename PolyCollection<T,OriginalAllocator>::value_type;
  using type=PolyCollection<T,Allocator<value_type>>;
};

template<std::size_t N>
struct layout_data
{
  std::array<const void*,N> datas;
  std::array<std::size_t,N> sizes;

  bool operator==(const layout_data& x)const
  {
    return datas==x.datas&&sizes==x.sizes;
  }
};

template<typename... Types,typename PolyCollection>
layout_data<sizeof...(Types)> get_layout_data(const PolyCollection& p)
{
  return{
    {{(p.template is_registered<Types>()?
      &*p.template begin<Types>():nullptr)...}},
    {{(p.template is_registered<Types>()?
      p.template size<Types>():0)...}}
  };
}

} /* namespace test_utilities */

#endif
