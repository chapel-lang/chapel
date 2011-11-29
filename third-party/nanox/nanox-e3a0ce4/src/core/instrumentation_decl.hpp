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
#ifdef NANOS_INSTRUMENTATION_ENABLED
#define NANOS_INSTRUMENT(f) f;
#else
#define NANOS_INSTRUMENT(f)
#endif

#ifndef __NANOS_INSTRUMENTOR_DECL_H
#define __NANOS_INSTRUMENTOR_DECL_H
#include <list>
#include <utility>
#include <string>
#include "compatibility.hpp"
#include "debug.hpp"
#include "nanos-int.h"
#include "atomic_decl.hpp"
#include "instrumentationcontext_fwd.hpp"
#include "workdescriptor_fwd.hpp"
#include "allocator_decl.hpp"

namespace nanos {

#ifdef NANOS_INSTRUMENTATION_ENABLED
   class InstrumentationValueDescriptor
   {
      private:
         nanos_event_value_t  _id;          /**< InstrumentationValueDescriptor id */
         std::string          _description; /**< InstrumenotrValueDescriptor description */
      private:
         /*! \brief InstrumentationValueDescriptor default constructor (private)
          */
         InstrumentationValueDescriptor ();
         /*! \brief InstrumentationValueDescriptor copy constructor (private)
          */
         InstrumentationValueDescriptor ( InstrumentationValueDescriptor &ivd );
         /*! \brief InstrumentationValueDescriptor copy assignment operator (private)
          */
         InstrumentationValueDescriptor& operator= ( InstrumentationValueDescriptor &ivd );
      public:
         /*! \brief InstrumentationValueDescriptor constructor
          */
         InstrumentationValueDescriptor ( nanos_event_value_t id, const std::string &description ) : _id( id ), _description ( description ) {}

         /*! \brief InstrumentationValueDescriptor constructor
          */
         InstrumentationValueDescriptor ( nanos_event_value_t id, const char *description ) : _id( id ), _description ( description ) {}

         /*! \brief InstrumentationValueDescriptor destructor
          */
         ~InstrumentationValueDescriptor() {}

         /*! \brief Gets value descriptor id
          */
         nanos_event_value_t getId ( void );

         /*! \brief Gets value descriptor textual description
          */
         const std::string getDescription ( void );

   };

   class InstrumentationKeyDescriptor
   {
      public:
         typedef TR1::unordered_map<std::string, InstrumentationValueDescriptor*> ValueMap;
         typedef ValueMap::iterator ValueMapIterator;
         typedef ValueMap::const_iterator ConstValueMapIterator;
      private:
         nanos_event_key_t    _id;          /**< InstrumentationKeyDescriptor id */
         std::string          _description; /**< InstrumenotrKeyDescriptor description */
         Atomic<unsigned int> _totalValues; /**< Total number of values */
         Lock                 _lock;        /**< _valueMap exclusive lock */
         ValueMap             _valueMap;    /**< Registered Value elements */
      private:
         /*! \brief InstrumentationKeyDescriptor default constructor (private)
          */
         InstrumentationKeyDescriptor ();
         /*! \brief InstrumentationKeyDescriptor copy constructor (private)
          */
         InstrumentationKeyDescriptor ( InstrumentationKeyDescriptor &ikd );
         /*! \brief InstrumentationKeyDescriptor copy assignment operator (private)
          */
         InstrumentationKeyDescriptor& operator= ( InstrumentationKeyDescriptor &ikd );
      public:
         /*! \brief InstrumentationKeyDescriptor constructor
          */
         InstrumentationKeyDescriptor ( nanos_event_key_t id, const std::string &description ) : _id( id ), _description ( description ),
                                     _totalValues(1), _lock(), _valueMap() {}

         /*! \brief InstrumentationKeyDescriptor constructor
          */
         InstrumentationKeyDescriptor ( nanos_event_key_t id, const char *description ) : _id( id ), _description ( description ),
                                     _totalValues(1), _lock(), _valueMap() {}

         /*! \brief InstrumentationKeyDescriptor destructor
          */
         ~InstrumentationKeyDescriptor() {}

