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

#ifndef _NANOS_SYNCHRONIZED_CONDITION_DECL
#define _NANOS_SYNCHRONIZED_CONDITION_DECL

#include <stdlib.h>
#include <list>
#include <vector>
#include "atomic_decl.hpp"
#include "debug.hpp"
#include "workdescriptor_fwd.hpp"

namespace nanos
{
  /*! \brief Represents an object that checks a given condition.
   */
   class ConditionChecker
   {
      public:
         /*! \brief ConditionChecker default constructor
          */
         ConditionChecker() {}
         /*! \brief ConditionChecker copy constructor
          */
         ConditionChecker ( const ConditionChecker & cc ) {}
         /*! \brief ConditionChecker copy assignment operator
          */
         ConditionChecker& operator=( const ConditionChecker & cc )
         {
            return *this;
         }
         /*! \brief ConditionChecker destructor
          */
         virtual ~ConditionChecker() {}
         /*! \brief interface used by the SynchronizedCondition to check the condition (pure virtual)
          */
         virtual bool checkCondition() = 0;
   };

  /*! \brief Checks a templated variable for equality with a given condition.
   */
   template<typename T>
   class EqualConditionChecker : public ConditionChecker
   {
      protected:
         volatile T     *_var;       /**< variable wich value has to be checked. */
         T               _condition; /**< variable value to ckeck for. */
      public:
         /*! \brief EqualConditionChecker default constructor
          */
         EqualConditionChecker() : ConditionChecker(), _var( NULL ), _condition() {}
         /*! \brief EqualConditionChecker destructor
          */
         EqualConditionChecker(volatile T* var, T condition) : ConditionChecker(), _var( var ), _condition( condition ) {}
         /*! \brief EqualConditionChecker copy constructor
          */
         EqualConditionChecker ( const EqualConditionChecker & cc ) 
         {
            this->_var = cc._var;
            this->_condition = cc._condition;
         }
         /*! \brief EqualConditionChecker copy assignment operator
          */
         EqualConditionChecker& operator=( const EqualConditionChecker & cc )
         {
            this->_var = cc._var;
            this->_condition = cc._condition;
            return *this;
         }
         /*! \brief EqualConditionChecker destructor
          */
         virtual ~EqualConditionChecker() {}
         /*! \brief Checks the variable against the condition.
          */
         virtual bool checkCondition() {
            return ( *(this->_var) == (this->_condition) );
         }
   };

  /*! \brief Checks a templated variable for being less or equal than a condition.
   */
   template<typename T>
   class LessOrEqualConditionChecker : public ConditionChecker
   {
      protected:
         volatile T     *_var;       /**< variable wich value has to be checked. */
         T               _condition; /**< variable value to ckeck for. */
      public:
         /*! \brief LessOrEqualConditionChecker default constructor
          */
         LessOrEqualConditionChecker() : ConditionChecker(), _var( NULL ), _condition() {}
         /*! \brief LessOrEqualConditionChecker copy constructor
          */
         LessOrEqualConditionChecker ( const LessOrEqualConditionChecker & cc ) 
         {
            this->_var = cc._var;
            this->_condition = cc._condition;
         }
         /*! \brief LessOrEqualConditionChecker copy assignment operator
          */
         LessOrEqualConditionChecker& operator=( const LessOrEqualConditionChecker & cc )
         {
            this->_var = cc._var;
            this->_condition = cc._condition;
            return *this;
         }
         /*! \brief LessOrEqualConditionChecker constructor - 1
          */
         LessOrEqualConditionChecker(volatile T* var, T condition)
            : ConditionChecker(), _var(var), _condition(condition) {}
         /*! \brief LessOrEqualConditionChecker destructor
          */
         virtual ~LessOrEqualConditionChecker() {}
         /*! \brief Checks the variable against the condition.
          */
         virtual bool checkCondition() {
            return ( *(this->_var) <= (this->_condition) );
         }
   };

