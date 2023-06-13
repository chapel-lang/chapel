use MemDiagnostics, Types, Time, Sort;

config const printTiming = false;
config const verify = true;

config const offset = 7;

config const memFraction = if verify then 100000 else 1000;
type eltType = int;

const totalMem = here.physicalMemory(unit = MemUnits.Bytes);
const target = (totalMem / numBytes(eltType)) / memFraction;

// set a max problem size
const n = min(target, 8 * 1e9) : int;

const D = {1..n};
var AD: domain(eltType);

var Aref: [D] eltType;
var A: [D] eltType;


for i in D {
  const off = 2*i-offset;
  Aref(i) = off;
  AD += off;
}
if verify then sort(Aref);

//
// SERIAL
//

{
  var timer: stopwatch;
  timer.start();
  for ai in AD {
    A((ai+offset)/2) = ai;
  }
  timer.stop();

  var success = true;

  if verify {
    sort(A);
    for i in D {
      if A(i) != Aref(i) {
        success = false;
        break;
      }
    }
  }

  writeln("Serial domain iteration: ", if success then "SUCCESS" else "FAILED");
  if printTiming then writeln("Serial: ", timer.elapsed());
}

//
// PARALLEL
//

{
  var timer: stopwatch;
  timer.start();
  forall ai in AD {
    A((ai+offset)/2) = ai;
  }
  timer.stop();

  var success = true;

  if verify {
    sort(A);
    for i in D {
      if A(i) != Aref(i) {
        success = false;
        break;
      }
    }
  }
  writeln("Parallel domain iteration: ", if success then "SUCCESS" else "FAILED");
  if printTiming then writeln("Parallel: ", timer.elapsed());
}