         /*! \brief Gets key descriptor id
          */
         nanos_event_key_t getId ( void );

         /*! \brief Gets key descriptor textual description
          */
         const std::string getDescription ( void );

         /*! \brief Inserts (or gets) a value into (from) valueMap 
          */
         nanos_event_value_t registerValue ( const std::string &value, const std::string &description="", bool abort_when_registered=true );

         /*! \brief Inserts (or gets) a value into (from) valueMap 
          */
         nanos_event_value_t registerValue ( const char *value, const char *description="", bool abort_when_registered=true );

         /*! \brief Inserts a value into valueMap (the value is given by user)
          */
         void registerValue ( const std::string &value, nanos_event_value_t val,
                              const std::string &description="", bool abort_when_registered=true );

         /*! \brief Inserts a value into valueMap (the value is given by user)
          */
         void registerValue ( const char *value, nanos_event_value_t val,
                              const char *description="", bool abort_when_registered=true );
         /*! \brief Gets a value into (from) valueMap 
          */
         nanos_event_value_t getValue ( const std::string &value );

         /*! \brief Gets a value into (from) valueMap 
          */
         nanos_event_value_t getValue ( const char *value );

         /*! \brief Returns starting point of valueMap ( iteration purposes )
          */
         ConstValueMapIterator beginValueMap ( void );

         /*! \brief Returns ending point of valueMap ( iteration purposes )
          */
         ConstValueMapIterator endValueMap ( void );

         /*! \brief Returns a Value description for a given value
          */
         const std::string getValueDescription ( nanos_event_value_t val );
   };

   class InstrumentationDictionary {
      public:
         typedef TR1::unordered_map<std::string, InstrumentationKeyDescriptor*> KeyMap;
         typedef KeyMap::iterator KeyMapIterator;
         typedef KeyMap::const_iterator ConstKeyMapIterator;
      private:
         Atomic<unsigned int> _totalKeys; /**< Total number of keys */
         Lock                 _lock;      /**< Is the _keyMap exclusive lock */
         KeyMap               _keyMap;    /**< Registered Key elements */
         
