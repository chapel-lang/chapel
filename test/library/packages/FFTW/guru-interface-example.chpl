/* This example was taken from Github issue 12181,
   and demonstrated how to use the FFTW guru interface
   to compute the FFT of an array in reverse.

*/

use FFTW;
use FFTW.C_FFTW;
use Random;


config const n1=100;
config const n2=24;
config const verbose=false;

config const threshold=1.0e-13;

const D = {0.. #n1, 0.. #n2}; // Just an example
// a1 : The original array
// a2 : The reversed array
// b1 : FFT of reverse of a1, calculated by guru interface.
// b2 : FFT of a2
var a1,b1,a2,b2: [D]complex;


fillRandom(a1); // Original array
a2 = reshape(a1, D by -1);

if verbose {
  writeln("The original array :");
  writeln(a1);
  writeln("The reversed array :");
  writeln(a2);
}

// Perform the FFT
//var plan1 = plan_dft(a1, b1, FFTW_FORWARD, FFTW_ESTIMATE);
var dims : [0..1]fftw_iodim;
dims[0].n =  n1:c_int;
dims[0].is= -n2:c_int;
dims[0].os=  n2:c_int;
dims[1].n =  n2:c_int;
dims[1].is=  -1:c_int;
dims[1].os=   1:c_int;
// Note that I need to send in the pointer to the array here.
var plan1 = fftw_plan_guru_dft(2, c_ptrTo(dims), 0, nil, c_ptrTo(a1[D.high]), c_ptrTo(b1),
                               FFTW_FORWARD, FFTW_ESTIMATE);

var plan2 = plan_dft(a2, b2, FFTW_FORWARD, FFTW_ESTIMATE);
execute(plan1);
execute(plan2);

if verbose {
  writeln("The FFT of the original array reversed :");
  writeln(b1);
  writeln("The FFT reversed array :");
  writeln(b2);
}

// Verify
var diff = 0.0;
forall (expected, actual) in zip(b2, b1) with (max reduce diff) {
  diff = abs(expected-actual);
}
if verbose then writeln("Maximum difference :", diff);
if (diff < threshold) then writeln("SUCCESS"); else writeln("FAILURE");