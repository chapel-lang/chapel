//# stl.h -*- C++ -*-
//
// For loop macros for use with STL container classes
//
//########################################################################

#ifdef STL_H
#error Multiple inclusion.
#else
#define STL_H

//
// These macros are intended to mimic the operation of the former
// forv_Vec-like macros, which declare the current element and make it
// available in the VAL argument.  So, for example
// "for_vector(FnSymbol, fn, gFnSymbols)" makes the declaration:
// FnSymbol* fn; and returns one FnSymbol* from the global list of
// function symbol(pointer)s on each iteration.
//
// To implement iteration compatibly (with, for example, forv_Vec), we
// need both an index (iterator) and the value obtained by
// dereferencing it.  Since it is not possible to declare variables
// with different base types in a for loop, we declare the hidden
// iterator internally, and push the declaration of the value outside
// the loop.  That is, the client code has to provide the declaration
// where formerly it did not.
//
// CAVEAT: Using the same name for the value in the macro may lead to
// compilation errors if both macro invocations are enclosed in the
// same scope.
//
// NOTE: We should consider replacing these with BOOST_FOREACH once
// more of the Boost libraries are accepted into the C++ standard.
//
#define for_vector(TYPE, VAL, VEC) TYPE* VAL;                 \
  for (std::vector<TYPE*>::iterator _i_##VAL = (VEC).begin(); \
       (VAL = (_i_##VAL != (VEC).end()) ? *_i_##VAL : (TYPE*)0) ; _i_##VAL++ )

#define for_set(TYPE, VAL, VEC) TYPE* VAL;                 \
  for (std::set<TYPE*>::iterator _i_##VAL = (VEC).begin(); \
       (VAL = (_i_##VAL != (VEC).end()) ? *_i_##VAL : (TYPE*)0) ; _i_##VAL++ )

#define for_queue(TYPE, VAL, VEC) TYPE* VAL;                 \
  for (std::queue<TYPE*>::iterator _i_##VAL = (VEC).begin(); \
       (VAL = (_i_##VAL != (VEC).end()) ? *_i_##VAL : (TYPE*)0) ; _i_##VAL++ )

#endif
