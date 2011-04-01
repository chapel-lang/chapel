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

#ifndef _NANOS_CACHE_DECL
#define _NANOS_CACHE_DECL

#include "config_decl.hpp"
#include "compatibility.hpp"
#include "directory_decl.hpp"
#include "atomic_decl.hpp"
#include "processingelement_fwd.hpp"
#include "copydescriptor_decl.hpp"

namespace nanos {

  /*! \brief Represents a cache entry identified by an address
   */
   class CacheEntry : public Entry
   {
      private:
         void *_addr; /**< Address identifier of the cache entry  */

         size_t _size; /**< Size of the block in the cache */
         size_t _allocSize; /**< Size of the block allocated in the device */

         volatile bool _dirty; /**< Dirty flag */
         Atomic<bool> _copying; /**< Copying status of the entry */
         Atomic<bool> _flushing; /**< Flushing status of the entry */
         Directory* _flushTo; /**< If the entry is being flushed, points to the directoryEntry that will be updated. */
         Atomic<unsigned int> _transfers; /**< Counts the number of in-flight transfers for dev-to-dev copies. */
         Atomic<bool> _resizing; /**< Tells whether the entry is being resized. */

      public:

        /*! \brief Default constructor
         */
         CacheEntry(): Entry(), _addr( NULL ), _size(0), _allocSize(0), _dirty( false ), _copying(false), _flushing(false), _flushTo(NULL), _transfers(0), _resizing(false) {}

        /*! \brief Constructor
         *  \param addr: address of the cache entry
         */
         CacheEntry( void *addr, size_t size, uint64_t tag, unsigned int version, bool dirty, bool copying ): Entry( tag, version ), _addr( addr ), _size(size), _allocSize(0), _dirty( dirty ), _copying(copying), _flushing(false), _flushTo(NULL), _transfers(0), _resizing(false) {}

        /*! \brief Copy constructor
         *  \param Another CacheEntry
         */
         CacheEntry( const CacheEntry &ce ): Entry( ce.getTag(), ce.getVersion() ), _addr( ce._addr ), _size( ce._size ), _allocSize( ce._allocSize ), _dirty( ce._dirty ), _copying(ce._copying), _flushing(false), _flushTo(NULL), _transfers(0), _resizing(false) {}

        /* \brief Destructor
         */
         ~CacheEntry() {}

        /* \brief Assign operator
         */
         CacheEntry& operator=( const CacheEntry &ce )
         {
            if ( this == &ce ) return *this;
            this->setTag( ce.getTag() );
            this->setVersion( ce.getVersion() );
            this->_addr = ce._addr;
            this->_size = ce._size;
            this->_allocSize = ce._allocSize;
            this->_dirty = ce._dirty;
            this->_copying = ce._copying;
            this->_flushing = ce._flushing;
            this->_flushTo = ce._flushTo;
            this->_transfers = ce._transfers;
            this->_resizing = ce._resizing;
            return *this;
         }

        /* \brief Returns the address identifier of the Cache Entry
         */
         void * getAddress()
         { return _addr; }

        /* \brief Address setter
         */
         void setAddress( void *addr )
         { _addr = addr; }

        /* \brief Returns the size of the block in the cache
         */
         size_t getSize() const
         { return _size; }

        /* \brief Size setter
         */
         void setSize( size_t size )
         { _size = size; }

        /* \brief Returns the size of the block in the device
         */
         size_t getAllocSize() const
         { return _allocSize; }

        /* \brief Size setter
         */
         void setAllocSize( size_t size )
         { _allocSize = size; }

        /*! \brief Returns whether the entry is dirty
         */
         bool isDirty()
         { return _dirty; }

        /*! \brief Set entry to 'dirty'
         *  \param dirty
         */
         void setDirty( bool dirty )
         { _dirty = dirty; }

        /*! \brief Tells whether the entry is copying
         */
         bool isCopying() const
         { return _copying.value(); }

        /*! \brief Set the entry to 'copying'
         *  \param copying
         */
         void setCopying( bool copying )
         { _copying = copying; }

        /*! \brief set copying status to ture if it wansn't atomically.
         */
         bool trySetToCopying()
         {
            Atomic<bool> expected = false;
            Atomic<bool> value = true;
            return _copying.cswap( expected, value );
         }

        /*! \brief Tells whether the entry is being flushed
         */
         bool isFlushing()
         { return _flushing.value(); }