  /*! \brief Abstract synchronization class.
   */
   class GenericSyncCond
   {
      private:
         Lock _lock; /**< Lock to block and unblock WorkDescriptors securely. */
      private:
         /*! \brief GenericSyncCond copy constructor (disabled)
          */
         GenericSyncCond ( const GenericSyncCond & gsc );
         /*! \brief GenericSyncCond copy assignment operator (disabled)
          */
         GenericSyncCond& operator=( const GenericSyncCond & gsc );
      public:
         /*! \brief GenericSyncCond default constructor
          */
         GenericSyncCond() : _lock() {}
         /*! \brief GenericSyncCond destructor
          */
         virtual ~GenericSyncCond() {}

         virtual void wait() = 0;
         virtual void signal() = 0;
         virtual void signal_one() = 0;
         virtual bool check() = 0;

        /*! \brief Sets a waiter Workdescriptor.
         */
         virtual void addWaiter( WorkDescriptor* wd) = 0;

        /*! \brief Returns true if there's any waiter on the condition.
         */
         virtual bool hasWaiters() = 0;

        /*! \brief Returns a waiter adn removes it from the condition.
         */
         virtual WorkDescriptor* getAndRemoveWaiter() = 0;

       
        /*! \brief acquire the lock.
         */
         void lock();

         /*! \brief Release the lock. The wait() method can switch context
          * so it is necessary this function to be public so that the switchHelper
          * can unlock it after removing the current WD from the stack.
          */
         void unlock();
   };

  /*! \brief Abstract template synchronization class.
   */
   template<class _T>
   class SynchronizedCondition : public GenericSyncCond
   {
      protected:
         _T _conditionChecker; /**< ConditionChecker associated to the SynchronizedCondition. */
         Atomic<int> _refcount; /**< References counter for waits that need to make sure there are no pending signals */
      
      public:
        /*! \brief SynchronizedCondition default constructor
         */
         SynchronizedCondition ( ) : GenericSyncCond(), _conditionChecker(), _refcount(0) { }

        /*! \brief SynchronizedCondition copy constructor
         */
         SynchronizedCondition ( const SynchronizedCondition & sc ) : GenericSyncCond(), _conditionChecker( sc._conditionChecker ), _refcount(0) { }

        /*! \brief SynchronizedCondition copy assignment operator
         */
         SynchronizedCondition& operator=( const SynchronizedCondition & sc )
         {
            this->_conditionChecker = sc._conditionChecker;
            return *this;
         }

        /*! \brief SynchronizedCondition constructor - 1
         */
         SynchronizedCondition ( _T cc ) : GenericSyncCond(), _conditionChecker(cc), _refcount(0) { }

        /*! \brief SynchronizedCondition destructor
         */
         virtual ~SynchronizedCondition() { }

        /*! \brief increase references
         */
         void reference();

        /*! \brief decrease references
         */
         void unreference();
 
        /*! \brief Wait until the condition has been satisfied ensures that there condition will not be signaled after the wait
         */
         void waitConditionAndSignalers();

        /*! \brief Wait until the condition has been satisfied
         */
         void wait();

        /*! \brief Signal the waiters if the condition has been satisfied. If they
         * are blocked they will be set to ready and enqueued.
         */
         void signal();

        /*! \brief Signal only one waiter if the condition has been satisfied. If it
         * is blocked it will be set to ready and enqueued.
         */
         void signal_one();

        /*! \brief Change the condition checker associated to the synchronizedConditon object.
         */
         void setConditionChecker( _T cc )
         {
            _conditionChecker = cc;
         }

        /*! \brief Check the condition
         */
         bool check ()
         {
            return _conditionChecker.checkCondition();
         }
   };

  /*! \brief SynchronizedCondition specialization that checks equality fon one
   * variable and with just one waiter.
   */
   template<class _T>
   class SingleSyncCond : public SynchronizedCondition<_T>
   {
      private:
         WorkDescriptor* _waiter; /**< Pointer to the WD waiting for the condition. */
      public:
         /*! \brief SingleSyncCond default constructor
          */
         SingleSyncCond ( ) : SynchronizedCondition<_T>( ), _waiter( NULL ) { }

