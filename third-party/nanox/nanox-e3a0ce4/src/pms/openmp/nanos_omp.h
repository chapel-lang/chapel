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

#ifndef _NANOS_OMP_H_
#define _NANOS_OMP_H_

#include "nanos.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _MERCURIUM
// define OpenMP API version
#pragma nanos interface family(openmp) version(3)
#endif

nanos_err_t nanos_omp_single ( bool *);
nanos_err_t nanos_omp_barrier ( void );

void nanos_omp_set_interface ( void * );

#ifdef __cplusplus
}
#endif

#endif
