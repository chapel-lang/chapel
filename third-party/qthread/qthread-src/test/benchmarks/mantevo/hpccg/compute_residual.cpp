// @HEADER
// ************************************************************************
//
//               HPCCG: Simple Conjugate Gradient Benchmark Code
//                 Copyright (2006) Sandia Corporation
//
// Under terms of Contract DE-AC04-94AL85000, there is a non-exclusive
// license for use of this work by or on behalf of the U.S. Government.
//
// This library is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as
// published by the Free Software Foundation; either version 2.1 of the
// License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA
// Questions? Contact Michael A. Heroux (maherou@sandia.gov)
//
// ************************************************************************
// @HEADER

// ///////////////////////////////////////////////////////////////////////

// Routine to compute the 1-norm difference between two vectors where:

// n - number of vector elements (on this processor)

// v1, v2 - input vectors

// residual - pointer to scalar value, on exit will contain result.

// ///////////////////////////////////////////////////////////////////////

#include <cmath>  // needed for fabs
using std::fabs;
#include "compute_residual.hpp"
#include "loop_defines.h"

#if defined(USING_QTHREADS) && !defined(CXX_LAMBDAS)

class cr_loop
{
public: cr_loop(const double *const V1,
                const double *const V2) :
        ret(0.0), v1(V1), v2(V2) {}
    void operator() (const size_t startat,
                     const size_t stopat)
    {
        double max = fabs(v1[startat] - v2[startat]);

        for (size_t i = startat + 1; i < stopat; i++) {
            double diff = fabs(v1[i] - v2[i]);
            if (diff > max) { max = diff; }
        }
        union {
            double   d;
            uint64_t u;
        } tmp1, tmp2;
        tmp1.d = ret;
        while (max > tmp1.d) {
            tmp2.u = tmp1.u;
            tmp1.d = max;
            tmp1.u = qthread_cas((uint64_t *)&ret, tmp2.u, tmp1.u);
            if (tmp1.u == tmp2.u) { break; }
        }
    }

    double ret;

private:
    const double *const v1;
    const double *const v2;
};

int compute_residual(const int           n,
                     const double *const v1,
                     const double *const v2,
                     double *const       residual)
{
    double local_residual = 0.0;

    extern int tcount;

    if (tcount > 0) {
        cr_loop loop(v1, v2);
        qt_loop_balance(0, n, loop);
        local_residual = loop.ret;
    } else {
        for (int i = 0; i < n; i++) {
            double diff = fabs(v1[i] - v2[i]);
            if (diff > local_residual) { local_residual = diff; }
        }
    }
# ifdef USING_MPI
    // Use MPI's reduce function to collect all partial sums

    double global_residual = 0;

    MPI_Allreduce(&local_residual, &global_residual, 1, MPI_DOUBLE, MPI_SUM,
                  MPI_COMM_WORLD);
    *residual = global_residual;
# else
    *residual = local_residual;
# endif

    return(0);
}

#else // if defined(USING_QTHREADS) && !defined(CXX_LAMBDAS)

int compute_residual(const int           n,
                     const double *const v1,
                     const double *const v2,
                     double *const       residual)
{
    double local_residual = 0.0;

    LOOP_BEGIN(0, n, start, stop);
    for (unsigned int i = start; i < stop; i++) {
        double diff = fabs(v1[i] - v2[i]);
        if (diff > local_residual) { local_residual = diff; }
    }
    LOOP_END();
# ifdef USING_MPI
    // Use MPI's reduce function to collect all partial sums

    double global_residual = 0;

    MPI_Allreduce(&local_residual, &global_residual, 1, MPI_DOUBLE, MPI_SUM,
                  MPI_COMM_WORLD);
    *residual = global_residual;
# else
    *residual = local_residual;
# endif

    return(0);
}

#endif // if defined(USING_QTHREADS) && !defined(CXX_LAMBDAS)

/* vim:set expandtab: */
