/* Copyright 2016 Joaquin M Lopez Munoz.
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/poly_collection for library home page.
 */

#ifndef BOOST_POLY_COLLECTION_ALGORITHM_HPP
#define BOOST_POLY_COLLECTION_ALGORITHM_HPP

#if defined(_MSC_VER)
#pragma once
#endif

#include <algorithm>
#include <boost/poly_collection/detail/poly_collection.hpp>
#include <boost/poly_collection/detail/segment_switch.hpp>

/* Improved performance versions of std algorithms over poly_collection.
 * poly_collection::alg(p,...) is expected to be faster than
 * std::alg(p.begin(),p.end(),...) because the latter does a traversal
 * over a segmented structured, where incrementing requires checking
 * for segment change, whereas the former for-loops over flat segments.
 * Additionally, poly_collection::alg<Ti...>(p,...,f) *restitutes* Ti when
 * passing elements to f, i.e. if the concrete type of the element is Ti
 * then f is invoked with a [const] Ti&, which can dramatically improve
 * performance when f has specific overloads for Ti (like, for instance,
 * generic lambdas) as static optimization can kick in (devirtualization
 * being a particular example).
 */

namespace boost{

namespace poly_collection{

namespace detail{

template<typename T,typename SegmentInfo,typename Function>
struct for_each_case
{
  void operator()(SegmentInfo s)
  {
    for(auto it=s.template begin<T>(),end=s.template end<T>();it!=end;++it){
      f(*it);
    };
  }

  Function& f;
};

template<typename... Ts,typename SegmentTraversalInfo,typename Function>
Function for_each(SegmentTraversalInfo st,Function f)
{
  using segment_info=decltype(*st.begin());

  auto sw=detail::segment_switch<Ts...>(
    [&](segment_info s){for(auto& x:s)f(x);},
    for_each_case<Ts,segment_info,Function>{f}...
  );

  for(auto s:st)sw(s);
  return std::move(f);
}

} /* namespace poly_collection::detail */

template<typename... Ts,typename M,typename A,typename Function>
Function for_each(detail::poly_collection<M,A>& p,Function f)
{
  return std::move(
    detail::for_each<Ts...>(p.segment_traversal(),std::move(f)));
}

template<typename... Ts,typename M,typename A,typename Function>
Function for_each(const detail::poly_collection<M,A>& p,Function f)
{
  return std::move(
    detail::for_each<Ts...>(p.segment_traversal(),std::move(f)));
}

} /* namespace poly_collection */

} /* namespace boost */

#endif
