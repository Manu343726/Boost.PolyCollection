/* Copyright 2016 Joaquin M Lopez Munoz.
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/poly_collection for library home page.
 */

#ifndef BOOST_FLYWEIGHT_TEST_FUNCTION_TYPES_HPP
#define BOOST_FLYWEIGHT_TEST_FUNCTION_TYPES_HPP

#if defined(_MSC_VER)
#pragma once
#endif

#include <boost/poly_collection/function_collection.hpp>

namespace function_types{

struct function1 final
{
  function1(int n):n{n}{}
  function1(function1&&)=default;
  function1(const function1&)=delete;
  function1& operator=(function1&&)=default;
  function1& operator=(const function1&)=delete;
  int operator()(int)const{return n;}
  bool operator==(const function1& x)const{return n==x.n;}
  int n;
};

struct function2
{
  function2(int n):n{n}{}
  int operator()(int x)const{return x*n;}
  bool operator==(const function2& x)const{return n==x.n;}
  int n;
};

struct function3
{
  function3():n{-1}{}
  function3(int n):n{n}{}
  int operator()(int x)const{return x*x*n;}
  int n;
};

struct function4:function3
{
  using function3::function3;
  int operator()(int x)const{return -(this->function3::operator()(x));}
  bool operator==(const function4& x)const{return n==x.n;}
};

struct function5
{
  function5(int n):n{n}{}
  int operator()(int x)const{return x*x*x*n;}
  int n;
};

using signature=int(int);
using collection=boost::function_collection<signature>;

using t1=function1;
using t2=function2;
using t3=function3;
using t4=function4;
using t5=function5;

} /* namespace function_types*/

#endif
