config const dummy = false;
config const printMemStats = false;

proc pass_original(A: []) {
  A;
}

proc return_original(A: []) {
  return A;
}

proc create_rankchange(A: []) {
  A[.., 3];
  if dummy then writeln(A.domain);
}

proc return_rankchange(A: []) {
  return A[.., 3];
}

use MemDiagnostics;
proc main() {
  var A: [1..9, 1..9] int;

  writeln("Calling pass_original() with rank-change:");
  var m1 = memoryUsed();
  serial {
    pass_original(A[3, 1..9]);
  }
  var m2 = memoryUsed();
  writeln("\t", m2-m1, " bytes leaked");
  if printMemStats then printMemAllocs();

  writeln("Calling return_original() with rank-change:");
  m1 = memoryUsed();
  serial {
    return_original(A[3, 1..9]);
  }
  m2 = memoryUsed();
  writeln("\t", m2-m1, " bytes leaked");
  if printMemStats then printMemAllocs();

  writeln("Calling return_original() with rank-change and assignment:");
  m1 = memoryUsed();
  serial {
    var A2 = return_original(A[3, 1..9]);
  }
  m2 = memoryUsed();
  writeln("\t", m2-m1, " bytes leaked");
  if printMemStats then printMemAllocs();

  writeln("Calling create_rankchange():");
  m1 = memoryUsed();
  serial {
    create_rankchange(A);
  }
  m2 = memoryUsed();
  writeln("\t", m2-m1, " bytes leaked");
  if printMemStats then printMemAllocs();

  writeln("Calling return_rankchange():");
  m1 = memoryUsed();
  serial {
    return_rankchange(A);
  }
  m2 = memoryUsed();
  writeln("\t", m2-m1, " bytes leaked");
  if printMemStats then printMemAllocs();

  writeln("Calling return_rankchange() with assignment:");
  m1 = memoryUsed();
  serial {
    var A2 = return_rankchange(A);
  }
  m2 = memoryUsed();
  writeln("\t", m2-m1, " bytes leaked");
  if printMemStats then printMemAllocs();
}