         /*! \brief InstrumentationDictionary copy constructor (private)
          */
         InstrumentationDictionary ( InstrumentationDictionary &id );
         /*! \brief InstrumentationDictionary copy assignment operator (private)
          */
         InstrumentationDictionary& operator= ( InstrumentationDictionary &id );
      public:
         /*! \brief InstrumentationDictionary default constructor
          */
         InstrumentationDictionary () : _totalKeys(1), _lock(), _keyMap()
         {
            /* ******************************************** */
            /* Instrumentation events: In order initialization */
            /* ******************************************** */

            /* 01 */ registerEventKey("api","Nanos Runtime API"); 
            registerEventValue("api","find_slicer","nanos_find_slicer()");
            registerEventValue("api","wg_wait_completion","nanos_wg_wait_completion()");
            registerEventValue("api","*_create_sync_cond","nanos_create_xxx_cond()");
            registerEventValue("api","sync_cond_wait","nanos_sync_cond_wait()");
            registerEventValue("api","sync_cond_signal","nanos_sync_cond_signal()");
            registerEventValue("api","destroy_sync_cond","nanos_destroy_sync_cond()");
            registerEventValue("api","wait_on","nanos_wait_on()");
            registerEventValue("api","init_lock","nanos_init_lock()");
            registerEventValue("api","set_lock","nanos_set_lock()");
            registerEventValue("api","unset_lock","nanos_unset_lock()");
            registerEventValue("api","try_lock","nanos_try_lock()");
            registerEventValue("api","destroy_lock","nanos_destroy_lock()");
            registerEventValue("api","single_guard","nanos_single_guard()");
            registerEventValue("api","team_barrier","nanos_team_barrier()");
            registerEventValue("api","current_wd", "nanos_current_wd()");
            registerEventValue("api","get_wd_id","nanos_get_wd_id()");
            registerEventValue("api","*_create_wd","nanos_create_xxx_wd()");
            registerEventValue("api","submit","nanos_submit()");
            registerEventValue("api","create_wd_and_run","nanos_create_wd_and_run()");
            registerEventValue("api","set_internal_wd_data","nanos_set_internal_wd_data()");
            registerEventValue("api","get_internal_wd_data","nanos_get_internal_wd_data()");
            registerEventValue("api","yield","nanos_yield()");
            registerEventValue("api","create_team","nanos_create_team()");
            registerEventValue("api","leave_team","nanos_leave_team()");
            registerEventValue("api","end_team","nanos_end_team()");
            registerEventValue("api","get_num_runnin_tasks","nanos_get_num_runnin_tasks()");
            registerEventValue("api","get_addr","nanos_get_addr()");
            registerEventValue("api","copy_value","nanos_copy_value()");
            registerEventValue("api","omp_barrier","nanos_omp_barrier()");

            /* 02 */ registerEventKey("wd-id","Work Descriptor id:");

            /* 03 */ registerEventKey("cache-copy-in","Transfer data into device cache");
            /* 04 */ registerEventKey("cache-copy-out","Transfer data to main memory");
            /* 05 */ registerEventKey("cache-local-copy","Local copy in device memory");
            /* 06 */ registerEventKey("cache-malloc","Memory allocation in device cache");
            /* 07 */ registerEventKey("cache-free","Memory free in device cache");
            /* 08 */ registerEventKey("cache-hit","Hit in the cache");

            /* 09 */ registerEventKey("copy-in","Copying WD inputs");
            /* 10 */ registerEventKey("copy-out","Copying WD outputs");

            /* 11 */ registerEventKey("user-funct-name","User Function Name");

            /* 12 */ registerEventKey("user-code","User Code (wd)");

            /* 13 */ registerEventKey("create-wd-id","Create WD Id:");
            /* 14 */ registerEventKey("create-wd-ptr","Create WD pointer:");
            /* 15 */ registerEventKey("wd-num-deps","Create WD num. deps."); 
            /* 16 */ registerEventKey("wd-deps-ptr","Create WD dependence pointer"); 

            /* 17 */ registerEventKey("lock-addr","Lock address"); 

            /* 18 */ registerEventKey("num-spins","Number of Spins"); 
            /* 19 */ registerEventKey("num-yields","Number of Yields"); 
            /* 20 */ registerEventKey("time-yields","Time on Yield (in nsecs)");

            /* 21 */ registerEventKey("user-funct-location","User Function Location");

            /* 22 */ registerEventKey("num-ready","Number of ready tasks in the queues");
            /* 23 */ registerEventKey("graph-size","Number tasks in the graph");

            /* 24 */ registerEventKey("loop-lower","Loop lower bound");
            /* 25 */ registerEventKey("loop-upper","Loop upper");
            /* 26 */ registerEventKey("loop-step","Loop step");

            /* 27 */ registerEventKey("in-cuda-runtime","Inside CUDA runtime");

            /* ** */ registerEventKey("debug","Debug Key"); /* Keep this key as the last one */
         }

         /*! \brief InstrumentationDictionary destructor
          */
         ~InstrumentationDictionary() {}

         /*! \brief Inserts (or gets) a key into (from) the keyMap
          */
         nanos_event_key_t registerEventKey ( const std::string &key, const std::string &description="", bool abort_when_registered=true );

         /*! \brief Inserts (or gets) a key into (from) the keyMap
          */
         nanos_event_key_t registerEventKey ( const char *key, const char *description="", bool abort_when_registered=true );

         /*! \brief Gets a key into (from) the keyMap
          */
         nanos_event_key_t getEventKey ( const std::string &key );

         /*! \brief Gets a key into (from) the keyMap
          */
         nanos_event_key_t getEventKey ( const char *key );

         /*! \brief Inserts (or gets) a value into (from) the valueMap (which belongs to 'key' parameter )
          */
         nanos_event_value_t registerEventValue ( const std::string &key, const std::string &value,
                                                  const std::string &description="", bool abort_when_registered=true );

