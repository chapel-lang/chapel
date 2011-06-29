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

#ifndef _NANOS_COPYDATA
#define _NANOS_COPYDATA

#include "copydata_decl.hpp"

using namespace nanos;

inline CopyData::CopyData ( uint64_t addr, nanos_sharing_t nxSharing, bool input, bool output, size_t storageSize )
{
   address = addr;
   sharing = nxSharing;
   flags.input = input;
   flags.output = output;
   size = storageSize;
}

inline CopyData::CopyData ( const CopyData &cd )
{
   address = cd.address;
   sharing = cd.sharing;
   flags.input = cd.flags.input;
   flags.output = cd.flags.output;
   size = cd.size;
}

inline const CopyData & CopyData::operator= ( const CopyData &cd )
{
   if ( this == &cd ) return *this; 
   address = cd.address;
   flags.input = cd.flags.input;
   flags.output = cd.flags.output;
   size = cd.size;
   return *this;
}

inline uint64_t CopyData::getAddress() const
{
   return address;
}

inline void CopyData::setAddress( uint64_t addr )
{
   address = addr;
}

inline bool CopyData::isInput() const
{
   return flags.input;
}

inline void CopyData::setInput( bool b )
{
   flags.input = b;
}

inline bool CopyData::isOutput() const
{
   return flags.output;
}

inline void CopyData::setOutput( bool b )
{
   flags.output = b;
}

inline size_t CopyData::getSize() const
{
   return size;
}

inline bool CopyData::isShared() const
{
   return sharing ==  NANOS_SHARED;
}

inline bool CopyData::isPrivate() const
{
   return sharing ==  NANOS_PRIVATE;
}

inline nanos_sharing_t CopyData::getSharing() const
{
   return sharing;
}

#endif
