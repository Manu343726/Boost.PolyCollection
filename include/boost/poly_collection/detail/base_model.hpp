/* Copyright 2016 Joaquin M Lopez Munoz.
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/poly_collection for library home page.
 */

#ifndef BOOST_POLY_COLLECTION_DETAIL_BASE_MODEL_HPP
#define BOOST_POLY_COLLECTION_DETAIL_BASE_MODEL_HPP

#if defined(_MSC_VER)
#pragma once
#endif

#include <boost/poly_collection/detail/is_final.hpp>
#include <boost/poly_collection/detail/packed_segment.hpp>
#include <boost/poly_collection/detail/stride_iterator.hpp>
#include <boost/poly_collection/detail/value_holder.hpp>
#include <boost/utility/addressof.hpp>
#include <memory>
#include <type_traits>
#include <typeinfo>
#include <utility>

namespace boost{

namespace poly_collection{

namespace detail{

/* model for base_collection */

template<typename Base>
struct base_model
{
  using value_type=Base;
  template<typename Derived>
  using is_subtype=std::is_base_of<Base,Derived>;
  template<typename T>
  using is_terminal=is_final<T>; //TODO: should we say !is_polymorhpic||is_final?
  template<typename T>
  static const std::type_info& sub_typeid(const T& x){return typeid(x);}
  template<typename T>
  static void* subtype_address(T& x){return boost::addressof(x);}
  template<typename T>
  static const void* subtype_address(const T& x){return boost::addressof(x);}

  using base_iterator=stride_iterator<Base>;
  using const_base_iterator=stride_iterator<const Base>;
  using base_sentinel=Base*;
  using const_base_sentinel=const Base*;
  template<typename Derived>
  using iterator=Derived*;
  template<typename Derived>
  using const_iterator=const Derived*;
  using segment_backend=detail::segment_backend<base_model>;

  static base_iterator nonconst_iterator(const_base_iterator it)
  {
    return {
      const_cast<value_type*>(static_cast<const value_type*>(it)),
      it.stride()
    };
  }

  template<typename T>
  static iterator<T> nonconst_iterator(const_iterator<T> it)
  {
    return const_cast<iterator<T>>(it);
  }

  template<typename Derived,typename Allocator>
  static segment_backend* make(const Allocator& al)
  {
    return new packed_segment<
      base_model,
      Derived,
      typename std::allocator_traits<Allocator>::
        template rebind_alloc<Derived>
    >{al};
  }

private:
  template<typename,typename,typename>
  friend class packed_segment;

  template<typename Derived>
  using element_type=value_holder<Derived>;

  template<typename Derived>
  static const Base* value_ptr(const value_holder<Derived>* p)noexcept
  {
    return reinterpret_cast<const Derived*>(
      static_cast<const value_holder_base<Derived>*>(p));
  }

  template<typename Derived>
  static const value_holder<Derived>* element_ptr(const Derived* p)noexcept
  {
    return static_cast<const value_holder<Derived>*>(
      reinterpret_cast<const value_holder_base<Derived>*>(p));
  }
};

} /* namespace poly_collection::detail */

} /* namespace poly_collection */

} /* namespace boost */

#endif