         /*! \brief Inserts (or gets) a value into (from) the valueMap (which belongs to 'key' parameter )
          */
         nanos_event_value_t registerEventValue ( const char *key, const char *value, const char *description="", bool abort_when_registered=true );

         /*! \brief Inserts a value into the valueMap, which belongs to 'key' parameter (value is given by user)
          */
         void registerEventValue ( const std::string &key, const std::string &value,
                                   nanos_event_value_t val,
                                   const std::string &description="", bool abort_when_registered=true );

         /*! \brief Inserts a value into the valueMap, which belongs to 'key' parameter (value is given by user)
          */
         void registerEventValue ( const char *key, const char *value, nanos_event_value_t val,
                                   const char *description="", bool abort_when_registered=true );

         /*! \brief Gets a value into (from) the valueMap (which belongs to 'key' parameter )
          */
         nanos_event_value_t getEventValue ( const std::string &key, const std::string &value );

         /*! \brief Gets a value into (from) the valueMap (which belongs to 'key' parameter )
          */
         nanos_event_value_t getEventValue ( const char *key, const char *value );

         /*! \brief Returns starting point of keyMap ( iteration purposes )
          */
         ConstKeyMapIterator beginKeyMap ( void );

         /*! \brief Returns ending point of keyMap ( iteration purposes )
          */
         ConstKeyMapIterator endKeyMap ( void );
         
         /*! \brief Returns a Key description for a given key
          */
         const std::string getKeyDescription ( nanos_event_key_t key );

         /*! \brief Returns a Value description for a given key and a value
          */
         const std::string getValueDescription ( nanos_event_key_t key, nanos_event_value_t val );

   };
#endif

   class Instrumentation 
   {
      public:
         class Event {
            public:
               typedef std::pair<nanos_event_key_t,nanos_event_value_t>   KV;
               typedef KV *KVList;
               typedef const KV *ConstKVList;
            private:
               nanos_event_type_t          _type;         /**< Event type */
               nanos_event_state_value_t   _state;        /**< Event state */

               unsigned int                _nkvs;         /**< Number of kvs elements */
               KVList                      _kvList;       /**< List of elements kv (key.value) */

               nanos_event_domain_t        _ptpDomain;    /**< A specific domain in which ptpId is unique */
               nanos_event_id_t            _ptpId;        /**< PtP event id */


            public:
               /*! \brief Event default constructor 
                *
                *  \see State Burst Point PtP
                */
               Event () : _type((nanos_event_type_t) 0), _state((nanos_event_state_value_t) 0), _nkvs(0),
                          _kvList(NULL), _ptpDomain((nanos_event_domain_t) 0), _ptpId(0) {}
               /*! \brief Event constructor
                *
                *  Generic constructor used by all other specific constructors
                *
                *  \see State Burst Point PtP
                */
               Event ( nanos_event_type_t type, nanos_event_state_value_t state, unsigned int nkvs, KVList kvlist,
                       nanos_event_domain_t ptp_domain, nanos_event_id_t ptp_id ) :
                     _type (type), _state (state), _nkvs(nkvs), _kvList (kvlist), 
                     _ptpDomain (ptp_domain), _ptpId (ptp_id)
               { }

               /*! \brief Event copy constructor
                */
               Event ( const Event & evt )
               {
                  _type = evt._type;
                  _state = evt._state;
                  _nkvs = evt._nkvs;
                  _kvList = NEW KV[_nkvs];
                  for ( unsigned int i = 0; i < _nkvs; i++ ) {
                     _kvList[i] = evt._kvList[i];
                  }
                  _ptpDomain = evt._ptpDomain;
                  _ptpId     = evt._ptpId;

               }

               /*! \brief Event copy assignment operator
                */
               void operator= ( const Event & evt ) 
               { 
                  // self-assignment: ok
                  if ( this == &evt ) return; 
                    
                  _type = evt._type;
                  _state = evt._state;
                  _nkvs = evt._nkvs;
                  _kvList = NEW KV[_nkvs];
                  for ( unsigned int i = 0; i < _nkvs; i++ ) {
                     _kvList[i] = evt._kvList[i];
                  }
                  _ptpDomain = evt._ptpDomain;
                  _ptpId     = evt._ptpId;

               }