         /*! \brief SingleSyncCond copy constructor
          */
         SingleSyncCond ( const SingleSyncCond & ssc ) :  SynchronizedCondition<_T>( ssc ), _waiter( NULL ) {}
         /*! \brief SingleSyncCond copy assignment operator
          */
         SingleSyncCond& operator=( const SingleSyncCond & ssc )
         {
            this->_conditionChecker =  ssc._conditionChecker;
            return *this;
         }
         /*! \brief SingleSyncCond constructor - 1
          *
          * \param var Variable which value is used for synchronization.
          * \param condition Value expected
          */
         SingleSyncCond ( _T cc ) : SynchronizedCondition<_T>( cc ), _waiter( NULL ) { }
         /*! \brief SingleSyncCond destructor
          */
         virtual ~SingleSyncCond() { }

      protected:
        /*! \brief Sets the waiter to wd.
         */
         virtual void addWaiter( WorkDescriptor* wd )
         {
            _waiter = wd;
         }

        /*! \brief Returns true if there's a waiter.
         */
         virtual bool hasWaiters()
         {
            return _waiter != NULL;
         }

        /*! \brief Returns the waiter and sets it to NULL.
         */
         virtual WorkDescriptor* getAndRemoveWaiter()
         {
            WorkDescriptor* result = _waiter;
            _waiter = NULL;
            return result;
         }
   };

  /*! \brief SynchronizedCondition specialization that checks equality fon one
   * variable and with more than one waiter.
   */
   template <class _T>
   class MultipleSyncCond : public SynchronizedCondition<_T>
   {
      private:
         typedef std::vector<WorkDescriptor*> WorkDescriptorList; /**< type vector of workdescriptors. */
         WorkDescriptorList _waiters; /**< List of WDs that wait on the condition. */

      private:
         /*! \brief MultipleSyncCond default constructor (disabled)
          */
         MultipleSyncCond ();
      public:
         /*! \brief MultipleSyncCond constructor
          *
          * \param cc ConditionChecker needed for
          * \param 
          */
         MultipleSyncCond (_T cc, size_t size) : SynchronizedCondition<_T> ( cc ), _waiters()
         {
            _waiters.reserve(size);
         }
         /*! \brief MultipleSyncCond copy constructor
          *
          * \param ssc Another MultipleSyncCond
          */
         MultipleSyncCond ( const MultipleSyncCond & ssc ) :  SynchronizedCondition<_T>( ssc ), _waiters()
         {
         }
         /*! \brief MultipleSyncCond copy assignment operator
          *
          * \param ssc Another MultipleSyncCond
          */
         MultipleSyncCond& operator=( const MultipleSyncCond & ssc )
         {
            this->_conditionChecker = ssc._conditionChecker;
            return *this;
         }
         /*! \brief MultipleSyncCond constructor - 1
          */
         MultipleSyncCond ( size_t size) : SynchronizedCondition<_T> (), _waiters()
         {
            _waiters.reserve(size);
         }
         /*! \brief MultipleSyncCond destructor
          */
         virtual ~MultipleSyncCond() { }
         /*! \brief Set the number of waiters expected to wait on this condition.
          */
         void resize ( size_t size )
         {
            _waiters.reserve(size);
         }
      protected:
         /*! \brief Sets the waiter to wd.
          */
         virtual void addWaiter( WorkDescriptor* wd )
         {
            _waiters.push_back( wd );
         }
         /*! \brief Returns true if there's a waiter.
          */
         virtual bool hasWaiters()
         {
            return !( _waiters.empty() );
         }
         /*! \brief Takes one of the the waiters from the list returns NULL if the list is empty.
          */
         virtual WorkDescriptor* getAndRemoveWaiter()
         {
            if ( _waiters.empty() )
               return (WorkDescriptor*)NULL;
            WorkDescriptor* reslt =  _waiters.back();
            _waiters.pop_back();
            return reslt;
         }
   };
}

#endif
