/*************************************************************************************/
/*      Copyright 2009 Barcelona Supercomputing Center                               */
/*                                                                                   */
/*      This file is part of the NANOS++ library.                                    */
/*                                                                                   */
/*      NANOS++ is free software: you can redistribute it and/or modify              */
/*      it under the terms of the GNU Lesser General Public License as published by  */
/*      the Free Software Foundation, either version 3 of the License, or            */
/*      (at your option) any later version.                                          */
/*                                                                                   */
/*      NANOS++ is distributed in the hope that it will be useful,                   */
/*      but WITHOUT ANY WARRANTY; without even the implied warranty of               */
/*      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                */
/*      GNU Lesser General Public License for more details.                          */
/*                                                                                   */
/*      You should have received a copy of the GNU Lesser General Public License     */
/*      along with NANOS++.  If not, see <http://www.gnu.org/licenses/>.             */
/*************************************************************************************/

#ifndef _NANOS_COMPATIBILITY_HPP
#define _NANOS_COMPATIBILITY_HPP

// Define GCC Version
#define GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)

// Define a linker section with GCC
#define LINKER_SECTION(name,type,nop) \
    __attribute__((weak, section( #name ))) type __section_##name = nop; \
    extern type __start_##name; \
    extern type __stop_##name;


#if __CUDACC__

#define BROKEN_COMPARE_AND_SWAP

#endif

// compiler issues

#if __GXX_EXPERIMENTAL_CXX0X__

#include <unordered_map>
#include <memory>

namespace TR1 = std;

#else

#include <tr1/unordered_map>
#include <tr1/memory>

namespace TR1 = std::tr1;

#endif

#ifdef __GNUC__
#if __GNUC__ == 4 && __GNUC_MINOR__ < 2

namespace std {
#ifndef __GXX_EXPERIMENTAL_CXX0X__
namespace tr1{
#endif // __GXX_EXPERIMENTAL_CXX0X__

/* Specialize hash for unsigned long long allows unordered_map<uint64_t, xxx> when compiling for 32 bits */
template<> struct hash<unsigned long long> : public std::unary_function<unsigned long long, std::size_t> { std::size_t operator()(unsigned long long val) const { return static_cast<std::size_t>(val); } };
}

#ifndef __GXX_EXPERIMENTAL_CXX0X__
}
#endif // __GXX_EXPERIMENTAL_CXX0X__

#endif // __GNUC__ == 4 && __GNUC_MINOR__ < 2
#endif // __GNUC__


#ifdef __GNUC__
// Dan Tsafrir [11/2/2011]: ugly hack to match the ugliness it fixes.
//
// Explanation:
//
// For the statements to which this macro is applied, gcc-4.1
//   (a) creates a temporary,
//   (b) copies it using the copy ctor to another temporary,
//   (c) invokes the operator=.
// But since the copy ctor in (b) does not exist => compile error.
// This macro prevents (b) from happening.
#if __GNUC__ == 4 && (__GNUC_MINOR__ == 1 || __GNUC_MINOR__ == 2)
#define ASSIGN_EVENT(event,type,args) do {type tmp_event args; event = tmp_event;} while(0)
#else
#define ASSIGN_EVENT(event,type,args) event = type args
#endif // __GNUC__ == 4 && (__GNUC_MINOR__ == 1 || __GNUC_MINOR__ == 2)
#endif // __GNUC__

#ifdef BROKEN_COMPARE_AND_SWAP

bool __sync_bool_compare_and_swap( int *ptr, int oldval, int newval );

#endif

#endif