               /*! \brief Event destructor
                */
               ~Event() { delete[] _kvList; }

               /*! \brief Get event type
                */
               nanos_event_type_t getType () const; 

               /*! \brief Get event state
                */
               nanos_event_state_value_t getState ();

               /*! \brief Get number of kvs
                */
               unsigned int getNumKVs () const;

               /*! \brief Get kvs vector
                */
               ConstKVList getKVs () const;

               /*! \brief Get specific domain ( useful in PtP events)
                *  \see getId
                */
               unsigned int getDomain ( void ) const;

               /*! \brief Get event id (unique in a specific domain, useful in PtP events)
                *  \see getDomain
                */
               unsigned int getId( void ) const;

               /*! \brief Change event type to the complementary value (i.e. if type is BURST_START it changes to BURST_END)
                */
               void reverseType ( );
         };
         class State : public Event {
            private:
              /*! \brief State event default constructor (private)
               */
               State();
              /*! \brief State event copy constructor (private)
               */
               State( State &s);
              /*! \brief State event copy constructor (private)
               */
               State& operator= ( State &s);
            public:
              /*! \brief State event constructor
               */
              State ( nanos_event_type_t type = NANOS_STATE_START, nanos_event_state_value_t state = NANOS_ERROR ) 
                    : Event (type, state, 0, NULL, (nanos_event_domain_t) 0, (nanos_event_id_t) 0 ) { }
         };
         class Burst : public Event {
             private:
               /*! \brief Burst event default constructor (private)
                */
               Burst();
               /*! \brief Burst event copy constructor (private)
                */
               Burst( Burst &b);
               /*! \brief Burst event copy constructor (private)
                */
               Burst& operator= ( Burst &b);
             public:
               /*! \brief Burst event constructor
                */
               Burst ( bool start, KV *kv )
                     : Event ( start? NANOS_BURST_START: NANOS_BURST_END, NANOS_ERROR, 1, kv, (nanos_event_domain_t) 0, (nanos_event_id_t) 0 ) { }
         };
         class Point : public Event {
             private:
               /*! \brief Point event default constructor (private)
                */
               Point();
               /*! \brief Point event copy constructor (private)
                */
               Point( Point &p );
               /*! \brief Point event copy assignment operator (private)
                */
               Point& operator= ( Point &p );
             public:
               /*! \brief Point event constructor
                */
               Point ( unsigned int nkvs, KVList kvlist )
                     : Event ( NANOS_POINT, NANOS_ERROR, nkvs, kvlist, (nanos_event_domain_t) 0, (nanos_event_id_t) 0 ) { }
         };
         class PtP : public Event {
            private:
               /*! \brief PtP event default constructor (private)
                */
               PtP();
               /*! \brief PtP event copy constructor (private)
                */
               PtP( PtP &ptp);
               /*! \brief PtP event copy assignment operator (private)
                */
               PtP& operator= ( PtP &ptp);
            public:
               /*! \brief PtP event constructor
                */
               PtP ( bool start, nanos_event_domain_t domain, nanos_event_id_t id, unsigned int nkvs,  KVList kvlist )
                   : Event ( start ? NANOS_PTP_START : NANOS_PTP_END , NANOS_ERROR, nkvs, kvlist, domain, id ) { }
         };
#ifndef NANOS_INSTRUMENTATION_ENABLED
      public:
         Instrumentation () {}
         ~Instrumentation () {}
#else
      protected: /* They can be accessed by plugins (derived classes ) */
         InstrumentationDictionary      _instrumentationDictionary; /**< Instrumentation Dictionary (allow to register keys and values) */
         InstrumentationContext        &_instrumentationContext; /**< Instrumentation Context */
      private:
         /*! \brief Instrumentation default constructor (private)
          */
         Instrumentation();
         /*! \brief Instrumentation copy constructor (private)
          */
         Instrumentation( Instrumentation &i);
         /*! \brief Instrumentation copy assignment operator (private)
          */
         Instrumentation& operator= ( Instrumentation &i);
      public:
         /*! \brief Instrumentation constructor
          */
         Instrumentation( InstrumentationContext &ic ) : _instrumentationDictionary(), _instrumentationContext(ic) {}

