/* Copyright 2016 Joaquin M Lopez Munoz.
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/poly_collection for library home page.
 */

#ifndef BOOST_FLYWEIGHT_TEST_ANY_TYPES_HPP
#define BOOST_FLYWEIGHT_TEST_ANY_TYPES_HPP

#if defined(_MSC_VER)
#pragma once
#endif

#include <boost/poly_collection/any_collection.hpp>
#include <boost/type_erasure/operators.hpp>

namespace any_types{

struct incrementable1
{
  incrementable1(int n):n{n}{}
  incrementable1(incrementable1&&)=default;
  incrementable1(const incrementable1&)=delete;
  incrementable1& operator=(incrementable1&&)=default;
  incrementable1& operator=(const incrementable1&)=delete;
  bool operator==(const incrementable1& x)const{return n==x.n;}
  incrementable1& operator++(){++n;return *this;}
  int n;
};

struct incrementable3
{
  incrementable3():n{-1}{}
  incrementable3(int n):n{(double)n}{}
  incrementable3& operator++(){++n;return *this;}
  double n;
};

using concept=boost::type_erasure::incrementable<>;
using collection=boost::any_collection<concept>;

using t1=incrementable1;
using t2=double;
using t3=incrementable3;
using t4=int;
using t5=char;

} /* namespace any_types*/

#endif
