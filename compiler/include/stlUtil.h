/*
 * Copyright 2004-2020 Cray Inc.
 * Other additional copyright holders may be indicated within.
 *
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 *
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

//# stl.h -*- C++ -*-
//
// For loop macros for use with STL container classes
//
//########################################################################

#ifndef STL_UTIL_H
#define STL_UTIL_H

//
// These macros are intended to mimic the operation of the former
// forv_Vec-like macros, which declare the current element and make it
// available in the VAL argument.  So, for example
// "for_vector(FnSymbol, fn, gFnSymbols)" makes the declaration:
// FnSymbol* fn; and returns one FnSymbol* from the global list of
// function symbol(pointer)s on each iteration.
//
// Implementation Note:
// To implement iteration compatibly (with, for example, forv_Vec), we
// need both an index (iterator) and the value obtained by
// dereferencing it.  Since it is not possible to declare variables
// with different base types in a for loop, we declare unique-ified
// temporary variables to store a reference to the collection, the
// begin iterator, and the end iterator. The loop still declares
// the VAL argument since its name cannot be unique-ified. In this
// way, the implementation takes care to evaluate VEC only once
// and allows multiple loops using the same index variable.
//
// TODO: We should consider replacing these with something else.
// In C++11 perhaps these macros could be written
//  for(TYPE* VAL: VEC)
//
// NOTE: unlike forv_Vec et al., these macros require enclosing { }
// when they are inside an if or similar, e.g.:
//
//   if (...some condition...) { // must use open brace here
//     for_vector(Expr, expr, myVector)
//       printf("%d", expr->id); // here, simple cases are OK without { }
//   }
//

#define STL_UTIL_TOK_PASTE_INTERNAL(a, b) a ## b
#define STL_UTIL_TOK_PASTE(a, b) STL_UTIL_TOK_PASTE_INTERNAL(a,b)
#define STL_UTIL_UNIQUE(NAME) STL_UTIL_TOK_PASTE(NAME, __LINE__)

#define for_vector(TYPE, VAL, VEC) \
  const std::vector<TYPE*> & STL_UTIL_UNIQUE(for_vector_vec) = (VEC); \
  std::vector<TYPE*>::const_iterator STL_UTIL_UNIQUE(for_vector_iter) = \
      STL_UTIL_UNIQUE(for_vector_vec).begin(); \
  std::vector<TYPE*>::const_iterator STL_UTIL_UNIQUE(for_vector_end) = \
      STL_UTIL_UNIQUE(for_vector_vec).end(); \
  for (TYPE * VAL = NULL ; \
       (VAL = ( STL_UTIL_UNIQUE(for_vector_iter) != \
                STL_UTIL_UNIQUE(for_vector_end) ) ? \
              * STL_UTIL_UNIQUE(for_vector_iter) : \
              (TYPE*)0) ; \
       ++STL_UTIL_UNIQUE(for_vector_iter) )

#define for_vector_allowing_0s(TYPE, VAL, VEC)                           \
  const std::vector<TYPE*> & STL_UTIL_UNIQUE(for_vector_vec) = (VEC);    \
  std::vector<TYPE*>::const_iterator STL_UTIL_UNIQUE(for_vector_iter) =  \
      STL_UTIL_UNIQUE(for_vector_vec).begin();                           \
  std::vector<TYPE*>::const_iterator STL_UTIL_UNIQUE(for_vector_end) =   \
      STL_UTIL_UNIQUE(for_vector_vec).end();                             \
  bool STL_UTIL_UNIQUE(for_vector_cont);                                 \
  for (TYPE * VAL = NULL ;                                               \
       (STL_UTIL_UNIQUE(for_vector_cont) =                               \
              ( STL_UTIL_UNIQUE(for_vector_iter) !=                      \
                STL_UTIL_UNIQUE(for_vector_end) )),                      \
       (VAL =   STL_UTIL_UNIQUE(for_vector_cont) ?                       \
              * STL_UTIL_UNIQUE(for_vector_iter) :                       \
                (TYPE*)0),                                               \
       STL_UTIL_UNIQUE(for_vector_cont);                                 \
       ++STL_UTIL_UNIQUE(for_vector_iter) )

#define for_set(TYPE, VAL, VEC) \
  const std::set<TYPE*> & STL_UTIL_UNIQUE(for_set_set) = (VEC); \
  std::set<TYPE*>::const_iterator STL_UTIL_UNIQUE(for_set_iter) = \
      STL_UTIL_UNIQUE(for_set_set).begin(); \
  std::set<TYPE*>::const_iterator STL_UTIL_UNIQUE(for_set_end) = \
      STL_UTIL_UNIQUE(for_set_set).end(); \
  for (TYPE * VAL = NULL ; \
       (VAL = ( STL_UTIL_UNIQUE(for_set_iter) != \
                STL_UTIL_UNIQUE(for_set_end) ) ? \
              * STL_UTIL_UNIQUE(for_set_iter) : \
              (TYPE*)0) ; \
       ++STL_UTIL_UNIQUE(for_set_iter) )

#endif
