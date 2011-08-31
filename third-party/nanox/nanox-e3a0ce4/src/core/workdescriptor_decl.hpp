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

#ifndef _NANOS_WORK_DESCRIPTOR_DECL_H
#define _NANOS_WORK_DESCRIPTOR_DECL_H

#include <stdlib.h>
#include <utility>
#include <vector>
#include "workgroup_decl.hpp"
#include "dependableobjectwd_decl.hpp"
#include "copydata_decl.hpp"
#include "synchronizedcondition_decl.hpp"
#include "atomic_decl.hpp"
#include "lazy_decl.hpp"
#include "instrumentationcontext_decl.hpp"
#include "compatibility.hpp"

#include "slicer_fwd.hpp"
#include "basethread_fwd.hpp"
#include "processingelement_fwd.hpp"
#include "wddeque_fwd.hpp"
#include "directory_decl.hpp"

namespace nanos
{

   /*! \brief This class represents a device object
    */
   class Device
   {
      private:

         const char *_name; /**< Identifies device type */

      public:

         /*! \brief Device constructor
          */
         Device ( const char *n ) : _name ( n ) {}

         /*! \brief Device copy constructor
          */
         Device ( const Device &arch ) : _name ( arch._name ) {}

         /*! \brief Device destructor
          */
         ~Device() {};

         /*! \brief Device assignment operator
          */
         const Device & operator= ( const Device &arch ) { _name = arch._name; return *this; }

         /*! \brief Device equals operator
          */
         bool operator== ( const Device &arch ) { return arch._name == _name; }

         /*! \brief Get device name
          */
         const char * getName ( void ) const { return _name; }

   };

  /*! \brief This class holds the specific data for a given device
   *
   */
   class DeviceData
   {
      private:
         /**Use pointers for this as is this fastest way to compare architecture compatibility */
         const Device *_architecture; /**< Related Device (architecture). */

      public:

         /*! \brief DeviceData constructor
          */
         DeviceData ( const Device *arch ) : _architecture ( arch ) {}

         /*! \brief DeviceData copy constructor
          */
         DeviceData ( const DeviceData &dd ) : _architecture ( dd._architecture )  {}

         /*! \brief DeviceData destructor
          */
         virtual ~DeviceData() {}

         /*! \brief DeviceData assignment operator
          */
         const DeviceData & operator= ( const DeviceData &dd )
         {
            // self-assignment: ok
            _architecture = dd._architecture;
            return *this;
         }

         /*! \brief Indicates if DeviceData is compatible with a given Device
          *
          *  \param[in] arch is the Device which we have to compare to.
          *  \return a boolean indicating if both elements (DeviceData and Device) are compatible.
          */
         bool isCompatible ( const Device &arch );

         /*! \brief FIXME: (#170) documentation needed
          */
         virtual void lazyInit (WorkDescriptor &wd, bool isUserLevelThread, WorkDescriptor *previous=NULL ) = 0;

         /*! \brief FIXME: (#170) documentation needed
          */
         virtual size_t size ( void ) = 0;

         /*! \brief FIXME: (#170) documentation needed 
          */
         virtual DeviceData *copyTo ( void *addr ) = 0;

    };

   /*! \brief This class identifies a single unit of work
    */
   class WorkDescriptor : public WorkGroup
   {
      public:
	 typedef enum { IsNotAUserLevelThread=false, IsAUserLevelThread=true } ULTFlag;

      private:

         typedef enum { INIT, READY, IDLE, BLOCKED } State;

         size_t                        _data_size;    /**< WD data size */
         int                           _data_align;   /**< WD data alignment */
         void                         *_data;         /**< WD data */
         void                         *_wdData;       /**< Internal WD data. this allows higher layer to associate data to the WD */
         bool                          _tie;          /**< FIXME: (#170) documentation needed */
         BaseThread                   *_tiedTo;       /**< FIXME: (#170) documentation needed */

         State                         _state;        /**< Workdescriptor current state */

         GenericSyncCond              *_syncCond;     /**< FIXME: (#170) documentation needed */

         WorkDescriptor               *_parent;       /**< Parent WD (task hierarchy). Cilk sched.: first steal parent, next other tasks */

         WDDeque                      *_myQueue;      /**< Reference to a queue. Allows dequeuing from third party (e.g. Cilk schedulers */

         unsigned                      _depth;        /**< Level (depth) of the task */

         unsigned                      _numDevices;   /**< Number of suported devices for this workdescriptor */
         DeviceData                  **_devices;      /**< Supported devices for this workdescriptor */
         DeviceData                   *_activeDevice; /**< Active device (if any) */

         size_t                        _numCopies;    /**< Copy-in / Copy-out data */
         CopyData                     *_copies;       /**< Copy-in / Copy-out data */

         TR1::shared_ptr<DOSubmit>     _doSubmit;     /**< DependableObject representing this WD in its parent's depsendencies domain */
         LazyInit<DOWait>              _doWait;       /**< DependableObject used by this task to wait on dependencies */