        /*! \brief Set the entry to 'flushing'
         *  \param flushing
         */
         void setFlushing( bool flushing )
         { _flushing = flushing; }

        /*! \brief Returns the directory that will be updated when the entry flush finishes
         */
         Directory* getFlushTo()
         { return _flushTo; }

        /*! \brief Set the entrie's flushing directory
         *  \param dir
         */
         void setFlushTo( Directory *dir )
         { _flushTo = dir; }

        /*! \brief set flushing  status to ture if it wansn't atomically.
         */
         bool trySetToFlushing()
         {
            Atomic<bool> expected = false;
            Atomic<bool> value = true;
            return _flushing.cswap( expected, value );
         }

        /*! \brief Tells whether the entry has in-flight transfers
         */
         bool hasTransfers()
         { return _transfers.value() > 0; }

        /*! \brief increase the number of thransfers
         */
         void increaseTransfers()
         { _transfers++; }

        /*! \brief decrease the number of thransfers
         */
         bool decreaseTransfers()
         {
            _transfers--;
            return hasTransfers();
         }

        /*! \brief Tells whether the entry is resizing
         */
         bool isResizing()
         {
            return _resizing.value();
         }

        /*! \brief Set the entry to 'resizing'
         *  \param resizing
         */
         void setResizing( bool resizing )
         {
            _resizing = resizing;
         }

        /*! \brief set resizing  status to ture if it wansn't atomically.
         */
         bool trySetToResizing()
         {
            Atomic<bool> expected = false;
            Atomic<bool> value = true;
            return _resizing.cswap( expected, value );
         }
   };


  /*! \class Cache
   *  \brief Generic interface of a Cache
   */
   class Cache
   {
         unsigned int _id;
      public:

        /*! \brief Default constructor
         */
         Cache();

        /*! \brief Destructor
         */
         virtual ~Cache() { }

        /*! \brief Allocate a block of memory in the device
         *  \param dir Directory to look for entries if the it needs to free space in the device.
         *  \param size size of the entry to allocate.
         */
         virtual void * allocate( Directory &dir, size_t size ) = 0;

        /*! \brief Resize a CacheEntry
         *  \param dir Directory to look for entries if the it needs to free space in the device.
         *  \param ce Entry to resize.
         *  \param size New size for the reallocated entry.
         */
         virtual void realloc( Directory &dir, CacheEntry * ce, size_t size ) = 0;

        /*! \brief Create a new entry in the cache
         *  \param tag tag identifier of the entry.
         *  \param size size of the entry to be created, and allocated.
         *  \param version initial version of the entry.
         *  \param dirty whether it is dirty from the beginning.
         */
         virtual CacheEntry& newEntry( uint64_t tag, size_t size, unsigned int version, bool dirty ) = 0;

        /*! \brief Insert an entry into the cache
         *  \param tag Tag identifier of the entry.
         *  \param ce CacheEntry to be inserted into the cache.
         *  \param inserted Will be set to true if there was no entry for the given tag and, thus, the entry has beeen inserted.
         */
         virtual CacheEntry& insert( uint64_t tag, CacheEntry& ce, bool& inserted ) = 0;

        /*! \brief Delete an entry from the cache
         *  \param tag Idetifier of the entry to be deleted from the cache
         */
         virtual void deleteEntry( uint64_t tag, size_t size ) = 0;

        /*! \brief Get an entry from the cache
         *  \param tag Identifier key to look for
         */
         virtual CacheEntry* getEntry( uint64_t tag ) = 0;

        /*! \brief Increase the number of references of an entry
         */
         virtual void addReference( uint64_t tag ) = 0;

        /*! \brief Decrease the number of references of an entry
         */
         virtual void deleteReference( uint64_t tag ) = 0;

        /*! \brief Copy data from the host to a device address
         *  \param cd Copy descriptor with the information about the addresses to be copied
         *  \param size Size of the data to be copied
         */
         virtual bool copyDataToCache( CopyDescriptor& cd, size_t size ) = 0;

        /*! \brief Copy data back to the host
         *  \param cd Copy descriptor with the information about the addresses to be copied
         *  \param size Size of the data to be copied
         */
         virtual bool copyBackFromCache( CopyDescriptor& cd, size_t size ) = 0;

        /*! \brief Local copy in the device
         *  \param dst Device address to copy the entry to
         *  \param tag identifier of the cache entry to be copied
         *  \param size Size of the data to be copied
         */
         virtual void copyTo( void *dst, uint64_t tag, size_t size ) = 0;

