use MemDiagnostics, Types, Time, Sort;

config const printTiming = false;
config const verify = true;

config const offset = 7;

config const memFraction = if verify then 100000 else 5000;
type eltType = int;

const totalMem = here.physicalMemory(unit = MemUnits.Bytes);
const target = (totalMem / numBytes(eltType)) / memFraction;

// set a max problem size
const n = min(target, 8 * 1e9) : int;

const D = {1..n};
var AD: domain(eltType);

var Aref: [D] eltType;
var A: [D] eltType;
var AA: [AD] eltType;

for i in D {
  const off = 2*i - offset;
  Aref(i) = off;
  AD += off;
}
if verify then sort(Aref);

forall i in AD {
  AA(i) = i;
}

if verify {
  for i in AD {
    if AA(i) != i then halt("uh oh! ", i);
  }
}

//
// SERIAL
//

{
  var timer: stopwatch;
  timer.start();
  for aa in AA {
    var ai = aa;
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

  writeln("Serial array iteration: ", if success then "SUCCESS" else "FAILED");
  if printTiming then writeln("Serial: ", timer.elapsed());
}

//
// PARALLEL
//

{
  var parastopwatch: stopwatch;
  parastopwatch.start();
  forall aa in AA {
    var ai = aa;
    A((ai+offset)/2) = ai;
  }
  parastopwatch.stop();

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
  writeln("Parallel array iteration: ", if success then "SUCCESS" else "FAILED");
  if printTiming then writeln("Parallel: ", parastopwatch.elapsed());
}
