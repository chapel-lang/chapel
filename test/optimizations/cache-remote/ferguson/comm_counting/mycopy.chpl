use CommDiagnostics;
use Time;

config const n = 10000;
config const verbose = false;

var A:[1..n] int;
var B:[1..n] int;

for i in 1..n {
  A[i] = i;
  B[i] = 0;
}

var t = new Timer();

t.start();

on Locales[1] {
  for i in 1..n {
    B[i] = A[i];
  }
}

t.stop();

for i in 1..n {
  A[i] = i;
  B[i] = 0;
}

resetCommDiagnostics();
startCommDiagnostics();

on Locales[1] {
  for i in 1..n {
    B[i] = A[i];
  }
}

stopCommDiagnostics();

writeln(B[1]);
writeln(B[n]);

if verbose {
  writeln(t.elapsed(), " seconds");
}

var d = getCommDiagnostics();
var ngets = (d(1).get + d(1).get_nb):int;
var nputs = (d(1).put + d(1).put_nb):int;
if verbose {
  writeln(ngets, " gets");
  writeln(nputs, " puts");
}

assert(ngets < n);
assert(nputs < n);

