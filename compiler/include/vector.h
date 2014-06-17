/**************************************************************************
  Copyright (c) 2004-2013, Cray Inc.  (See LICENSE file for more details)
**************************************************************************/


//# vector.h -*- C++ -*-
//########################################################################

#ifdef VECTOR_H
#error Multiple inclusion.
#else
#define VECTOR_H

#include <vector>

// This macro is intended to mimic the operation of the former forv_Vec macro, 
// which declares the current element and makes it available in the VAL argument.
// So, for example "for_vector(FnSymbol, fn, gFnSymbols)" makes the declaration:
//  FnSymbol* fn;
// and returns one FnSymbol* from the global list of function symbol(pointer)s on
// each iteration.
//
// To implement iteration compatibly (with forv_Vec), we need both an index (iterator) 
// and the value obtained by dereferencing it.  Since it is not possible to declare 
// variables with different base types in a for loop, we declare the hidden iterator
// internally, and push the declaration of the value outside the loop.  That is, the
// client code has to provide the declaration where formerly it did not.
//
#define for_vector(TYPE, VAL, VEC) TYPE* VAL;                 \
  for (std::vector<TYPE*>::iterator _i_##VAL = (VEC).begin(); \
       (VAL = (_i_##VAL != (VEC).end()) ? *_i_##VAL : (TYPE*)0) ; _i_##VAL++ )

#endif
