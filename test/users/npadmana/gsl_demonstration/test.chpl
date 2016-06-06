use SysCTypes;
require '-lgsl','-lgslcblas';

extern {
// Integration
#include "gsl/gsl_integration.h"
}

record Payload {
  var alpha : real;
}
export proc func1(x : real, p : c_void_ptr) : real {
  var r = (p : c_ptr(Payload)).deref();
  return log(r.alpha*x)/sqrt(x);
}

var p = new Payload(1.0);
var F : gsl_function;
F.function = c_ptrTo(func1) : c_void_ptr;
F.params = c_ptrTo(p);