        /*! \brief Force an entry to be copied back to the Host
         *  \param dir Directory to update when copy finishes
         *  \param tag Identifier key of the entry to invalidate
         *  \param size Size of the entry in the cache
         *  \param de DirectoryEntry to be updated
         */
         virtual void invalidate( Directory &dir, uint64_t tag, size_t size, DirectoryEntry *de ) = 0;

        /*! \brief Force an entry to be copied back to the Host
         *  \param dir Directory to update when copy finishes
         *  \param tag Identifier key of the entry to invalidate
         *  \param de DirectoryEntry to be updated
         */
         virtual void invalidate( Directory &dir, uint64_t tag, DirectoryEntry *de ) = 0;

        /*! \brief Request the Device to priorize an asynchronous transfer
         *  \param tag Identifier to locate the transfer to be priorized
         */
         virtual void syncTransfer( uint64_t tag ) = 0;

        /*! \brief Get the number of references of an entry in the cache
         *  \param tag Identifier of the cache entry
         */
         virtual int getReferences( unsigned int tag ) = 0;

        /*! \brief Returns the cache identifier
         */
         unsigned int getId() const;
   };

  /*! \class CachePolicy
   *  \brief Interface to implement different behaviors of the cache
   */
   class CachePolicy
   {
      private:
        /*! \brief Copy Constructor
         */
         CachePolicy( const CachePolicy &policy );

        /*! \brief Assign Operator
         */
         const CachePolicy & operator= ( const CachePolicy &policy );

      public:
         Cache& _cache; /**< Cache that is using the policy */

        /*! \brief Policy Constructor
         *  \param cache Cache that is using the policy     
         */
         CachePolicy( Cache& cache ) : _cache( cache ) { }

        /*! \brief Destructor
         */
         virtual ~CachePolicy() { }

        /*! \brief Regisers or updates the entry identified by the 'tag' key in the cache with the given properties
         *  \param dir Current directory to look for the entry
         *  \param tag Identifier key of the entry
         *  \param size Size of the entry
         *  \param input Whether the access will read the data (it has to be updated to the latest version known to the directory)
         *  \param output Whether the acces writes the data (the Cache entry must be marked as dirty)
         */
         virtual void registerCacheAccess( Directory &dir, uint64_t tag, size_t size, bool input, bool output );

        /*! \brief Notify the cache that one usage of an entry has finished
         *  \param dir Current directory
         *  \param tag Identifier key of the entry
         *  \param size Size of the entry
         *  \param output If the entry has been written it must be copied back to the host at some point.
         */
         virtual void unregisterCacheAccess( Directory &dir, uint64_t tag, size_t size, bool output ) = 0;

        /*! \brief Register an acces tot the Cache for an address that will only exist for one user
         */
         virtual void registerPrivateAccess( Directory &dir, uint64_t tag, size_t size, bool input, bool output );

        /*! \brief Unregister an acces tot the Cache for an address that will only exist for one user
         */
         virtual void unregisterPrivateAccess( Directory &dir, uint64_t tag, size_t size );
   };

   // FIXME: A plugin maybe?? (see #405)
  /*! \class WriteThroughPolicy
   *  \brief Cache policy that copies back an output entry to the host as soon as its access is released 
   */
   class WriteThroughPolicy : public CachePolicy
   {
      private:

        /*! \brief Copy Constructor
         */
         WriteThroughPolicy( const WriteThroughPolicy &policy );

        /*! \brief Assign Operator
         */
         const WriteThroughPolicy & operator= ( const WriteThroughPolicy &policy );

      public:

        /*! \brief Policy Constructor
         *  \param cache Cache that is using the policy     
         */
         WriteThroughPolicy( Cache& cache ) : CachePolicy( cache ) { }

        /*! \brief Destructor
         */
         virtual ~WriteThroughPolicy() { }

        /*! \brief Notify the cache that one usage of an entry has finished
         *  \param dir Current directory
         *  \param tag Identifier key of the entry
         *  \param size Size of the entry
         *  \param output If the entry has been written it must be copied back to the host at some point.
         */
         virtual void unregisterCacheAccess( Directory &dir, uint64_t tag, size_t size, bool output );
   };

  /*! \class WriteBackPolicy
   *  \brief Cache policy that copies back an output entry to the host as late as possible
   */
   class WriteBackPolicy : public CachePolicy
   {
      private:

