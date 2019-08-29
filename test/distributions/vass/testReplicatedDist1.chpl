use ReplicatedDist, BlockDist, CyclicDist;

// what to iterate over

config const n = 5;
config const lo = 2, b = 3;

const Dbase = {1..n, 1..n};
const DsubLoc = {lo..#b, lo..#b};

// array element type

config type elt = int;

// ARepl - the replicated array to be tested

const repllocales = Locales;  // in case this changes
var ReplBlockDist = new dmap(new Replicated());
var DRepl: domain(2) dmapped ReplBlockDist = DsubLoc;
var ARepl: [DRepl] elt;

proc show() {
  for loc in Locales {
    on loc {
      writeln(loc, ":");
      writeln(ARepl);
    }
  }
}
    

// set everything to predetermined values
proc reset() {
  // explicitly go to each locale
  coforall loc in repllocales do on loc do
    // explicitly index into each element
    for ix in DsubLoc do ARepl[ix] = 100 + here.id;
}

/*
// differentiate elements based on their locales
proc addLocale() {
  // explicitly go to each locale
  coforall loc in repllocales do on loc do
    // explicitly index into each element
    for ix in DsubLoc do ARepl[ix] += here.id;
}
*/

proc start(msg...) { write("\n---  ", (...msg), "\n"); }

// run all the tests when the reference array A is under a given domain map

proc trydist(mydmap, teston: locale, dmname) {
  write("\n----------- testing on ", teston,
        " with ", dmname, " -----------\n");


on teston {

  var D: domain(2) dmapped mydmap = Dbase;
  const Dsub = D[DsubLoc];
  var A: [D] elt;
  proc resetA() { A = [(i,j) in D] i*10 + j; }
  proc showA() { write(/*"A=\n",*/ A, "\n"); }

  start("resetting A");
  resetA();
  showA();

  // initialize ARepl
  start("resetting ARepl");
  reset();
  show();

  // Convention: for each test, reset ARepl before; if needed, resetA after.

  // assignment
  start("ARepl = A[Dsub];");
  reset();
  ARepl = A[Dsub];
  show();

  // slicing using a local / DR domain
  start("ARepl = A[DsubLoc];");
  reset();
  ARepl = A[DsubLoc];
  show();

  // equivalent to the assignment
  start("forall (a,b) in zip(ARepl, A[Dsub]) do a = b;");
  reset();
  forall (a,b) in zip(ARepl, A[Dsub]) do a = b;
  show();

  // sequential loop
  start("for (a,b) in zip(ARepl, A[Dsub]) do a = b;");
  reset();
  for (a,b) in zip(ARepl, A[Dsub]) do a = b;
  show();

  // --- iterate over DsubLoc => only the current locale's replicant is visited --

  // in a forall
  start("forall ix in DsubLoc do ARepl[ix] = A[ix];");
  reset();
  forall ix in DsubLoc do ARepl[ix] = A[ix];
  show();

  // --- iterate over Dsub => each locale's replicant is visited

  // in a forall
  start("forall ix in Dsub do ARepl[ix] = A[ix];");
  reset();
  forall ix in Dsub do ARepl[ix] = A[ix];
  show();

  // in a for
  start("for ix in Dsub do ARepl[ix] = A[ix];");
  reset();
  for ix in Dsub do ARepl[ix] = A[ix];
  show();

  // --- iterate over D => each index visits only one replicant ---

  // assignment - reverse
  start("A[Dsub] = ARepl;");
  reset();
  resetA();
  A[Dsub] = ARepl;
  showA();
  resetA();

  start("A[DsubLoc] = ARepl;");
  reset();
  resetA();
  A[DsubLoc] = ARepl;
  showA();
  resetA();

  // parallel loop
  start("forall (b,a) in zip(A[Dsub],ARepl) do a = b;");
  reset();
  forall (b,a) in zip(A[Dsub],ARepl) do a = b;
  show();

  // sequential loop
  start("for (b,a) in zip(A[Dsub],ARepl) do a = b;");
  reset();
  for (b,a) in zip(A[Dsub],ARepl) do a = b;
  show();

  start("for (b,a) in zip(A[DsubLoc],ARepl) do a = b;");
  reset();
  for (b,a) in zip(A[DsubLoc],ARepl) do a = b;
  show();

  // --- explicitly iterating over DRepl, in a for (forall would lead to races)

  // A is assigned (multiply) from the current locale's replicand
  start("for ix in DRepl do A[ix] = ARepl[ix];");
  reset();
  for ix in DRepl do A[ix] = ARepl[ix];
  showA();
  resetA();

  // --- verifying locality ---

  // The following tries to assert that no communication is required to
  // operate on a replicated array relative to local data; need to make
  // 'A' Block or Cyclic-distributed to make it interesting; it will
  // require privatization to work correctly (if A is truly distributed).

/* this currently does not work because privatization is not implemented:

  start("for (i,j) in Dsub do on (A(i,j)) do local { A(i,j) = ARepl(i,j); }");
  reset();
  for (i,j) in Dsub do
    on (A(i,j)) do
      local { A(i,j) = ARepl(i,j); }
  showA();
  resetA();

*/

} // on teston
} // trydist()


// driver for trydist()

write("available locales: ", Locales, "\n");
start("ARepl before reset"); show();

iter testLocs(): locale {
  yield Locales[0];
  if numLocales > 2 then yield Locales[2];
}

for tloc in testLocs() {
  // gotta create new dmap() on the same locale as 'domainmap'
  trydist(defaultDist, tloc, "default");
  trydist(new dmap(new Block(boundingBox=Dbase)), tloc, "block");
  trydist(new dmap(new Cyclic(startIdx=Dbase.low)), tloc, "cyclic");
}

write("\nDone\n");
