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

#ifndef _NANOS_DEPENDENCY_DECL
#define _NANOS_DEPENDENCY_DECL

#include "nanos-int.h"

namespace nanos
{

  /*! \class Dependency
   *  \brief Contains information about dependencies necessary to add a DependableObject to the Dependencies system
   */
   class Dependency : public nanos_dependence_internal_t
   {
      public:
         /*! \brief Dependency default constructor
          *
          *  \param address Address of the dependency's address 
          *  \param input Whether the dependency is input or not 
          *  \param output Whether the dependency is output or not
          *  \param canRename Whether the dependency can rename or not
          */
         Dependency ( void ** addr = NULL, ptrdiff_t offset = 0, bool input = false, bool output = false,
                      bool canRename = false, bool commutative = false, size_t storageSize = 0 );

         /*! \brief Dependency copy constructor
          *
          *  \param obj another Dependency
          */
         Dependency ( const Dependency &dep );

        /*! \brief Dependency copy asssignment operator, can be self-assigned.
         *
         *  \param obj another Dependency
         */
         const Dependency & operator= ( const Dependency &dep );

         /*! \brief Dependency destructor
          */
         ~Dependency () {}
         
        /*! \brief Obtain the dependency's address address
         */
         void ** getAddress() const;
         
        /*! \brief Obtain the dependency's address address
         */
         ptrdiff_t getOffset() const;

        /*! \brief Compute the dependency address
         */
         void * getDepAddress() const;
         
        /*! \brief returns true if it is an input dependency
         */
         bool isInput() const;

        /*! \brief sets the dependency input clause to b
         */
         void setInput( bool b );
         
        /*! \brief returns true if it is an output dependency
         */
         bool isOutput() const;

        /*! \brief sets the dependency output clause to b
         */
         void setOutput( bool b );
         
        /*! \brief return true if the dependency can rename
         */
         bool canRename() const;

        /*! \brief sets the rename attribute to b
         */
         void setCanRename( bool b );

        /*! \brief returns true if there is a commutative over this dependency
         */
         bool isCommutative() const;

        /*! \brief sets the dependency to be a commutative
         */
         void setCommutative( bool b );
         
   };
}

#endif