         LazyInit<DependenciesDomain>  _depsDomain;   /**< Dependences domain. Each WD has one where DependableObjects can be submitted */
         LazyInit<Directory>           _directory;    /**< Directory to mantain cache coherence */

         InstrumentationContextData    _instrumentationContextData; /**< Instrumentation Context Data (empty if no instr. enabled) */

         bool                          _submitted;  /**< Has this WD been submitted to the Scheduler? */

         nanos_translate_args_t        _translateArgs; /**< Translates the addresses in _data to the ones obtained by get_address(). */

      private: /* private methods */
         /*! \brief WorkDescriptor copy assignment operator (private)
          */
         const WorkDescriptor & operator= ( const WorkDescriptor &wd );
         /*! \brief WorkDescriptor default constructor (private) 
          */
         WorkDescriptor ();
      public: /* public methods */

         /*! \brief WorkDescriptor constructor - 1
          */
         WorkDescriptor ( int ndevices, DeviceData **devs, size_t data_size = 0, int data_align = 1, void *wdata=0,
                          size_t numCopies = 0, CopyData *copies = NULL, nanos_translate_args_t translate_args = NULL )
                        : WorkGroup(), _data_size ( data_size ), _data_align( data_align ),  _data ( wdata ),
                          _wdData ( NULL ), _tie ( false ), _tiedTo ( NULL ),
                          _state( INIT ), _syncCond( NULL ),  _parent ( NULL ), _myQueue ( NULL ), _depth ( 0 ),
                          _numDevices ( ndevices ), _devices ( devs ), _activeDevice ( ndevices == 1 ? devs[0] : NULL ),
                          _numCopies( numCopies ), _copies( copies ), _doSubmit(), _doWait(),
                          _depsDomain(), _directory(), _instrumentationContextData(),_submitted(false), _translateArgs( translate_args ) { }

         /*! \brief WorkDescriptor constructor - 2
          */
         WorkDescriptor ( DeviceData *device, size_t data_size = 0, int data_align = 1, void *wdata=0,
                          size_t numCopies = 0, CopyData *copies = NULL, nanos_translate_args_t translate_args = NULL )
                        : WorkGroup(), _data_size ( data_size ), _data_align ( data_align ), _data ( wdata ),
                          _wdData ( NULL ), _tie ( false ), _tiedTo ( NULL ),
                          _state( INIT ), _syncCond( NULL ), _parent ( NULL ), _myQueue ( NULL ), _depth ( 0 ),
                          _numDevices ( 1 ), _devices ( &_activeDevice ), _activeDevice ( device ),
                          _numCopies( numCopies ), _copies( copies ), _doSubmit(), _doWait(),
                          _depsDomain(), _directory(), _instrumentationContextData(),_submitted(false), _translateArgs( translate_args ) { }

         /*! \brief WorkDescriptor copy constructor (using a given WorkDescriptor)
          *
          *  This function is used as a constructor, receiving as a parameter other WorkDescriptor.
          *  The constructor uses a DeviceData vector and a new void * data which will be completely
          *  different from the former WorkDescriptor. Rest of the data is copied from the former WD.
          *
          *  This constructor is used only for duplicating purposes
          *
          *  \see WorkDescriptor System::duplicateWD System::duplicateSlicedWD
          */
         WorkDescriptor ( const WorkDescriptor &wd, DeviceData **devs, CopyData * copies, void *data = NULL )
                        : WorkGroup( wd ), _data_size( wd._data_size ), _data_align( wd._data_align ), _data ( data ),
                          _wdData ( NULL ), _tie ( wd._tie ), _tiedTo ( wd._tiedTo ),
                          _state ( INIT ), _syncCond( NULL ), _parent ( wd._parent ), _myQueue ( NULL ), _depth ( wd._depth ),
                          _numDevices ( wd._numDevices ), _devices ( devs ), _activeDevice ( wd._numDevices == 1 ? devs[0] : NULL ),
                          _numCopies( wd._numCopies ), _copies( wd._numCopies == 0 ? NULL : copies ),
                          _doSubmit(), _doWait(), _depsDomain(), _directory(), _instrumentationContextData(),_submitted(false), _translateArgs( wd._translateArgs ) { }

         /*! \brief WorkDescriptor destructor
          *
          * All data will be allocated in a single chunk so only the destructors need to be invoked
          * but not the allocator
          */
         virtual ~WorkDescriptor()
         {
            for ( unsigned i = 0; i < _numDevices; i++ )
               _devices[i]->~DeviceData();
         }

         /*! \brief Has this WorkDescriptor ever run?
          */
         bool started ( void ) const;

         /*! \brief Prepare WorkDescriptor to run
          *
          *  This function is useful to perform lazy initialization in the workdescriptor
          */
         void init ();

         /*! \brief Last operations just before WD execution
          *
          *  This function is useful to perform any operation that needs to be done at the last moment
          *  before the execution of the WD.
          */
         void start ( ULTFlag isUserLevelThread, WorkDescriptor *previous = NULL );

         /*! \brief Get data size
          *
          *  This function returns the size of the user's data related with current WD
          *
          *  \return data size
          *  \see getData setData setDatasize
          */
         size_t getDataSize () const;

