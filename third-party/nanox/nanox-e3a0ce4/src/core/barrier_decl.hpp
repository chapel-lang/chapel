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

#ifndef __NANOS_BARRIER_DECL_H
#define __NANOS_BARRIER_DECL_H

namespace nanos
{
   class Barrier
   {
      public:
        /*! \brief Barrier default constructor
         */
         Barrier() {}
        /*! \brief Barrier copy constructor
         */
         Barrier(const Barrier &) {}
        /*! \brief Barrier copy assignment operator 
         */
         const Barrier & operator= ( const Barrier & ) { return *this; }
        /*! \brief Barrier destructor
         */
         virtual ~Barrier() {}
        /*! Init is called when the number of participants in the barrier are known so the barrier
         *  can initialize the proper data structures if needed
         *  \warning Must be called by just one thread
         */
         virtual void init ( int numParticipants ) { }
        /*! Called when the number of number of participants in the barrier changes to
         *  restructure data structures if needed
         *  \warning Must be called by just one thread
         */
         virtual void resize ( int numThreads ) { }
        /*! \brief Perform a barrier among the participants
         *
         *  When it returns it guarantees that all participants have reached the barrier
         */
         virtual void barrier (int participant) = 0;
   };

   typedef Barrier * ( *barrFactory ) ();

}

#endif
