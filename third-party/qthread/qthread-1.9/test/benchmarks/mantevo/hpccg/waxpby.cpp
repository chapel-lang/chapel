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

// Routine to compute the update of a vector with the sum of two
// scaled vectors where:

// w = alpha*x + beta*y

// n - number of vector elements (on this processor)

// x, y - input vectors

// alpha, beta - scalars applied to x and y respectively.

// w - output vector.

// ///////////////////////////////////////////////////////////////////////

#include "waxpby.hpp"
#include "loop_defines.h"

#if defined(USING_QTHREADS) && !defined(CXX_LAMBDAS)

class waxpby_both
{
public: waxpby_both(const double *const X,
                    const double        BETA,
                    const double        ALPHA,
                    const double *const Y,
                    double *const       W) :
        x(X), y(Y), beta(BETA), alpha(ALPHA), w(W) {}
    void operator() (const size_t startat,
                     const size_t stopat)
    {
        for (size_t i = startat; i < stopat; ++i) {
            w[i] = alpha * x[i] + beta * y[i];
        }
    }

private:
    const double *const x;
    const double *const y;
    const double        beta;
    const double        alpha;
    double *const       w;
};

class waxpby_shortcut
{
public: waxpby_shortcut(const double *const X,
                        const double        BA,
                        const double *const Y,
                        double *const       W) :
        x(X), y(Y), ba(BA), w(W) {}
    void operator()(const size_t startat,
                    const size_t stopat)
    {
        for (size_t i = startat; i < stopat; ++i) {
            w[i] = ba * x[i] + y[i];
        }
    }

private:
    const double *const x;
    const double *const y;
    const double        ba;
    double *const       w;
};

int waxpby (const int           n,
            const double        alpha,
            const double *const x,
            const double        beta,
            const double *const y,
            double *const       w)
{
    extern int tcount;

    if (tcount > 0) {
        if (alpha == 1.0) {
            waxpby_shortcut loop(y, beta, x, w);
            qt_loop_balance(0, n, loop);
        } else if (beta == 1.0) {
            waxpby_shortcut loop(x, alpha, y, w);
            qt_loop_balance(0, n, loop);
        } else {
            waxpby_both loop(x, beta, alpha, y, w);
            qt_loop_balance(0, n, loop);
        }
    } else {
        if (alpha == 1.0) {
            for (int i = 0; i < n; i++) w[i] = x[i] + beta * y[i];
        } else if(beta == 1.0) {
            for (int i = 0; i < n; i++) w[i] = alpha * x[i] + y[i];
        } else {
            for (int i = 0; i < n; i++) w[i] = alpha * x[i] + beta * y[i];
        }
    }

    return(0);
}

#else // if defined(USING_QTHREADS) && !defined(CXX_LAMBDAS)

int waxpby (const int           n,
            const double        alpha,
            const double *const x,
            const double        beta,
            const double *const y,
            double *const       w)
{
    if (alpha == 1.0) {
        LOOP_BEGIN(0, n, start, stop);
        for (unsigned int i = start; i < stop; ++i) w[i] = x[i] + beta * y[i];
        LOOP_END();
    } else if(beta == 1.0) {
        LOOP_BEGIN(0, n, start, stop);
        for (unsigned int i = start; i < stop; ++i) w[i] = alpha * x[i] + y[i];
        LOOP_END();
    } else {
        LOOP_BEGIN(0, n, start, stop);
        for (unsigned int i = start; i < stop; ++i) w[i] = alpha * x[i] + beta * y[i];
        LOOP_END();
    }

    return(0);
}

#endif // if defined(USING_QTHREADS) && !defined(CXX_LAMBDAS)

/* vim:set expandtab: */
