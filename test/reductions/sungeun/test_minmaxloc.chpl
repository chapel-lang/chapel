use Random;

record LastLocState {
  type eltType;
  var value: eltType;
  var uninitialized = true;

  proc init(type eltType) {
    this.eltType = eltType;
  }

  proc init(type eltType, startMin: bool) {
    this.eltType = eltType;
    this.value = if startMin then min(eltType) else max(eltType);
  }
}

class lastmaxloc: ReduceScanOp {
  type eltType;
  var state: LastLocState(eltType);

  proc identity  return new LastLocState(eltType, startMin=true);

  proc accumulate(otherSt) { accumulateOntoState(this.state, otherSt.value); }

  proc accumulateOntoState(ref state, otherVal) {
    if state.uninitialized                  ||
       ( otherVal(0) > state.value(0) )     ||
       ( (otherVal(0) == state.value(0)) &&
         (otherVal(1) > state.value(1))   )
    then
      state.value = otherVal;

    state.uninitialized = false;
  }

  proc combine(otherOp) {
    if otherOp.state.uninitialized then
      ; // no update is needed
    else
      accumulate(otherOp.state);
  }

  proc generate()  return state.value;

  proc clone()     return new unmanaged lastmaxloc(eltType = eltType);
}

class lastminloc: ReduceScanOp {
  type eltType;
  var state: LastLocState(eltType);

  proc identity  return new LastLocState(eltType, startMin=false);

  proc accumulate(otherSt) { accumulateOntoState(this.state, otherSt.value); }

  proc accumulateOntoState(ref state, otherVal) {
    if state.uninitialized                  ||
       ( otherVal(0) < state.value(0) )     ||
       ( (otherVal(0) == state.value(0)) &&
         (otherVal(1) > state.value(1))   )
    then
      state.value = otherVal;

    state.uninitialized = false;
  }

  proc combine(otherOp) {
    if otherOp.state.uninitialized then
      ; // no update is needed
    else
      accumulate(otherOp.state);
  }

  proc generate()  return state.value;

  proc clone()     return new unmanaged lastminloc(eltType = eltType);
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

