config const debug = false;
config const printTiming = false;

config const n = 1024;
const D = {1..n};
var AD: domain(int);
config const offset = 7;
var Aref: [D] int;
var A: [D] int;
var success = true;

for i in D {
  Aref(i) = 2*i-offset;
  AD += Aref(i);
}
QuickSort(Aref);
if debug then writeln(AD);
if debug then writeln(Aref);

use Time;
var t0: Timer;

t0.start();
for ai in AD {
  A((ai+offset)/2) = ai;
}
t0.stop();
if debug then writeln(A);
success = true;
QuickSort(A);
for i in D {
  if A(i) != Aref(i) {
    success = false;
    break;
  }
}
writeln("Serial domain iteration: ", if success then "SUCCESS" else "FAILED");
if printTiming then writeln("Serial: ", t0.elapsed(TimeUnits.milliseconds));


var t1: Timer;
t1.start();
forall ai in AD {
  A((ai+offset)/2) = ai;
}
t1.stop();
if debug then writeln(A);
success = true;
QuickSort(A);
for i in D {
  if A(i) != Aref(i) {
    success = false;
    break;
  }
}
writeln("Parallel domain iteration: ", if success then "SUCCESS" else "FAILED");
if printTiming then writeln("Parallel: ", t1.elapsed(TimeUnits.milliseconds));

