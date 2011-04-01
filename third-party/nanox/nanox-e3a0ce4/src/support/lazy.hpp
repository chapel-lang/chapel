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

#ifndef _NANOS_LAZY_INIT
#define _NANOS_LAZY_INIT

#include "lazy_decl.hpp"

#define likely(x)       __builtin_expect((x),1)
#define unlikely(x)     __builtin_expect((x),0)

template <class T>
inline void LazyInit<T>::construct ()
{
   _ptr = new (_storage) T();
}

template <class T>
inline void LazyInit<T>::destroy ()
{
    _ptr->~T();
}

template <class T>
inline LazyInit<T>::~LazyInit ()
{
   if (_ptr != NULL) destroy();
}

template <class T>
inline T * LazyInit<T>::operator-> ()
{
   if (unlikely(_ptr == NULL)) construct();
   return _ptr;
}

template <class T>
inline T & LazyInit<T>::operator* ()
{
   return *(operator->());
}

template <class T>
inline bool LazyInit<T>::isInitialized()
{
   return _ptr != NULL;
}

#endif
