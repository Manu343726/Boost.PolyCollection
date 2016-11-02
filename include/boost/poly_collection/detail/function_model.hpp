/* Copyright 2016 Joaquin M Lopez Munoz.
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/poly_collection for library home page.
 */

#ifndef BOOST_POLY_COLLECTION_DETAIL_FUNCTION_MODEL_HPP
#define BOOST_POLY_COLLECTION_DETAIL_FUNCTION_MODEL_HPP

#if defined(_MSC_VER)
#pragma once
#endif

#include <boost/poly_collection/detail/callable_wrapper.hpp>
#include <boost/poly_collection/detail/callable_wrapper_iterator.hpp>
#include <boost/poly_collection/detail/is_callable.hpp>
#include <boost/poly_collection/detail/segment_backend.hpp>
#include <boost/poly_collection/detail/split_segment.hpp>
#include <boost/utility/addressof.hpp>
#include <memory>
#include <type_traits>
#include <typeinfo>
#include <utility>

namespace boost{

namespace poly_collection{

namespace detail{

/* model for function_collection */

template<typename Signature>
struct function_model;

/* is_terminal defined out-class to allow for partial specialization */

template<typename T>
struct function_model_is_terminal:std::true_type{};

template<typename Signature>
struct function_model_is_terminal<callable_wrapper<Signature>>:
  std::false_type{};

template<typename R,typename... Args>
struct function_model<R(Args...)>
{
  using value_type=callable_wrapper<R(Args...)>;

  template<typename Callable>
  using is_subtype=is_callable<Callable&(Args...),R>;

  template<typename T>
  using is_terminal=function_model_is_terminal<T>;

  template<typename T>
  static const std::type_info& sub_typeid(const T&){return typeid(T);}

  template<typename Signature>
  static const std::type_info& sub_typeid(
    const callable_wrapper<Signature>& f)
  {
    return f.target_type();
  }

  template<typename T>
  static void* subtype_address(T& x){return boost::addressof(x);}

  template<typename T>
  static const void* subtype_address(const T& x){return boost::addressof(x);}

  template<typename Signature>
  static void* subtype_address(callable_wrapper<Signature>& f)
  {
    return f.data();
  }
  
  template<typename Signature>
  static const void* subtype_address(const callable_wrapper<Signature>& f)
  {
    return f.data();
  }

  using base_iterator=callable_wrapper_iterator<value_type>;
  using const_base_iterator=callable_wrapper_iterator<const value_type>;
  using base_sentinel=value_type*;
  using const_base_sentinel=const value_type*;
  template<typename Callable>
  using iterator=Callable*;
  template<typename Callable>
  using const_iterator=const Callable*;
  using segment_backend=detail::segment_backend<function_model>;

  static base_iterator nonconst_iterator(const_base_iterator it)
  {
    return base_iterator{
      const_cast<value_type*>(static_cast<const value_type*>(it))};
  }

  template<typename T>
  static iterator<T> nonconst_iterator(const_iterator<T> it)
  {
    return const_cast<iterator<T>>(it);
  }

  template<typename Callable,typename Allocator>
  static segment_backend* make(const Allocator& al)
  {
    return new split_segment<
      function_model,
      Callable,
      typename std::allocator_traits<Allocator>::
        template rebind_alloc<Callable>
    >{al};
  }
};

} /* namespace poly_collection::detail */

} /* namespace poly_collection */

} /* namespace boost */

#endif
