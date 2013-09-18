// 9-point stencil using a modified version of BlockDist that can 
// handle fluff (ghosts/overlap/etc.) and periodic boundaries
// Note: disable the RAD optimization when compiling: -sdefaultDoRADOpt=false
//
// Compiling: chpl stencil9-mod.chpl -M ../../release/examples/benchmarks/miniMD/stencil/helpers/StencilDist.chpl <other>

use StencilDist;

config const n = 10;

config const epsilon = 0.01;

config const printArrays = false;

const LocDom = {1..n  , 1..n  },
         Dom = LocDom dmapped Block(LocDom, fluff=(1,1));

var A, B: [Dom] real;

A[  n/4+1,   n/4+1] =  1.0;
A[3*n/4+1, 3*n/4+1] =  1.0;
A[  n/4+1, 3*n/4+1] = -1.0;
A[3*n/4+1,   n/4+1] = -1.0;

A.updateFluff();

if printArrays then
  writeln("Initial A:\n", A, "\n");

var numIters = 0;

do {
  numIters += 1;

  forall (i,j) in Dom do 
    B[i,j] = 0.25   * A[i,j]
           + 0.125  * (A[i+1,j  ] + A[i-1,j  ] + A[i  ,j-1] + A[i  ,j+1])
           + 0.0625 * (A[i-1,j-1] + A[i-1,j+1] + A[i+1,j-1] + A[i+1,j+1]);

  const delta = max reduce fabs(B - A);

  if printArrays then
    writeln("delta: ", delta);

  B <=> A;
  
  A.updateFluff();

 } while (delta > epsilon);

if printArrays then
  writeln("Final A:\n", A);

writeln("# iterations: ", numIters);