        /*! \brief Copy Constructor
         */
         WriteBackPolicy( const WriteBackPolicy &policy );

        /*! \brief Assign Operator
         */
         const WriteBackPolicy & operator= ( const WriteBackPolicy &policy );

      public:

        /*! \brief Policy Constructor
         *  \param cache Cache that is using the policy     
         */
         WriteBackPolicy( Cache& cache ) : CachePolicy( cache ) { }

        /*! \brief Destructor
         */
         virtual ~WriteBackPolicy() { }

        /*! \brief Notify the cache that one usage of an entry has finished
         *  \param dir Current directory
         *  \param tag Identifier key of the entry
         *  \param size Size of the entry
         *  \param output If the entry has been written it must be copied back to the host at some point.
         */
         virtual void unregisterCacheAccess( Directory &dir, uint64_t tag, size_t size, bool output );
   };

  /*! \brief A Cache is a class that provides basic services for registering and
   *         searching for memory blocks in a device using an identifier represented
   *         by an unsigned int of 64 bits which represents the address of the original
   *         data in the host. 
   */
   template <class _T, class _Policy = WriteThroughPolicy>
   //template <class _T, class _Policy = WriteBackPolicy>
   class DeviceCache : public Cache
   {
     /* FIXME (see #195)
      *   - Code in the cache c file?
      *   - check for errors 
      */
      private:
         ProcessingElement *_pe; /**< PE used to manage asynchronous copies */

         typedef HashMap<uint64_t, CacheEntry> CacheHash; /**< Maps keys with CacheEntries  */
         CacheHash _cache; /**< HashMap where the cache entries are stored */

         _Policy _policy; /**< Cache Policy used by this cache */

         size_t _size; /**< Size of the cache in bytes */
         size_t _usedSize; /**< Cache space usage counter */

        /*! \brief Copy Constructor
         */
         DeviceCache( const DeviceCache &cache );

        /*! \brief Assign operator
         */
         const DeviceCache & operator= ( const DeviceCache &cache );

        /*! \brief Internal data structure for copy synchronization
         */
         struct SyncData {
            DeviceCache* _this;
         };

        /*! \brief Internal synchronization function
         *  \param sd pointer to this cache
         *  \param cd Descriptor of the copy to synchronize
         */
         static void synchronizeInternal( SyncData &sd, CopyDescriptor &cd );
 
      public:
        /* \brief Default constructor
         */
         DeviceCache( size_t size, ProcessingElement *pe = NULL ) : _pe( pe ), _cache(), _policy( *this ), _size( size ), _usedSize(0) {}

        /*! \brief Destructor
         */
         virtual ~DeviceCache() {}

        /*! \brief Returns the size of the cache
         */
         size_t getSize();

        /*! \brief Set the size of the cache
         *  \param size new size for the cache
         */
         void setSize( size_t size );

        /*! \brief Allocate a block of memory in the device
         *  \param dir Directory to look for entries if the it needs to free space in the device.
         *  \param size size of the entry to allocate.
         */
         void * allocate( Directory &dir, size_t size );

        /*! \brief delete unused entries to make space in the cache (flush them if necessary)
         *  \param dir Directory to be able to synchronize flushed entries
         *  \param size
         */
         void freeSpaceToFit( Directory& dir, size_t size );

        /*! \brief Delete an entry from the cache
         *  \param tag Idetifier of the entry to be deleted from the cache
         */
         void deleteEntry( uint64_t tag, size_t size );

        /*! \brief Resize a CacheEntry
         *  \param dir Directory to look for entries if the it needs to free space in the device.
         *  \param ce Entry to resize.
         *  \param size New size for the reallocated entry.
         */
         void realloc( Directory &dir, CacheEntry *ce, size_t size );

        /* \brief get the Address in the cache for tag
         * \param tag: Identifier of the entry to look for
         */
         void * getAddress( uint64_t tag );

        /* \brief Copy data from the address represented by the tag to the entry in the device.
         * \param cd: identifier of the entry
         * \param size: number of bytes to copy
         */
         bool copyDataToCache( CopyDescriptor &cd, size_t size );

        /* \brief Copy back from the entry to the address represented by the tag.
         * \param cd: Entry identifier and address of original data
         * \param size: number of bytes to copy
         */
         bool copyBackFromCache( CopyDescriptor &cd, size_t size );

        /* \brief Perform local copy in the device for an entry
         * \param dst: Device destination address to copy to
         * \param tag: entry identifier to look for the source data
         * \param size: number of bytes to copy
         */
         void copyTo( void *dst, uint64_t tag, size_t size );

