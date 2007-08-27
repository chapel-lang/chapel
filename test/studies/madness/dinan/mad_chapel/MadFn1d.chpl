/** Test functions for Function1d class
 */

use Fn1d;
use Math;

const PI = 3.14159265358979323846;

/** gaussian with square normalized to 1 */
class Fn_Test1: Fn1d {
    def this(x: real): real {
        var a = 500.0;
        return (2*a/PI)**0.25 * exp(-a * (x-0.5)**2.0);
    }
};


/** derivative of test1 */
class Fn_dTest1: Fn1d {
    def this(x: real): real {
        var a = 500.0;
        return -2.0*a*(x-0.5) * (2*a/PI) ** 0.25 * exp(-a * (x-0.5)**2.0);
    }
};


/** superposition of multiple gaussians */
class Fn_Test2: Fn1d {
    var g = Fn_Test1();

    def this(x: real): real {
        return g.this(x-0.3) + g.f(x) + g.f(x+0.3);
    }
};


/** derivative of test2 */
class Fn_dTest2: Fn1d {
    var g = Fn_DTest1();

    def this(x: real): real {
        return g.this(x-0.3) + g.f(x) + g.f(x+0.3);
    }
};


/** a more interesting (singular and oscillating) function
  ... note it is never computed exactly at the singularity except by
  the test code below.
 */
class Fn_Test3: Fn1d {
    def this(x: real): real {
        var a = 100.0*PI;
        if (x == 0.5) then
            return 0.0;
        else
            return cos(a*(x-0.5))/sqrt(abs(x-0.5));
    }
};


/** derivative of test3 */
class Fn_dTest3: Fn1d {
    def this(x: real): real {
        var a = 100.0*PI;
        if (x == 0.5) {
            return 0.0;
        } else {
            var s = if (x < 0.5) then -1.0 else 1.0;
            return -a*sin(a*(x-0.5))/sqrt(abs(x-0.5))
                - s*0.5*cos(a*(x-0.5))/abs(x-0.5)**1.5;
        }
    }
}
