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

// Set up the GSL function
var p = new Payload(1.0);
var F : gsl_function;
F.function = c_ptrTo(func1);
F.params = c_ptrTo(p);

// Set up the integrator
var wk = gsl_integration_workspace_alloc(1000);
var result, error: real(64);
const expected = -4.0;

// Actual call
gsl_integration_qags(c_ptrTo(F), 0.0, 1.0, 0.0, 1.0e-7, 1000, wk, c_ptrTo(result), c_ptrTo(error));

// Clean up
writeln("Integration result : ",result);
writeln("Expected : ", expected);
writeln("Estimated error : ",error);
writeln("Actual error : ",abs(result-expected));
gsl_integration_workspace_free(wk);
