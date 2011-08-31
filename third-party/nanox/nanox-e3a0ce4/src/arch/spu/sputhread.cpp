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

#include "sputhread.hpp"
#include "system.hpp"
#include <libspe2.h>

using namespace nanos;
using namespace nanos::ext;

extern spe_program_handle_t spu_idle;

void SPUThread::bootstrap ()
{
    spe_context_ptr_t ctx;
    unsigned int entry = SPE_DEFAULT_ENTRY;

    debug("Starting SPU bootstrap");
    if ((ctx = spe_context_create (0, NULL)) == NULL) {
      perror ("Failed creating context");
      exit (1);
    }

    if (spe_program_load (ctx, &spu_idle)) {
      perror ("Failed loading program");
      exit (1);
    }
    
    if (spe_context_run(ctx, &entry, 0, 0, (void*)0, NULL) < 0 ) {
      perror ("Failed running context");
      exit (1);
    }
}

void SPUThread::start ()
{
   getPPU().start();
}

void SPUThread::join ()
{
   getPPU().join();
}

void SPUThread::runDependent ()
{
}

void SPUThread::inlineWorkDependent( WD &work )
{
}

void SPUThread::switchTo( WD *work )
{
}

void SPUThread::exitTo( WD *work )
{
}

void SPUThread::bind ()
{
   getPPU().bind();
}
