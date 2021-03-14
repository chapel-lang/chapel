/** Matrix-Vector Multiplication in Chapel
    James Dinan, June 2007

    MVM: C = A * B
 **/

use Time;

var t: Timer;

config param quiet   = true;
config const M: uint = 1024;
config const N: uint = 4192;

var A: [1..M, 1..N] real = 1.0;
var B: [1..N] real = 1.0;
var C: [1..M] real = 0.0;

if !quiet {
  writeln("MVM_par: Parallel Matrix Vector Multiplication in Chapel (C = A * B)");
  writeln("Matrix dimensions: ", A.domain, " Vector dimensions: ", B.domain);
  writeln("Memory usage: A = ", M*N*numBytes(real)/1024,
          " KiB, B = ", N*numBytes(real)/1024,
          " KiB, C = ", M*numBytes(real)/1024, " KiB");
  writeln();
  writeln("Multiplying..");
  t.start();
}

coforall i in A.domain.dim(0) {
  for j in A.domain.dim(1) {
    C[i] += A[i,j] * B[j];
  }
}

if !quiet {
  t.stop();
  writeln("Validating result..");
}

var error = false;

for i in C.domain {
  // Verify the result:
  //  Since A and B are all '1's then every entry in C should be equal to N
  if (C[i]:uint != N) {
    writeln("oops: C[",i,"] = ", C[i], ".  Should be ", N, ".");
    error = true;
  }
}

if !quiet then writeln("parallel mvm time: ", t.elapsed(), "s FLOPS: ", M*N*2/t.elapsed());

if (!error) {
  writeln("test: passed");
} else
  writeln("test: failed");
