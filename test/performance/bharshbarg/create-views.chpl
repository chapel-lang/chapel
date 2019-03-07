
use Time;

config const n = 100;

config const printPerf = false;

// Number of seconds to sample
config const limit = 2.0;

enum mode {
  Slice,
  RankChange,
  Reindex
}

proc makeSlice(X: []) {
  if X.rank == 1 {
    ref s = X[1..n/2];
    // use 's' so it's not somehow optimized away
    if s.size > X.size then halt();
  } else {
    ref s = X[1..n/4, 1..n/4];
    if s.size > X.size then halt();
  }
}

proc makeRankChange(X: []) {
  if X.rank == 2 {
    ref rc = X[.., 1];
    if rc.size > X.size then halt();
  }
}

proc makeReindex(X: []) {
  if X.rank == 1 {
    ref r = X.reindex(0..#X.size);
    if printPerf == false then assert(X.size == r.size);
    if r.size > X.size then halt();
  } else {
    ref r = X.reindex(0..#n/2, 0..#n/2);
    if printPerf == false then assert(X.size == r.size);
    if r.size > X.size then halt();
  }
}

proc test(m : mode, X : []) {
  var t : Timer;
  var c : int;

  t.start();
  while true {
    if c % 10000 == 0 && t.elapsed() > limit then break;
    if      m == mode.Slice      then makeSlice(X);
    else if m == mode.RankChange then makeRankChange(X);
    else if m == mode.Reindex    then makeReindex(X);
    c += 1;
  }
  t.stop();

  // Mega-views/s
  if printPerf {
    const dim = "-" + X.rank:string + "D";
    writeln(m, dim, ": ", c / t.elapsed() / 1e6);
  }
}

proc main() {
  var A : [1..n] int;
  var B : [1..n/2, 1..n/2] int;

  test(mode.Slice, A);
  test(mode.Slice, B);
  test(mode.RankChange, B);
  test(mode.Reindex, A);
  test(mode.Reindex, B);
}
