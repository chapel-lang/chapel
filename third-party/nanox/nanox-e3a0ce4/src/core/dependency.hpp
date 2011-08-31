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

#ifndef _NANOS_DEPENDENCY
#define _NANOS_DEPENDENCY

#include "dependency_decl.hpp"

using namespace nanos;

inline Dependency::Dependency ( void ** addr, ptrdiff_t offset, bool input, bool output,
             bool canRename, bool commutative, size_t storageSize )
{
   address = addr;
   offset = offset;
   flags.input = input;
   flags.output = output;
   flags.can_rename = canRename;
   flags.commutative = commutative;
   size = storageSize;
}

inline Dependency::Dependency ( const Dependency &dep )
{
   address = dep.address;
   offset = dep.offset;
   flags.input = dep.flags.input;
   flags.output = dep.flags.output;
   flags.can_rename = dep.flags.can_rename;
   flags.commutative = dep.flags.commutative;
   size = dep.size;
}

inline const Dependency & Dependency::operator= ( const Dependency &dep )
{
   if ( this == &dep ) return *this; 
   address = dep.address;
   offset = dep.offset;
   flags.input = dep.flags.input;
   flags.output = dep.flags.output;
   flags.can_rename = dep.flags.can_rename;
   flags.commutative = dep.flags.commutative;
   size = dep.size;
   return *this;
}

inline void ** Dependency::getAddress() const
{
   return address;
}

inline ptrdiff_t Dependency::getOffset() const
{
   return offset;
}

inline void * Dependency::getDepAddress() const
{
   return (void *) ( (char *) (*address) + offset );
}

inline bool Dependency::isInput() const
{
   return flags.input;
}

inline void Dependency::setInput( bool b )
{
 flags.input = b;
}

inline bool Dependency::isOutput() const
{
   return flags.output;
}

inline void Dependency::setOutput( bool b )
{
   flags.output = b;
}

inline bool Dependency::canRename() const
{
   return flags.can_rename;
}

inline void Dependency::setCanRename( bool b )
{
   flags.can_rename = b;
}

inline bool Dependency::isCommutative() const
{
   return flags.commutative;
}

inline void Dependency::setCommutative( bool b )
{
   flags.commutative = b;
}

#endif
