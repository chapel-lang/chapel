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
#include "new_decl.hpp"

#ifdef NANOS_DEBUG_ENABLED

#include "memtracker.hpp"

void* operator new ( size_t size, const char *file, int line ) { return nanos::getMemTracker().allocate( size, file, line ); }
void* operator new[] ( size_t size, const char *file, int line ) { return nanos::getMemTracker().allocate( size, file, line ); }

void* operator new ( size_t size ) { return nanos::getMemTracker().allocate( size ); }
void* operator new[] ( size_t size ) { return nanos::getMemTracker().allocate( size ); }
void operator delete ( void *p ) { nanos::getMemTracker().deallocate( p ); }
void operator delete[] ( void *p ) { nanos::getMemTracker().deallocate( p ); }

#else

#include "allocator.hpp"

void* operator new ( size_t size ) { return nanos::getAllocator().allocate( size ); }
void* operator new[] ( size_t size ) { return nanos::getAllocator().allocate( size ); }
void operator delete ( void *p ) { nanos::getAllocator().deallocate( p ); }
void operator delete[] ( void *p ) { nanos::getAllocator().deallocate( p ); }

#endif

