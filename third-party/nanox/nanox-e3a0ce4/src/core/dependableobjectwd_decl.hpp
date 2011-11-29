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

#ifndef _NANOS_DEPENDABLE_OBJECT_WD_DECL
#define _NANOS_DEPENDABLE_OBJECT_WD_DECL

#include "synchronizedcondition_decl.hpp"
#include "dependableobject_decl.hpp"
#include "workdescriptor_fwd.hpp"

namespace nanos
{

   /*! \class DOSubmit
    *  \brief DependableObject representing a WorkDescriptor as Dependable entity
    */
   class DOSubmit : public DependableObject
   {
      private:
         WorkDescriptor *_submittedWD; /**< Pointer to the work descriptor represented by this DependableObject */

      public:
         /*! \brief DOSubmit default constructor
          */
         DOSubmit ( ) : DependableObject(), _submittedWD(NULL) { }

         /*! \brief DOSubmit constructor
          */
         DOSubmit ( WorkDescriptor* wd) : DependableObject ( ), _submittedWD( wd ) { }

         /*! \brief DOSubmit copy constructor
          *  \param dos another DOSubmit
          */
         DOSubmit ( const DOSubmit &dos ) : DependableObject(dos), _submittedWD( dos._submittedWD ) { } 

         /*! \brief DOSubmit assignment operator, can be self-assigned.
          *  \param dos another DOSubmit
          */
         const DOSubmit & operator= ( const DOSubmit &dos );

         /*! \brief DOSubmit virtual destructor
          */
         virtual ~DOSubmit ( ) { }

         /*! \brief Submits WorkDescriptor when dependencies are satisfied
          */
         virtual void dependenciesSatisfied ( );

         /*! \brief TODO 
          */
         void setWD( WorkDescriptor *wd );

         /*! \brief TODO 
          */
         unsigned long getDescription ( );

         /*! \brief Get the related object which actually has the dependence
          */
         virtual void * getRelatedObject ( );

         /*! \brief Instrument predecessor -> successor dependency
          */
         virtual void instrument ( DependableObject& successor );
   };

  /*! \brief DependableObject representing a WorkDescriptor as a task domain to wait on some dependencies
   */
   class DOWait : public DependableObject
   {
      private:
         WorkDescriptor     *_waitDomainWD; /**< Pointer to the WorkDescriptor that waits on data */
         volatile bool       _depsSatisfied; /**< Condition to satisfy before execution can go forward */
         SingleSyncCond<EqualConditionChecker<bool> >  _syncCond; /**< TODO */
      public:
         /*! \brief DOWait default constructor
          */
         DOWait ( ) : DependableObject(), _waitDomainWD(NULL), _depsSatisfied( false ),
            _syncCond( EqualConditionChecker<bool>( &_depsSatisfied, true ) ) { }

         /*! \brief DOWait constructor
          */
         DOWait ( WorkDescriptor *wd ) : DependableObject(), _waitDomainWD( wd ), _depsSatisfied( false ),
           _syncCond( EqualConditionChecker<bool>( &_depsSatisfied, true ) ) { }

         /*! \brief DOWait copy constructor
          *  \param dos another DOWait
          */
         DOWait ( const DOWait &dow ) : DependableObject(dow), _waitDomainWD( dow._waitDomainWD ), _depsSatisfied( false ),
           _syncCond( EqualConditionChecker<bool>( &_depsSatisfied, true ) ) { }

         /*! \brief DOWait assignment operator, can be self-assigned.
          *  param dos another DOWait
          */
         const DOWait & operator= ( const DOWait &dow );

         /*! \brief Virtual destructor
          */
         virtual ~DOWait ( ) { }

         /*! \brief Initialise wait condition
          */
         virtual void init ( );

         /*! \brief Wait method blocks execution untill dependencies are satisfied
          */
         virtual void wait ( std::list<Dependency *> deps );

         /*! \brief whether the DO gets blocked and no more dependencies can
          *  be submitted until it is satisfied.
          */
         virtual bool waits ( );

         /*! \brief Unblock method when dependencies are satisfied
          */
         virtual void dependenciesSatisfied ( );

         /*! \brief TODO
          */
         void setWD( WorkDescriptor *wd );

         /*! \brief Get the related object which actually has the dependence
          */
         virtual void * getRelatedObject ( );

         /*! \brief Instrument predecessor -> successor dependency
          */
         virtual void instrument ( DependableObject& successor );
   };
};

#endif

