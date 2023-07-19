config const dummy = false;
config const printMemStats = false;

proc create_literal() {
  var A = [1, 3, 5, 7];
}
proc return_literal() {
  return [1, 3, 5, 7];
}

proc create_alias(A: []) {
  const ref A2 = A;
}
proc return_alias(A: []) {
  const ref A2 = A;
  return A2;
}

proc create_slice(A: []) {
  A[2..3];
}

proc pass_original(A: [] ) {
  A;
}

proc return_original(A: []) {
  return A;
}

proc return_slice(A: []) {
  return A[2..3];
}

proc create_reindex(A: []) {
  A.reindex(3..6);
  if dummy then writeln(A.domain);
}
proc return_reindex(A: []) {
  return A.reindex(3..6);
}

use MemDiagnostics;
proc main() {
  writeln("Calling create_literal():");
  var m1 = memoryUsed();
  serial {
    create_literal();
  }
  var m2 = memoryUsed();
  writeln("\t", m2-m1, " bytes leaked");
  if printMemStats then printMemAllocs();

  writeln("Calling return_literal():");
  m1 = memoryUsed();
  serial {
    return_literal();
  }
  m2 = memoryUsed();
  writeln("\t", m2-m1, " bytes leaked");
  if printMemStats then printMemAllocs();

  writeln("Calling return_literal() with assignment:");
  m1 = memoryUsed();
  serial {
    var A2 = return_literal();
  }
  m2 = memoryUsed();
  writeln("\t", m2-m1, " bytes leaked");
  if printMemStats then printMemAllocs();

  const A = return_literal();

  writeln("Calling create_alias():");
  m1 = memoryUsed();
  serial {
    create_alias(A);
  }
  m2 = memoryUsed();
  writeln("\t", m2-m1, " bytes leaked");
  if printMemStats then printMemAllocs();

  writeln("Calling return_alias():");
  m1 = memoryUsed();
  serial {
    return_alias(A);
  }
  m2 = memoryUsed();
  writeln("\t", m2-m1, " bytes leaked");
  if printMemStats then printMemAllocs();

  writeln("Calling return_alias() with assignment:");
  m1 = memoryUsed();
  serial {
    var A2 = return_alias(A);
  }
  m2 = memoryUsed();
  writeln("\t", m2-m1, " bytes leaked");
  if printMemStats then printMemAllocs();

  writeln("Calling create_slice():");
  m1 = memoryUsed();
  serial {
    create_slice(A);
  }
  m2 = memoryUsed();
  writeln("\t", m2-m1, " bytes leaked");
  if printMemStats then printMemAllocs();

  writeln("Calling return_slice():");
  m1 = memoryUsed();
  serial {
    return_slice(A);
  }
  m2 = memoryUsed();
  writeln("\t", m2-m1, " bytes leaked");
  if printMemStats then printMemAllocs();

  writeln("Calling return_slice() with assignment:");
  m1 = memoryUsed();
  serial {
    var A2 = return_slice(A);
  }
  m2 = memoryUsed();
  writeln("\t", m2-m1, " bytes leaked");
  if printMemStats then printMemAllocs();

  writeln("Calling pass_original() for slice:");
  m1 = memoryUsed();
  serial {
    pass_original(A[2..3]);
  }
  m2 = memoryUsed();
  writeln("\t", m2-m1, " bytes leaked");
  if printMemStats then printMemAllocs();

  writeln("Calling return_original() for slice:");
  m1 = memoryUsed();
  serial {
    return_original(A[2..3]);
  }
  m2 = memoryUsed();
  writeln("\t", m2-m1, " bytes leaked");
  if printMemStats then printMemAllocs();

  writeln("Calling return_original for slice with assignment:");
  m1 = memoryUsed();
  serial {
    var A2 = return_original(A[2..3]);
  }
  m2 = memoryUsed();
  writeln("\t", m2-m1, " bytes leaked");
  if printMemStats then printMemAllocs();

  writeln("Calling pass_original() with reindex:");
  m1 = memoryUsed();
  serial {
    pass_original(A.reindex(3..6));
  }
  m2 = memoryUsed();
  writeln("\t", m2-m1, " bytes leaked");
  if printMemStats then printMemAllocs();

  writeln("Calling return_original() with reindex:");
  m1 = memoryUsed();
  serial {
    return_original(A.reindex(3..6));
  }
  m2 = memoryUsed();
  writeln("\t", m2-m1, " bytes leaked");
  if printMemStats then printMemAllocs();

  writeln("Calling return_original() with reindex and assignment:");
  m1 = memoryUsed();
  serial {
    var A2 = return_original(A.reindex(3..6));
  }
  m2 = memoryUsed();
  writeln("\t", m2-m1, " bytes leaked");
  if printMemStats then printMemAllocs();

  writeln("Calling create_reindex():");
  m1 = memoryUsed();
  serial {
    create_reindex(A);
  }
  m2 = memoryUsed();
  writeln("\t", m2-m1, " bytes leaked");
  if printMemStats then printMemAllocs();

  writeln("Calling return_reindex():");
  m1 = memoryUsed();
  serial {
    return_reindex(A);
  }
  m2 = memoryUsed();
  writeln("\t", m2-m1, " bytes leaked");
  if printMemStats then printMemAllocs();

  writeln("Calling return_reindex() with assignment:");
  m1 = memoryUsed();
  serial {
    var A2 = return_reindex(A);
  }
  m2 = memoryUsed();
  writeln("\t", m2-m1, " bytes leaked");
  if printMemStats then printMemAllocs();
}
