use Memory;

config const n = 4;
config const printProgress = false;
config const printMemStats = false;

const D = {1..n};
var A: [D] int;

proc main() {
  writeln("Calling do_local_domain():");
  var m1 = memoryUsed();
  serial do do_local_domain();
  var m2 = memoryUsed();
  writeln("\t", m2-m1, " bytes leaked");
  if printMemStats then printMemAllocs();

  writeln("Calling do_local_array():");
  m1 = memoryUsed();
  serial do do_local_array();
  m2 = memoryUsed();
  writeln("\t", m2-m1, " bytes leaked");
  if printMemStats then printMemAllocs();

  writeln("Calling do_array() with global:");
  m1 = memoryUsed();
  serial do do_array(A);
  m2 = memoryUsed();
  writeln("\t", m2-m1, " bytes leaked");
  if printMemStats then printMemAllocs();

  writeln("Calling do_array() with local:");
  m1 = memoryUsed();
  serial {
    const D = {1..n};
    var A: [D] int;
    do_array(A);
  }
  m2 = memoryUsed();
  writeln("\t", m2-m1, " bytes leaked");
  if printMemStats then printMemAllocs();

  writeln("Calling sync begin do_array() with global:");
  m1 = memoryUsed();
  serial do sync begin do_array(A);
  m2 = memoryUsed();
  writeln("\t", m2-m1, " bytes leaked");
  if printMemStats then printMemAllocs();

  writeln("Calling sync begin do_array() with local:");
  m1 = memoryUsed();
  serial do sync {
    const D = {1..n};
    var A: [D] int;
    begin do_array(A);
  }
  m2 = memoryUsed();
  writeln("\t", m2-m1, " bytes leaked");
  if printMemStats then printMemAllocs();

  writeln("Calling do_tuple() with global:");
  m1 = memoryUsed();
  serial do do_tuple((A, D));
  m2 = memoryUsed();
  writeln("\t", m2-m1, " bytes leaked");
  if printMemStats then printMemAllocs();

  writeln("Calling do_tuple() with local:");
  m1 = memoryUsed();
  serial {
    const D = {1..n};
    var A: [D] int;
    do_tuple((A, D));
  }
  m2 = memoryUsed();
  writeln("\t", m2-m1, " bytes leaked");
  if printMemStats then printMemAllocs();

  writeln("Calling if_fn with local:");
  m1 = memoryUsed();
  serial {
    const D = {1..n};
    var A : [D] int;
    if_fn(A, D);
  }
  m2 = memoryUsed();
  writeln("\t", m2-m1, " bytes leaked");
  if printMemStats then printMemAllocs();
}

proc do_local_domain() {
  var D, D_copy, D_slice, D_expand, D_exterior, D_interior, D_translate: domain(1);
  var D_strided: domain(1, stridable=true);

  if printProgress then writeln("Initializing D");
  D = {1..n};   // add ref count and decrement in destructor

  if printProgress then writeln("Initializing D_copy");
  D_copy = D;                   // create a new domain, ref count

  if printProgress then writeln("Initializing D_strided");
  D_strided = D by 2;           // create a new domain, ref count

  if printProgress then writeln("Initializing D_slice");
  D_slice = D[2..n-1];          // create a new domain, ref count

  if printProgress then writeln("Initializing D_expand");
  D_expand = D.expand(1);       // create a new domain, ref count

  if printProgress then writeln("Initializing D_exterior");
  D_exterior = D.exterior(1);   // create a new domain, ref count

  if printProgress then writeln("Initializing D_interior");
  D_interior = D.interior(1);   // create a new domain, ref count

  if printProgress then writeln("Initializing D_translate");
  D_translate = D.translate(1); // create a new domain, ref count
}

proc do_local_array() {
  if printProgress then writeln("Creating A");
  var A: [1..n] int; // add ref count and decrement in destructor

  if printProgress then writeln("Creating A_copy");
  var A_copy = A;                      // create nw array, ref count

  if printProgress then writeln("Creating A_domain");
  var A_domain: [A.domain] int; // add ref count and decrement in destructor

  if printProgress then writeln("Creating A_alias");
  ref A_alias = A;  // no need to ref count

  if printProgress then writeln("Creating A_slice");
  var A_slice = A[2..n-1];             // create new descriptor, ref count

  if printProgress then writeln("Creating A_reindex");
  var A_reindex = A.reindex(4..n+3); // create new descriptor, ref count
}

proc do_array(A:[]) {
  if printProgress then writeln("Creating A_copy");
  var A_copy = A;                      // create nw array, ref count

  if printProgress then writeln("Creating A_domain");
  var A_domain: [A.domain] int; // add ref count and decrement in destructor

  if printProgress then writeln("Creating A_alias");
  ref A_alias = A;  // no need to ref count

  if printProgress then writeln("Creating A_slice");
  var A_slice = A[2..n-1];             // create new descriptor, ref count

//  if printProgress then writeln("Creating A_reindex");
//  var A_reindex = A.reindex(4..n+3); // create new descriptor, ref count
}


proc do_tuple(t: _tuple) {
  if printProgress then writeln("Creating A");
  var A = t(0);
  if printProgress then writeln("Creating D");
  var D = t(1);
}

proc if_fn(A:[], D) {
  ref X = if isDomain(D) then A[D] else A;
  X[D.first] = 1;
}
