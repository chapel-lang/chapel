#include "fma-sincosm1pi.h"
#include <math.h>

void c_sincosm1pi(const double a, double* res0, double* res1) {
    const double t = a * a;
    const double pi = 3.1415926535897931e+0;
    /*
     * Approximate cos(pi*x)-1 for x in [-0.25,0.25]
     * Approximate sin(pi*x) for x in [-0.25,0.25]
     */
    double c = (-1.0369917389758117e-4);
    double s = (+4.6151442520157035e-4);

    c = fma (c, t,  1.9294935641298806e-3);
    s = fma (s, t, -7.3700183130883555e-3);
    c = fma (c, t, -2.5806887942825395e-2);
    s = fma (s, t,  8.2145868949323936e-2);
    c = fma (c, t,  2.3533063028328211e-1);
    s = fma (s, t, -5.9926452893214921e-1);
    c = fma (c, t, -1.3352627688538006e+0);
    s = fma (s, t,  2.5501640398732688e+0);
    c = fma (c, t,  4.0587121264167623e+0);
    s = fma (s, t, -5.1677127800499516e+0);
    c = fma (c, t, -4.9348022005446790e+0);
    s = fma (a, pi, s * (t * a));

    *res0 = c * t;
    *res1 = s;
}