         /*! \brief Set data size
          *
          *  This function set the size of the user's data related with current WD
          *
          *  \see getData setData getDataSize
          */
         void setDataSize ( size_t data_size );

         /*! \brief Get data alignment
          *
          *  This function returns the data alignment of the user's data related with current WD
          *
          *  \return data alignment
          *  \see getData setData setDatasize
          */
         int getDataAlignment () const;

         /*! \brief Set data alignment
          *
          *  This function set the data alignment of the user's data related with current WD
          *
          *  \see getData setData setDataSize
          */
         void setDataAlignment ( int data_align) ;

         WorkDescriptor * getParent();

         void setParent ( WorkDescriptor * p );

         WDDeque * getMyQueue();

         void setMyQueue ( WDDeque * myQ );

         bool isEnqueued();

         /*! \brief FIXME: (#170) documentation needed
          *
          *  Named arguments idiom format.
          */
         WorkDescriptor & tied ();

         WorkDescriptor & tieTo ( BaseThread &pe );

         bool isTied() const;

         BaseThread * isTiedTo() const;

         void setData ( void *wdata );

         void * getData () const;

         bool isIdle () const;

         void setIdle ();

         bool isBlocked () const;

         void setBlocked ();

         bool isReady () const;

         void setReady ();

         GenericSyncCond * getSyncCond();

         void setSyncCond( GenericSyncCond * syncCond );

         void setDepth ( int l );

         unsigned getDepth();

         /* device related methods */
         DeviceData * findDeviceData ( const Device &device ) const;
         bool canRunIn ( const Device &device ) const;
         bool canRunIn ( const ProcessingElement &pe ) const;
         DeviceData & activateDevice ( const Device &device );
         DeviceData & getActiveDevice () const;

         bool hasActiveDevice() const;

         void setInternalData ( void *data );

         void * getInternalData () const;

         void setTranslateArgs( nanos_translate_args_t translateArgs );

         /*! \brief Get the number of devices
          *
          *  This function return the number of devices for the current WD
          *
          *  \return WorkDescriptor's number of devices
          *  \see getDevices
          */
         unsigned getNumDevices ( void );

         /*! \brief Get devices
          *
          *  This function return a device vector which are related with the current WD
          *
          *  \return devices vector
          *  \see getNumDevices
          */
         DeviceData ** getDevices ( void );

         /*! \brief WD dequeue 
          *
          *  This function give us the next WD slice to execute. As a default
          *  behaviour give the whole WD and returns true, meaning that there
          *  are no more slices to compute
          *
          *  \param [out] slice is the next slice to manage
          *
          *  \return true if there are no more slices to manage, false otherwise
          */
         virtual bool dequeue ( WorkDescriptor **slice );

         // headers
         virtual void submit ( void );

         virtual void done ();

         void clear ();

         /*! \brief returns the number of CopyData elements in the WorkDescriptor
          */
         size_t getNumCopies() const;

         /*! \brief returns the CopyData vector that describes the copy-ins/copy-outs of the WD
          */
         CopyData * getCopies() const;

         /*! \brief Returns a pointer to the DOSubmit of the WD
          */
         TR1::shared_ptr<DOSubmit> & getDOSubmit();

         /*! \brief Add a new WD to the domain of this WD.
          *  \param wd Must be a WD created by "this". wd will be submitted to the
          *  scheduler when its dependencies are satisfied.
          *  \param numDeps Number of dependencies.
          *  \param deps Array with dependencies associated to the submitted wd.
          */
         void submitWithDependencies( WorkDescriptor &wd, size_t numDeps, Dependency* deps );

         /*! \brief Waits untill all (input) dependencies passed are satisfied for the _doWait object.
          *  \param numDeps Number of de dependencies.
          *  \param deps dependencies to wait on, should be input dependencies.
          */
         void waitOn( size_t numDeps, Dependency* deps );

         /*! If this WorkDescriptor has an immediate succesor (i.e., anothur WD that only depends on him)
             remove it from the dependence graph and return it. */
         WorkDescriptor * getImmediateSuccessor ( BaseThread &thread );

         /*! \brief Make this WD's domain know a WD has finished.
          *  \paran wd Must be a wd created in this WD's context.
          */
         void workFinished(WorkDescriptor &wd);

         /*! \brief Returns the DependenciesDomain object.
          */
         DependenciesDomain & getDependenciesDomain();

         /*! \brief Returns embeded instrumentation context data.
          */
         InstrumentationContextData *getInstrumentationContextData( void );

         /*! \breif Prepare private copies to have relative addresses
          */
         void prepareCopies();

         /*! \brief Get the WorkDescriptor's directory.
          *  if create is true and directory is not initialized returns NULL,
          *  otherwise it is created (if necessary) and a pointer to it is returned.
          */
         Directory* getDirectory(bool create=false);

         virtual void waitCompletion();
         virtual void waitCompletionAndSignalers();

         bool isSubmitted( void ) const;
         void submitted( void );
   };

   typedef class WorkDescriptor WD;

   typedef class DeviceData DD;

};

#endif