        /*! \brief Create a new entry in the cache
         *  \param tag tag identifier of the entry.
         *  \param size size of the entry to be created, and allocated.
         *  \param version initial version of the entry.
         *  \param dirty whether it is dirty from the beginning.
         */
         CacheEntry& newEntry( uint64_t tag, size_t size, unsigned int version, bool dirty );

        /*! \brief Insert an entry into the cache
         *  \param tag Tag identifier of the entry.
         *  \param ce CacheEntry to be inserted into the cache.
         *  \param inserted Will be set to true if there was no entry for the given tag and, thus, the entry has beeen inserted.
         */
         CacheEntry& insert( uint64_t tag, CacheEntry& ce, bool& inserted );

        /*! \brief Get an entry from the cache
         *  \param tag Identifier key to look for
         */
         CacheEntry* getEntry( uint64_t tag );

        /*! \brief Increase the number of references of an entry
         */
         void addReference( uint64_t tag );

        /*! \brief Decrease the number of references of an entry
         */
         void deleteReference( uint64_t tag );

        /*! \brief Regisers or updates the entry identified by the 'tag' key in the cache with the given properties
         *  \param dir Current directory to look for the entry
         *  \param tag Identifier key of the entry
         *  \param size Size of the entry
         *  \param input Whether the access will read the data (it has to be updated to the latest version known to the directory)
         *  \param output Whether the acces writes the data (the Cache entry must be marked as dirty)
         */
         void registerCacheAccess( Directory &dir, uint64_t tag, size_t size, bool input, bool output );

        /*! \brief Notify the cache that one usage of an entry has finished
         *  \param dir Current directory
         *  \param tag Identifier key of the entry
         *  \param size Size of the entry
         *  \param output If the entry has been written it must be copied back to the host at some point.
         */
         void unregisterCacheAccess( Directory &dir, uint64_t tag, size_t size, bool output );

        /*! \brief Register an acces tot the Cache for an address that will only exist for one user
         */
         void registerPrivateAccess( Directory &dir, uint64_t tag, size_t size, bool input, bool output );

        /*! \brief Unregister an acces tot the Cache for an address that will only exist for one user
         */
         void unregisterPrivateAccess( Directory &dir, uint64_t tag, size_t size );

        /*! \brief Use to synchronize a transfer when the copy is finished
         *  \param tag Identifier key of the entry to be synchronized
         */
         void synchronizeTransfer( uint64_t tag );

        /*! \brief Synchronize a copy using the copy descriptor
         *  \param cd
         */
         void synchronize( CopyDescriptor &cd );

        /*! \brief Synchronize a list of copies
         *  \param cds list of copy descriptors
         */
         void synchronize( std::list<CopyDescriptor> &cds );

        /*! \brief Busy wait until the entry is ready to be used
         *  \param tag Identifier key of the entry to wait to be copied
         */
         void waitInput( uint64_t tag );

        /*! \brief Internal wait interface used by for_each
         */
         static void waitInput( DeviceCache* _this, uint64_t tag );

        /*! \brief Busy wait until the entries are ready to be used
         *  \param tags List of identifier keys of the entries to wait to be copied
         */
         void waitInputs( std::list<uint64_t> &tags );

        /*! \brief Force an entry to be copied back to the Host
         *  \param dir Directory to update when copy finishes
         *  \param tag Identifier key of the entry to invalidate
         *  \param size Size of the entry in the cache
         *  \param de DirectoryEntry to be updated
         */
         void invalidate( Directory &dir, uint64_t tag, size_t size, DirectoryEntry *de );

        /*! \brief Force an entry to be copied back to the Host
         *  \param dir Directory to update when copy finishes
         *  \param tag Identifier key of the entry to invalidate
         *  \param de DirectoryEntry to be updated
         */
         void invalidate( Directory &dir, uint64_t tag, DirectoryEntry *de );

        /*! \brief get a reference to the size variable to allow using it as a ConfigOption
         */
         size_t& getCacheSize();

        /*! \brief Request the Device to priorize an asynchronous transfer
         *  \param tag Identifier to locate the transfer to be priorized
         */
         void syncTransfer( uint64_t tag );

        /*! \brief Get the number of references of an entry in the cache
         *  \param tag Identifier of the cache entry
         */
         int getReferences( unsigned int tag );
   };

}

#endif
