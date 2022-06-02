use Random;

config const seed = 1;
config const verbose = false;

config const n = 100000;

proc testit(seed: int, n: int) {
  var A: [1..n] uint;
  var B: [1..n] uint;

  A=0;
  B=0;
  fillRandom(A, min, max, seed);
  fillRandom(B, min, max, seed);
  checkMatch(A,B);
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
