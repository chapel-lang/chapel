use Random;

config const seed = 1;
config const verbose = false;

config const n = 100000;


proc testit(seed: int, n: int) {
  var A: [1..n] uint;
  var B: [1..n] uint;
  var afterA:uint;
  var afterB:uint;

  var rng = new randomStream(seed=seed, eltType=uint);
  for i in 1..n {
    A[i] = rng.next();
  }
  afterA = rng.next();

  if verbose then writeln("testit ", seed, " ", n, " A");
  rng.skipTo(0);
  for i in 1..n {
    B[i] = rng.next();
  }
  checkMatch(A, B);
  afterB = rng.next();
  assert(afterB == afterA);

  if verbose then writeln("testit ", seed, " ", n, " B");
  rng.skipTo(0);
  rng.fill(B);
  checkMatch(A, B);
  afterB = rng.next();
  assert(afterB == afterA);

  if verbose then writeln("testit ", seed, " ", n, " C");
  B = 0;
  rng.skipTo(0);
  for (i,x) in zip(A.domain, rng.next(A.domain)) {
    B[i] = x;
  }
  checkMatch(A, B);
  afterB = rng.next();
  assert(afterB == afterA);

  if verbose then writeln("testit ", seed, " ", n, " D");
  B = 0;
  rng.skipTo(0);
  forall (i,x) in zip(A.domain, rng.next(A.domain)) with (ref B) {
    B[i] = x;
  }
  checkMatch(A, B);
  afterB = rng.next();
  assert(afterB == afterA);

  if verbose then writeln("testit ", seed, " ", n, " E");
  B = 0;
  rng.skipTo(0);
  for (x,i) in zip(rng.next(A.domain), A.domain) {
    B[i] = x;
  }
  checkMatch(A, B);
  afterB = rng.next();
  assert(afterB == afterA);

  if verbose then writeln("testit ", seed, " ", n, " F");
  B = 0;
  rng.skipTo(0);
  forall (x,i) in zip(rng.next(A.domain), A.domain) with (ref B) {
    B[i] = x;
  }
  checkMatch(A, B);
  afterB = rng.next();
  assert(afterB == afterA);

  if verbose then writeln("testit ", seed, " ", n, " G");
  rng.skipTo(0);
  for i in 1..n {
    B[i] = rng.next(0, max(uint));
  }
  checkMatch(A, B);
  afterB = rng.next();
  assert(afterB == afterA);

  if verbose then writeln("testit ", seed, " ", n, " H");
  rng.skipTo(0);
  rng.fill(B, 0, max(uint));
  checkMatch(A, B);
  afterB = rng.next();
  assert(afterB == afterA);

  if verbose then writeln("testit ", seed, " ", n, " I");
  B = 0;
  rng.skipTo(0);
  for (i,x) in zip(A.domain, rng.next(A.domain, 0, max(uint))) {
    B[i] = x;
  }
  checkMatch(A, B);
  afterB = rng.next();
  assert(afterB == afterA);

  if verbose then writeln("testit ", seed, " ", n, " J");
  B = 0;
  rng.skipTo(0);
  forall (i,x) in zip(A.domain, rng.next(A.domain, 0, max(uint))) with (ref B) {
    B[i] = x;
  }
  checkMatch(A, B);
  afterB = rng.next();
  assert(afterB == afterA);

  if verbose then writeln("testit ", seed, " ", n, " K");
  B = 0;
  rng.skipTo(0);
  for (x,i) in zip(rng.next(A.domain, 0, max(uint)), A.domain) {
    B[i] = x;
  }
  checkMatch(A, B);
  afterB = rng.next();
  assert(afterB == afterA);

  if verbose then writeln("testit ", seed, " ", n, " L");
  B = 0;
  rng.skipTo(0);
  forall (x,i) in zip(rng.next(A.domain, 0, max(uint)), A.domain) with (ref B) {
    B[i] = x;
  }
  checkMatch(A, B);
  afterB = rng.next();
  assert(afterB == afterA);
}

proc testitb(seed: int, n: int, min: uint, max: uint) {
  var A: [1..n] uint;
  var B: [1..n] uint;

  var rng = new randomStream(seed=seed, eltType=uint);
  for i in 1..n {
    A[i] = rng.next(min, max);
    assert(min <= A[i] && A[i] <= max);
  }

  if verbose then writeln("testitb ", seed, " ", n, " ", min, " ", max, " A");
  rng.skipTo(0);
  for i in 1..n {
    B[i] = rng.next(min, max);
  }
  checkMatch(A, B);

  if verbose then writeln("testitb ", seed, " ", n, " ", min, " ", max, " B");
  rng.skipTo(0);
  rng.fill(B, min, max);
  checkMatch(A, B);

  if verbose then writeln("testitb ", seed, " ", n, " ", min, " ", max, " C");
  B = 0;
  rng.skipTo(0);
  for (i,x) in zip(A.domain, rng.next(A.domain, min, max)) {
    B[i] = x;
  }
  checkMatch(A, B);

  if verbose then writeln("testitb ", seed, " ", n, " ", min, " ", max, " D");
  B = 0;
  rng.skipTo(0);
  forall (i,x) in zip(A.domain, rng.next(A.domain, min, max)) with (ref B) {
    B[i] = x;
  }
  checkMatch(A, B);
}

var failures = 0;
proc checkMatch(A, B) {
  for i in A.domain {
    if A[i] != B[i] {
      if i-1 >= 1 then
        writef("note: at %n %n vs %n\n", i-1, A[i-1], B[i-1]);
      writef("errr: at %n %n vs %n\n", i, A[i], B[i]);
      if i+1 <= n then
        writef("note: at %n %n vs %n\n", i+1, A[i+1], B[i+1]);
      failures += 1;
    }
  }
}

testit(seed, n);
testitb(seed, n, 0, 1);
testitb(seed, n, 0, 2);
testitb(seed, n, 0, 3);
testitb(seed, n, 0, 10);
testitb(seed, n, 0, 100);
testitb(seed, n, 0, 1000);
testitb(seed, n, 0, 10000);
testitb(seed, n, 0, 100000);
testitb(seed, n, 0, 1000000);
testitb(seed, n, 0, 10000000);
testitb(seed, n, 0, 100000000);
testitb(seed, n, 0, 1000000000);
testitb(seed, n, 0, 10000000000);
testitb(seed, n, 0, 100000000000);
testitb(seed, n, 0, 1000000000000);
testitb(seed, n, 0, 10000000000000);
testitb(seed, n, 0, 100000000000000);
testitb(seed, n, 0, 1000000000000000);
testitb(seed, n, 0, 10000000000000000);
testitb(seed, n, 0, 100000000000000000);
testitb(seed, n, 0, 1000000000000000000);
testitb(seed, n, 0, 10000000000000000000);

if failures == 0 {
  writeln("OK");
} else {
  writeln("FAIL: ", failures);
}
