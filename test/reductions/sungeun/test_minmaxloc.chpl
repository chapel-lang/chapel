use Random;

class lastmaxloc: ReduceScanOp {
  type eltType;
  var value: eltType = min(eltType);
  var uninitialized = true;

  proc accumulate(x) {
    if uninitialized || (x(1) > value(1)) ||
      ((x(1) == value(1)) && (x(2) > value(2))) then
      value = x;
    uninitialized = false;
  }
  proc combine(x) {
    if uninitialized || (x.value(1) > value(1)) ||
      ((x.value(1) == value(1)) && (x.value(2) > value(2))) {
      if !x.uninitialized {
        value = x.value;
        uninitialized = false;
      }
    }
  }
  proc generate() return value;
}

class lastminloc: ReduceScanOp {
  type eltType;
  var value: eltType = max(eltType);
  var uninitialized = true;

  proc accumulate(x) {
    if uninitialized || (x(1) < value(1)) ||
      ((x(1) == value(1)) && (x(2) > value(2))) then
      value = x;
    uninitialized = false;
  }
  proc combine(x) {
    if uninitialized || (x.value(1) < value(1)) ||
      ((x.value(1) == value(1)) && (x.value(2) > value(2))) {
      if !x.uninitialized {
        value = x.value;
        uninitialized = false;
      }
    }
  }
  proc generate() return value;
}

config const seed = 889;
config const sigfigs = 1;
config const n = 1000000;
config const debug = false;

var R: [1..n] real;
var A: [1..n] int;

fillRandom(R, seed, algorithm=RNG.NPB);

proc getSigDigit(r: real) {
  var rn = r*10;
  while (rn < 10**(sigfigs-1)) do rn *= 10;
  return rn:int;
}

A = getSigDigit(R);

if debug {
  writeln(R);
  writeln(A);
}

var ml = minloc reduce zip(A, {1..n});
writeln(ml);
ml = lastminloc reduce zip(A, {1..n});
writeln(ml);

ml = maxloc reduce zip(A, {1..n});
writeln(ml);
ml = lastmaxloc reduce zip(A, {1..n});
writeln(ml);

