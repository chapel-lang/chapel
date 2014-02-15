use CommDiagnostics;

config const n = 100000;
config param tupleLen = 10;

// This replaces the compiler-supplied element-by-element write with a bulk
// copy.
proc =(ref lhs:tupleLen*int, rhs:tupleLen*int) { __primitive("=", lhs, rhs); }

var A:[1..n] tupleLen*int;
resetCommDiagnostics();
startCommDiagnostics();

on Locales[1] {
  for tup in A {
    var tmp: tupleLen*int;
    for j in 1..tupleLen {
      tmp[j] = 1;
    }
    tup = tmp;
  }
}

stopCommDiagnostics();

writeln(A[1]);
writeln(A[n]);

writeln(getCommDiagnostics());
