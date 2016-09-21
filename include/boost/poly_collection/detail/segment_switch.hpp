/* Copyright 2016 Joaquin M Lopez Munoz.
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/poly_collection for library home page.
 */

#ifndef BOOST_POLY_COLLECTION_DETAIL_SEGMENT_SWITCH_HPP
#define BOOST_POLY_COLLECTION_DETAIL_SEGMENT_SWITCH_HPP

#if defined(_MSC_VER)
#pragma once
#endif

#include <typeinfo>
#include <utility>

namespace boost{

namespace poly_collection{

namespace detail{

/* segment_switch<Ts...>(Default d,Fs... fs) returns a visitor over a segment
 * s that calls Fi(s) if the type of the elements of s is Ti, and Default(s)
 * otherwise.
 */

template<typename... Ts>
struct segment_switch_list{};

template<typename TypeList,typename FunctionList,typename Default>
struct segment_switcher;

template<
  typename T,typename... Ts,
  typename F,typename... Fs,
  typename Default
>
struct segment_switcher<
  segment_switch_list<T,Ts...>,
  segment_switch_list<F,Fs...>,
  Default
>:segment_switcher<
  segment_switch_list<Ts...>,
  segment_switch_list<Fs...>,
  Default
>
{
  using super=segment_switcher<
    segment_switch_list<Ts...>,
    segment_switch_list<Fs...>,
    Default
  >;

  segment_switcher(F f,Fs... fs,Default d):super{fs...,d},f{f}{}

  template<typename Segment>
  auto operator()(Segment s)->decltype(std::declval<F>()(s))
  {
    if(s.type_index()==typeid(T))return f(s);
    else return super::operator()(s);
  }

  F f;
};

template<typename Default>
struct segment_switcher<
  segment_switch_list<>,
  segment_switch_list<>,
  Default
>
{
  segment_switcher(Default d):d{d}{}

  template<typename Segment>
  auto operator()(Segment s)->decltype(std::declval<Default>()(s))
  {
    return d(s);
  }

  Default d;
};

template<
  typename... Ts,
  typename... Fs,
  typename Default
>
segment_switcher<
  segment_switch_list<Ts...>,
  segment_switch_list<Fs...>,
  Default
>
segment_switch(Default d,Fs... fs)
{
  return {fs...,d};
}

} /* namespace poly_collection::detail */

} /* namespace poly_collection */

} /* namespace boost */

#endif
