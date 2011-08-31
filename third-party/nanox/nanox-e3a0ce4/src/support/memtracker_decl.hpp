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
#ifndef _NANOS_MEMTRACKER_DECL_HPP
#define _NANOS_MEMTRACKER_DECL_HPP

#ifdef NANOS_DEBUG_ENABLED

#include "memtracker_fwd.hpp"
#include "debug.hpp"
#include "atomic.hpp"
#include "allocator.hpp"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <map>
#include <limits>
#include <exception>

namespace nanos {

class MemTracker
{
      struct BlockInfo {
	size_t       _size;
	const char * _file;
	int          _line;
        int          _thread;

	BlockInfo ( ) { }
	BlockInfo ( const size_t size, const char *file, const int line, const int thread ) : _size(size), _file(file), _line(line), _thread(thread) {}
      };

      struct DistrInfo {
	size_t _current;
	size_t _max;
	size_t _total;
      };

      template<class K, class T>
      struct InternalMap {
         typedef std::map <K, T, std::less<K>, InternalAllocator<std::pair<const K,T> > > type;
      };

      typedef InternalMap<void *,BlockInfo>::type AddrMap;
      typedef InternalMap<size_t,DistrInfo>::type SizeMap;

      AddrMap     _blocks;
      SizeMap     _stats;
      
      size_t      _totalMem;
      size_t      _numBlocks;
      size_t      _maxMem;

      Lock        _lock;
      bool        _showStats;

  public:

      MemTracker();

      ~MemTracker() { showStats(); }

      void *allocate ( size_t size, const char *file = 0, int line = 0 );
      void deallocate ( void * p, const char *file = 0, int line = 0 );
      void showStats ();
};

extern MemTracker *mem;

inline MemTracker & getMemTracker ()
{
   if (!mem) {
      mem = (MemTracker *) malloc(sizeof(MemTracker));
      new (mem) MemTracker();
   }
   return *mem;
}

class NanosMemTrackerHelper {
  public:
    NanosMemTrackerHelper() { getMemTracker(); }
    ~NanosMemTrackerHelper() { getMemTracker().showStats(); }
};


} // namespace nanos
#endif
#endif
