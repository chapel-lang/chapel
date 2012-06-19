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
// To accomplish the iteration without internal knowledge of the vector class,
// we need to maintain both an iteration index and the current element.
// Wc can declare multiple variables in the initializer expression for a for loop,
// but (in accordance with standard C++ syntax) these variables must all have the 
// same set of declaration specifiers (which is the fundamental type portion of a
// declaration, trimming off pointer, reference and array modifiers).
// The trick we use is to declare the iteration index as a pointer and then cast it
// to an integer whenever we reference it.  The std::vector class supports indexing
// into the data structure (using operator[]), so we can enumerate the elements in
// the vector that way.
//
// Using a pointer to contain an integer value is pretty hideous, but the best we can 
// do under C++ syntax restrictions.  It seems pretty safe to assume that a pointer 
// has at least as many bits as a size_t.
//
#define for_vector(TYPE, VAL, VEC) for (TYPE *VAL, *_i_##VAL = 0; \
  ((size_t)_i_##VAL) < (VEC).size() ? (VAL = (VEC)[(size_t)_i_##VAL], true) : false; \
  ++(*(size_t*)&_i_##VAL))

#endif
