use SysCTypes;
require '-lgsl','-lgslcblas';

/* Put in the usual GSL includes here */
extern {  
// Special functions
#include "gsl/gsl_sf.h"
// Constants
#include "gsl/gsl_const.h"
// Integration
#include "gsl/gsl_integration.h"
// Random numbers and distributions
#include "gsl/gsl_rng.h"
#include "gsl/gsl_randist.h"
#include "gsl/gsl_cdf.h"
// Interpolation
#include "gsl/gsl_interp.h"
#include "gsl/gsl_spline.h"
}

// Special functions 
// We try out both interfaces. Note that you need to explicitly
// use the c_ptrTo function in these cases.
var res : gsl_sf_result;
writeln(gsl_sf_erf(0.1));
// Note how the res structure is available to the user
var ret = gsl_sf_erf_e(0.1, c_ptrTo(res));
writeln(res);
writeln(createStringWithNewBuffer(gsl_strerror(ret)));

// Use GSL precision modes. These are #defines and the extern block code
// gets the types wrong, so we need to explicitly case.
ret = gsl_sf_airy_Ai_e(10.0, GSL_PREC_DOUBLE : c_uint, c_ptrTo(res));
writeln(createStringWithNewBuffer(gsl_strerror(ret)), res);
ret = gsl_sf_airy_Ai_e(10.0, GSL_PREC_APPROX : c_uint,c_ptrTo(res));
writeln(createStringWithNewBuffer(gsl_strerror(ret)), res);

// Random number generators
// Note that this follows the C-API --- so you need to explicitly free things.
var rng1 = gsl_rng_alloc(gsl_rng_mt19937);
var rng2 = gsl_rng_alloc(gsl_rng_taus2);
writeln("Random numbers from a few random number generators");
for ii in 1..10 {
  writeln(gsl_rng_uniform(rng1), ' ',gsl_rng_get(rng2));
}

// Try out some random number distributions
var arr : [0.. #1000]real(64);
// GSL random number routines may share state. Use a serial loop
for x in arr do x = gsl_ran_gaussian(rng1, 1.0);
writeln("<x> of a sample of Gaussian random numbers :",(+ reduce arr)/1000.0);
writeln("<x^2> of a sample of Gaussian random numbers :",(+ reduce arr**2)/1000.0);
writeln("<x^3> of a sample of Gaussian random numbers :",(+ reduce arr**3)/1000.0);
writeln("<x^4> of a sample of Gaussian random numbers :",(+ reduce arr**4)/1000.0);

// Try the CDF
writeln("68.3% of a Gaussian distribution occurs < ",gsl_cdf_gaussian_Pinv(0.683,1.0));
writeln("Expected : 0.476104 (Mathematica)");

// Try some Poisson numbers
writeln("Some Poisson numbers with mean 2...");
for ii in 1..50 do writef("%s ",gsl_ran_poisson(rng1,2));
writeln();


gsl_rng_free(rng1);
gsl_rng_free(rng2);

// 1D interpolation

// Set up a simple function
var x : [0.. #10] real(64);
[i in x.domain] x[i]=i*0.1;
var y : [0.. #10] real(64) = x**3 + 2*x;
writeln('x:',x);
writeln('y:',y);

// Use the 1D higher level interface for this
var sp = gsl_spline_alloc(gsl_interp_cspline, 10);
var acc = gsl_interp_accel_alloc();
gsl_spline_init(sp, c_ptrTo(x[0]), c_ptrTo(y[0]), 10);
writeln('x^3 + 2x for x=0.45 :',gsl_spline_eval(sp, 0.45, acc));
writeln('Compared to :',0.45**3+0.9);
gsl_interp_accel_free(acc);
gsl_spline_free(sp);

// Integration
// The following is the example in the GSL manual translated.
// We do the following integral :
//    \int_0^1 x^{-1/2} log(x) dx = -4
// 
// Note that, for this particular problem, it might have been
// simpler to just wrap everything in C. Also note that we need to 
// write some boilerplate to actually pass things to C.
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

// Constants 
writef("The speed of light in m/s is %r\n",GSL_CONST_MKS_SPEED_OF_LIGHT);