         /*! \brief Instrumentation destructor
          */
         virtual ~Instrumentation() {}

         /*! \brief Gets InstrumentationDictionary
          *
          */
         InstrumentationDictionary * getInstrumentationDictionary ( void );

         // low-level instrumentation interface (pure virtual functions)

         /*! \brief Pure virtual functions executed at the beginning of instrumentation phase
          *
          *  Each of (specific) instrumentation modules have to implement this function in order
          *  to be consistent with the instrumentation model
          */
         virtual void initialize( void ) = 0;

         /*! \brief Pure virtual functions executed at the end of instrumentation phase
          *
          *  Each of (specific) instrumentation modules have to implement this function in order
          *  to be consistent with the instrumentation model
          */
         virtual void finalize( void ) = 0;

         /*! \brief Pure virtual functions executed each time runtime wants to add an event
          *
          *  Each of (specific) instrumentation modules have to implement this function in order
          *  to be consistent with the instrumentation model. This function includes several
          *  events in a row to facilitate implementation in which several events occurs at
          *  the same time (i.e. same timestamp).
          *
          *  \param[in] count is the number of events
          *  \param[in] events is a vector of 'count' events
          */
         virtual void addEventList ( unsigned int count, Event *events ) = 0;

         // CORE: high-level instrumentation interface (virtual functions)

         /*! \brief Used when creating a work descriptor (initializes instrumentation context associated to a WD)
          */   
         virtual void wdCreate( WorkDescriptor* newWD );

         /*! \brief Used in work descriptor context switch (oldWD has finished completely its execution
          *
          *  \param[in] oldWD, is the work descriptor which leaves the cpu
          *  \param[in] newWD, is the work descriptor which enters the cpu
          */
         virtual void wdSwitch( WorkDescriptor* oldWD, WorkDescriptor* newWD, bool last = false );

         /*! \brief Used by higher levels to create a BURST_START event
          *
          *  \param[in,out] e is an event reference, preallocated by the caller
          *  \param[in] key is the key in the related  pair <key,value>
          *  \param[in] value is the value in related pair <key,value>
          */
         void  createBurstEvent ( Event *e, nanos_event_key_t key, nanos_event_value_t value, InstrumentationContextData *icd = NULL );

         /*! \brief Used by higher levels to create a BURST_END event
          *
          *  \param[in,out] e is an event reference, preallocated by the caller
          *  \param[in] key is the key in the related  pair <key,value>
          *  \param[in] value is the value in related pair <key,value>
          */
         void closeBurstEvent ( Event *e, nanos_event_key_t key, InstrumentationContextData *icd = NULL );

         /*! \brief Used by higher levels to create a STATE event
          *
          *  \param[in,out] e is an event reference, preallocated by the caller
          *  \param[in] state is the state value for the event
          */
         void createStateEvent ( Event *e, nanos_event_state_value_t state );

         /*! \brief Used by higher levels to create a STATE event (value will be previous state in instrumentation context info) 
          *
          *  \param[in,out] e is an event reference, preallocated by the caller
          */
         void returnPreviousStateEvent ( Event *e );

         /*! \brief Used by higher levels to create a POINT (punctual) event
          *
          *  The created event will contain a vector of nkvs pairs <key,value> that are build from
          *  separated vectors of keys and values respectively (received as a parameters).
          *
          *  \param[in,out] e is an event reference, preallocated by the caller
          *  \param[in] nkvs is the number of pairs <key,value> related with the new event
          *  \param[in] key is a vector of nkvs keys 
          *  \param[in] value is a vector of nkvs  values
          */
         void createPointEvent ( Event *e, unsigned int nkvs, nanos_event_key_t *keys, nanos_event_value_t *values );

