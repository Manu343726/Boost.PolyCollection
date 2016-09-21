/* Copyright 2016 Joaquin M Lopez Munoz.
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/poly_collection for library home page.
 */

#ifndef BOOST_POLY_COLLECTION_DETAIL_VALUE_HOLDER_HPP
#define BOOST_POLY_COLLECTION_DETAIL_VALUE_HOLDER_HPP

#if defined(_MSC_VER)
#pragma once
#endif

#include <boost/type_traits/has_equal_to.hpp>
#include <boost/poly_collection/detail/is_nothrow_eq_comparable.hpp>
#include <boost/poly_collection/exception.hpp>
#include <new>
#include <type_traits>
#include <utility>

namespace boost{

namespace poly_collection{

namespace detail{

/* Segments of a poly_collection maintain vectors of value_holder<T>
 * rather than directly T. This serves two purposes:
 *  - value_holder<T> is copy constructible and equality comparable even if T
 *    is not: executing the corresponding op results in a reporting exception
 *    being thrown. This allows the segment to offer its full virtual
 *    interface regardless of the properties of the concrete class contained.
 *  - value_holder<T> supports the type-erased emplace mechanism required
 *    by segment_backend.
 *
 *  A pointer to value_holder_base<T> can be reinterpret_cast'ed to T*.
 */

template<typename T>
class value_holder_base
{
protected:
  typename std::aligned_storage<sizeof(T),alignof(T)>::type s;
};

template<typename T>
class value_holder:public value_holder_base<T>
{
  void*       data()noexcept{return reinterpret_cast<void*>(&this->s);}
  const void* data()const noexcept
                {return reinterpret_cast<const void*>(&this->s);}

  T&       value()noexcept{return *static_cast<T*>(data());}
  const T& value()const noexcept{return *static_cast<const T*>(data());}

public:
  value_holder(const T& x)
    noexcept(std::is_nothrow_copy_constructible<T>::value)
    {copy(x);}
  value_holder(T&& x)
    noexcept(std::is_nothrow_move_constructible<T>::value)
    {::new (data()) T(std::move(x));}
  value_holder(void (* emplace)(void*,void*),void* arg)
    {emplace(data(),arg);}
  value_holder(const value_holder& x)
    noexcept(std::is_nothrow_copy_constructible<T>::value)
    {copy(x.value());}
  value_holder(value_holder&& x)
    noexcept(std::is_nothrow_move_constructible<T>::value)
    {::new (data()) T(std::move(x.value()));}

  value_holder& operator=(const value_holder& x)=delete;
  value_holder& operator=(value_holder&& x)
    noexcept(std::is_nothrow_move_assignable<T>::value)
  {
    value()=std::move(x.value());
    return *this;
  }

  ~value_holder()
    noexcept(std::is_nothrow_destructible<T>::value)
    {value().~T();}

  friend bool operator==(const value_holder& x,const value_holder& y)
    noexcept(is_nothrow_equality_comparable<T>::value)
  {
    return x.equal(y.value());
  }

private:
  using is_copy_constructible=std::is_copy_constructible<T>;
  using is_copy_assignable=std::is_copy_assignable<T>;
  using is_equality_comparable=std::integral_constant<
    bool,
    boost::has_equal_to<T,T>::value
  >;

  void copy(const T& x){copy(x,is_copy_constructible{});}

  void copy(const T& x,std::true_type)
  {
    ::new (data()) T(x);
  }

  void copy(const T&,std::false_type)
  {
    throw not_copy_constructible{typeid(T)};
  }

  bool equal(const T& x)const{return equal(x,is_equality_comparable{});}

  bool equal(const T& x,std::true_type)const
  {
    return value()==x;
  }

  bool equal(const T&,std::false_type)const
  {
    throw not_equality_comparable{typeid(T)};
  }
};

} /* namespace poly_collection::detail */

} /* namespace poly_collection */

} /* namespace boost */

#endif
