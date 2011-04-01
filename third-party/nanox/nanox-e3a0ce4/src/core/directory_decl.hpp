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

#ifndef _NANOS_DIRECTORY_DECL_H
#define _NANOS_DIRECTORY_DECL_H

#include "compatibility.hpp"
#include "cache_fwd.hpp"
#include "hashmap_decl.hpp"
#include "atomic_decl.hpp"
#include "cache_map_decl.hpp"

namespace nanos
{

  /*! \class Entry
   *  \brief Generic Entry Representing a block of memory accessed by a WD
   */
   class Entry
   {
      private:

         uint64_t _tag; /**< Identifier key of the entry (it's address) */
         Atomic<unsigned int> _version; /**< Version of the entry */
      public:

         /*! \brief Entry default constructor
          */
         Entry() : _tag(0), _version(0) { }

         /*! \brief Entry copy constructor
          */
         Entry ( const Entry &ent ) : _tag( ent._tag ), _version ( ent._version ) { }

         /*! \brief Entry copy assignment operator (private)
          */
         const Entry& operator= ( const Entry &ent );

         /*! \brief Entry constructor
          */
         Entry ( uint64_t tag, unsigned int version ) : _tag( tag ), _version( version ) { }

         /*! \brief Entry destructor
          */
         ~Entry () {}

        /*! \brief Returns the entrie's tag
         */
         uint64_t getTag() const;

        /*! \brief Set the entrie's tag to 'tag'
         */
         void setTag( uint64_t tag);

        /*! \brief Returns the version
         */
         unsigned int getVersion() const;

        /*! \brief Set the version to 'version'
         */
         void setVersion( unsigned int version );

        /*! \brief Increase the version number by one
         */
         void increaseVersion();

        /*! \brief Try to set version number to version atomically and return true if succeeded
         */
         bool setVersionCS( unsigned int version );
   };

  /*! \class DirectoryEntry
   *  \brief Entry version for the directory
   */
   class DirectoryEntry : public Entry
   {
      private:
         Atomic<Cache *> _owner;    /**< Cache that owns the entry when it is dirty */

         Lock _entryLock;           /**< Lock for atomic operations in the directory (entry insertion) */
         Atomic<bool> _invalidated; /**< Invalidated flag*/

         CacheAccessMap _cacheAccesses;
      private:

      public:
         /*! \brief DirectoryEntry default constructor
          */
         DirectoryEntry();

         /*! \brief DirectoryEntry copy constructor
          */
         DirectoryEntry ( const DirectoryEntry &de) : Entry( de ), _owner( de._owner ), _entryLock(), _invalidated( false ), _cacheAccesses( de._cacheAccesses ) { }

         /*! \brief DirectoryEntry copy assignment operator
          */
         const DirectoryEntry& operator= ( const DirectoryEntry &ent );

         /*! \brief DirectoryEntry constructor 
          */
         DirectoryEntry( uint64_t tag, unsigned int version, Cache *c, unsigned int cacheMapSize )
            : Entry( tag, version ), _owner( c ), _entryLock(), _invalidated( false ), _cacheAccesses( cacheMapSize ) { }

         /*! \brief DirectoryEntry destructor
          */
         ~DirectoryEntry () {}


        /*! \brief Returns the owner of the entry
         */
         Cache * getOwner() const;

        /*! \brief Set the owner of the entry to 'owner'
         */
         void setOwner( Cache *owner );

        /*! \brief Clear the owner atomically, returns false if it had already been cleared
         */
         bool clearOwnerCS( Cache *current );

        /*! \brief Returns true if the entry has been invalidated
         */
         bool isInvalidated();

        /*! \brief Set the entry to 'invalid'
         */
         void setInvalidated( bool invalid );

        /*! \brief tries to set the entry to invalidated atomically. Returns false if it failed
         */
         bool trySetInvalidated();

        /*! \brief add an acces to this entry for the cache 'cache'
         */
         void addAccess( unsigned int cacheId );

        /*! \brief remove an acces to this entry for the cache 'cache'
         */
         void removeAccess( unsigned int cacheId );

         unsigned int getAccess( unsigned int cacheId );
   };

  /*! \class Directory
   *  \brief Stores copy accesses controls their versions and if they are dirty in any cache
   */
   class Directory
   {
      private:
         typedef HashMap<uint64_t, DirectoryEntry> DirectoryMap; /**< Directorie's HashMap*/
         DirectoryMap _directory; /**< The map will store the entries indexed by they tag */
         Directory* _parent; /**< Parent directory (following the WD's hierarchy */
         unsigned int _cacheMapSize;

      private:

         /*! \brief Directory copy constructor (private) 
          */
         Directory( const Directory &dir );

         /*! \brief Directory copy assignment operator (private) 
          */
         const Directory & operator= ( const Directory &dir );

      public:

         /*! \brief Directory default constructor
          */
         Directory();

         /*! \brief Directory destructor
          */
         ~Directory() { }

        /*! \brief Set the parent Directory to 'parent'
         */
         void setParent( Directory *parent );

        /*! \brief Returns the cache map size
         */
         unsigned int getCacheMapSize();

        /*! \brief Insert an entry into the directory. Returns a reference to the inserted entry
         *  \param tag Identifier key of the entry
         *  \param ent Entry to be inserted
         *  \param inserted Set to true if inserted, if not, the returned one is the one found in the Map
         */
         DirectoryEntry& insert( uint64_t tag, DirectoryEntry &ent, bool &inserted );

        /*! \brief Create and insert a new entry in the directory
         *  \param tag Idetifier key of the entry
         *  \param version Version to initialize the new entry with
         *  \param owner Cache owner of the entry if it is dirty from the beggining
         */
         DirectoryEntry& newEntry( uint64_t tag, unsigned int version, Cache* owner );

        /*! \brief returns the entry identified by 'tag' or creates it empty if it was not found
         */
         DirectoryEntry* getEntry( uint64_t tag, unsigned int cVersion=0 );

        /*! \brief returns the entry identified by 'tag' or NULL if it was not found
         */
         DirectoryEntry* findEntry( uint64_t tag );

        /*! \brief Set the directory version to the current's one incremented by one for the entry with key 'tag'
         *  \param tag Identifier key of the entry to update
         *  \param current 'this' directory is the parent of 'current' which may have a newer version of the entry
         */
         void updateCurrentDirectory( uint64_t tag, Directory &current );

        /*! \brief Register an access to a copy by the host
         *  \param tag Identifier key of the access (address)
         *  \param size Size of the read or written block
         *  \param input Whether the access is a read
         *  \param output Whether the access is a write
         */
         void registerAccess( uint64_t tag, size_t size, bool input, bool output );

        /*! \brief Unregister an access to a copy by the host
         *  \param tag Identifier key of the access (address)
         *  \param output Whether the access was a write
         *  \param current Internal directory of the (finished) task that is unregistering the access (if any)
         */
         void unRegisterAccess( uint64_t tag, bool output, Directory *current );

        /*! \brief Waits for an entry to finish flushing if it was flushing
         *  \param tag Identifier key of the entry to wait for
         *  \param output currently unused
         */
         void waitInput( uint64_t tag, bool output );

        /*! \brief Request for all entries to be invalidated so the host can access them and caches will see the newer version
         */
         void synchronizeHost();

        /*! \brief Request for entries in the list to be invalidated so the host can access them and caches will see the newer version
         *  \param syncTags List of identifier keys of the entries to synchronize
         */
         void synchronizeHost( std::list<uint64_t> syncTags );
   };

};

#endif