         /*! \brief Used by higher levels to create a PTP_START event
          *
          *  The created event will contain a vector of nkvs pairs <key,value> that are build from
          *  separated vectors of keys and values respectively (received as a parameters).
          *
          *  \param[in,out] e is an event reference, preallocated by the caller
          *  \param[in] domain specifies a specific domain in which id is a unique value
          *  \param[in] id is a unique id in a given domain context
          *  \param[in] nkvs is the number of pairs <key,value> related with the new event
          *  \param[in] key is a vector of nkvs keys 
          *  \param[in] value is a vector of nkvs  values
          */
         void createPtPStart ( Event *e, nanos_event_domain_t domain, nanos_event_id_t id,
                               unsigned int nkvs, nanos_event_key_t *keys, nanos_event_value_t *values );

         /*! \brief Used by higher levels to create a PTP_END event
          *
          *  The created event will contain a vector of nkvs pairs <key,value> that are build from
          *  separated vectors of keys and values respectively (received as a parameters).
          *
          *  \param[in,out] e is an event reference, preallocated by the caller
          *  \param[in] domain specifies a specific domain in which id is a unique value
          *  \param[in] id is a unique id in a given domain context
          *  \param[in] nkvs is the number of pairs <key,value> related with the new event
          *  \param[in] key is a vector of nkvs keys 
          *  \param[in] value is a vector of nkvs  values
          */
         void createPtPEnd ( Event *e, nanos_event_domain_t domain, nanos_event_id_t id,
                             unsigned int nkvs, nanos_event_key_t *keys, nanos_event_value_t *values );

         /*! \brief Used by higher levels to create a deferred POINT event into a given WorkDescriptor (wd)
          */
         void createDeferredPointEvent ( WorkDescriptor &wd, unsigned int nkvs, nanos_event_key_t *keys,
                                         nanos_event_value_t *values );

         /*! \brief Used by higher levels to create a deferred PTP_START event into a given WorkDescriptor (wd)
          */
         void createDeferredPtPStart ( WorkDescriptor &wd, nanos_event_domain_t domain, nanos_event_id_t id,
                                       unsigned int nkvs, nanos_event_key_t *keys, nanos_event_value_t *values );

         /*! \brief Used by higher levels to create a deferred PTP_END event into a given WorkDescriptor (wd)
          */
         void createDeferredPtPEnd ( WorkDescriptor &wd, nanos_event_domain_t domain, nanos_event_id_t id,
                                     unsigned int nkvs, nanos_event_key_t *keys, nanos_event_value_t *values );

         void raisePointEvent ( nanos_event_key_t key, nanos_event_value_t val );
         void raisePointEventNkvs ( unsigned int nkvs, nanos_event_key_t *key, nanos_event_value_t *val );

         void raiseOpenStateEvent ( nanos_event_state_value_t state );
         void raiseCloseStateEvent ( void );

         void raiseOpenBurstEvent ( nanos_event_key_t key, nanos_event_value_t val );
         void raiseCloseBurstEvent ( nanos_event_key_t key );

         void raiseOpenPtPEvent ( nanos_event_domain_t domain, nanos_event_id_t id, nanos_event_key_t key, nanos_event_value_t val );
         void raiseOpenPtPEventNkvs ( nanos_event_domain_t domain, nanos_event_id_t id, unsigned int nkvs,
                                      nanos_event_key_t *key, nanos_event_value_t *val );
         void raiseClosePtPEvent ( nanos_event_domain_t domain, nanos_event_id_t id, nanos_event_key_t key, nanos_event_value_t val ); 
         void raiseClosePtPEventNkvs ( nanos_event_domain_t domain, nanos_event_id_t id, unsigned int nkvs,
                                       nanos_event_key_t *key, nanos_event_value_t *val ); 

         void raiseOpenStateAndBurst ( nanos_event_state_value_t state, nanos_event_key_t key, nanos_event_value_t val );
         void raiseCloseStateAndBurst ( nanos_event_key_t key );

         void disableStateEvents ( nanos_event_state_value_t state );
         void enableStateEvents ( void ); 
#endif
   };
}
#endif
