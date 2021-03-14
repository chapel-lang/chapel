config const N=1000;
config const M=10000;
config const choice=true;

// Explicit main included to reduce test maintenance
proc main() { }

proc mini_ig() {
  var A: [0..#M] int = 0..#M;
  var rindex: [0..#N] int;
  var tmp: [0..#N] int;

  forall i in 0..#N {
    tmp.localAccess[i] = A[rindex.localAccess[i]];
  }
}
mini_ig();

proc mini_histo() {
  var A: [0..#M] atomic int;
  var rindex: [0..#N] int;

  forall r in rindex {
    A[r].add(1);
  }
}
mini_histo();

proc mini_histo_block() {
  var A: [0..#M] atomic int;
  var rindex: [0..#N] int;

  forall r in rindex {
    {
      A[r].add(1);
    }
  }
}
mini_histo_block();

proc mini_histo_cond() {
  var A: [0..#M] atomic int;
  var rindex: [0..#N] int;

  forall r in rindex {
    if choice then
      A[r].add(1);
  }
}
mini_histo_cond();

proc mini_histo_cond2() {
  var A: [0..#M] atomic int;
  var rindex: [0..#N] int;

  forall r in rindex {
    if choice then
      A[r].add(1);
    else
      A[r].add(1);
  }
}
mini_histo_cond2();

proc mini_histo_cond3() {
  var A: [0..#M] atomic int;
  var rindex: [0..#N] int;

  forall r in rindex {
    if choice {
    } else {
      A[r].add(1);
    }
  }
}
mini_histo_cond3();

proc read_then_histo() {
  var A: [0..#M] atomic int;
  var B: [0..#M] atomic int;
  var rindex: [0..#N] int;

  forall r in rindex {
    var x = A[r].read();
    B[r].add(1);
  }
}
read_then_histo();

proc double_histo() {
  var A: [0..#M] atomic int;
  var B: [0..#M] atomic int;
  var rindex: [0..#N] int;

  forall r in rindex {
    A[r].add(1); // don't optimize this one
    B[r].add(1); // this one can be optimized
  }
}
double_histo();

proc nested_foralls_ok() {
  var A: [0..#M] atomic int;
  var B: [0..#M] atomic int;
  var rindex: [0..#N] int;

  forall r in rindex {
    forall r in rindex {
      B[r].add(1);
    }
    A[r].add(1);
  }
}
nested_foralls_ok();

proc mini_ra() {
  var T: [0..1023] atomic int;
  var indexMask = 1023;
  var Updates: [0..#M] int;
  var RAStream: [0..#M] int;

  forall (_, r) in zip(Updates, RAStream) do
    T(r & indexMask).xor(r);
}
mini_ra();

proc recursive(arg: int): int {
  if arg <= 0 then
    return 0;
  return 1+recursive(arg-1);
}

proc mini_ra_recursive() {
  var T: [0..1023] atomic int;
  var indexMask = 1023;
  var Updates: [0..#M] int;
  var RAStream: [0..#M] int;

  forall (_, r) in zip(Updates, RAStream) {
    recursive(indexMask);
    T(r & indexMask).xor(r);
  }
}
mini_ra_recursive();

config const multiplier = 1234567;
config const increment = 98765;

// An iterator representing a not-order-independent loop
// similar to RAStream
iter rng_iter() {
  var cursor = multiplier + increment;
  for i in 1..M {
    cursor = cursor * multiplier + increment;
    yield cursor;
  }
}
iter rng_iter(param tag: iterKind, followThis) where tag == iterKind.follower {
  const (followInds,) = followThis;
  const start = followInds.low;
  var cursor = multiplier * start + increment;
  for i in 1..M {
    cursor = cursor * multiplier + increment;
    yield cursor;
  }
}

proc mini_ra_lf() {
  var T: [0..1023] atomic int;
  var indexMask = 1023;
  var Updates: [0..#M] int;

  forall (_, r) in zip(Updates, rng_iter()) do
    T(r & indexMask).xor(r);
}
mini_ra_lf();
