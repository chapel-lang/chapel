/*************************************************************************************/
/*      Copyright 2010 Barcelona Supercomputing Center                               */
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

#ifndef NANOS_PM_INTERFACE_DECL
#define NANOS_PM_INTERFACE_DECL

#include "config_fwd.hpp"
#include "workdescriptor_decl.hpp"
#include "threadteam_decl.hpp"

class PMInterface
{
   private:
      /*! \brief PMInterface copy constructor (private)
       */
      PMInterface( PMInterface &pmi );
      /*! \brief PMInterface copy assignment operator (private)
       */
      PMInterface& operator= ( PMInterface &pmi );
   public:
      /*! \brief PMInterface default constructor
       */
      PMInterface() {}
      /*! \brief PMInterface destructor
       */
      virtual ~PMInterface() {}

      virtual int getInternalDataSize() const { return 0; }
      virtual int getInternalDataAlignment() const { return 1; }

      virtual void config (nanos::Config &cfg) {}
      virtual void start () {}
      virtual void finish() {}

      virtual void setupWD( nanos::WD &wd ) {}
      virtual void wdStarted( nanos::WD &wd ) {}
      virtual void wdFinished( nanos::WD &wd ) {}

      virtual nanos::ThreadTeamData* getThreadTeamData() { return NEW nanos::ThreadTeamData(); }
};

#endif /* PM_INTERFACE_HPP_ */
