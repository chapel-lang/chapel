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

#ifndef _NANOS_FUNCTORS
#define _NANOS_FUNCTORS

#include <string>
#include "functors_decl.hpp"

template<typename T>
void deleter( T *p ) { delete p; }

template<typename T>
void pair_deleter1 ( std::pair<std::string,T *> pair ) { delete pair.first; }

template<typename T>
void pair_deleter2 ( std::pair<std::string,T *> pair ) { delete pair.second; }

template<typename T>
T * creator ( T *p ) { return NEW T( *p ); }

template<typename T>
T * cloner ( T *p ) { return p->clone(); }

#endif

