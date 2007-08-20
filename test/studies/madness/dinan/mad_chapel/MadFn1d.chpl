/** Test function for Function1d class
 */

use Fn1d;
use Math;

const PI = 3.14159265358979323846;

/** gaussian with square normalized to 1 */
class Test1: Fn1d {
  def f(x: real): real {
    var a = 500.0;
    return (2*a/PI)**0.25 * exp(-a * (x-0.5)**2.0);
  }
};

var test1 = Test1();

/** derivative of test1 */
class DTest1: Fn1d {
  def f(x: real): real {
    var a = 500.0;
    return -2.0*a*(x-0.5) * (2*a/PI) ** 0.25 * exp(-a * (x-0.5)**2.0);
  }
};

var dtest1 = DTest1();

/** superposition of multiple gaussians */
class Test2: Fn1d {
  def f(x: real): real {
    return test1.f(x-0.3) + test1.f(x)  + test1.f(x+0.3);
  }
};

var test2 = Test2();

/** derivative of test2 */
class DTest2: Fn1d {
  def f(x: real): real {
    return dtest1.f(x-0.3) + dtest1.f(x)  + dtest1.f(x+0.3);
  }
};

var dtest2 = DTest2();

/** a more interesting (singular and oscillating) function
  ... note it is never computed exactly at the singularity except by
  the test code below.
 */
class Test3: Fn1d {
  def f(x: real): real {
    var a = 100.0*PI;
    if (x == 0.5) then
      return 0.0;
    else
      return cos(a*(x-0.5))/sqrt(abs(x-0.5));
  }
};

var test3 = Test3();
    
/** derivative of test3 */
class DTest3: Fn1d {
  def f(x: real): real {
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

var dtest3 = DTest3();
