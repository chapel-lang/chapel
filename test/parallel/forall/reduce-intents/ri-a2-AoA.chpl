// Timing of in- and reduce-intents for an array-of-arrays.
// Note that it may somewhat depend on how the loop body is optimized.


config const n_bodies = 100;
const pDomain = {0..#n_bodies};

config const printTimings = false;
use Time;
var timer: Timer;
var snapshot, completion: real;
proc snapTime() { snapshot = timer.elapsed(); }
timer.start();
proc reportTimings(message:string) {
  if printTimings {
    writeln(message, " - startup: ", snapshot);
    writeln(message, " - completion: ", completion);
  }
}

/////////// arrays of arrays ///////////

// The arrays' names are insignificant.

type vec3 = [0..#3] real;
var forces: [pDomain] vec3;
var velocities: [pDomain] vec3;

// in-intent, vec3
timer.clear();
forall q in pDomain with (in forces) {
  if (q == 0) then
    snapTime();
}
completion = timer.elapsed();
reportTimings("vec3 in-intent");

// reduce intent, vec3
timer.clear();
forall q in pDomain with (+ reduce velocities) {
  if (q == 0) then
    snapTime();
}
completion = timer.elapsed();
reportTimings("vec3 reduce intent");

/////////// arrays of tuples ///////////

type tup3 = 3*real;
var positions: [pDomain] tup3;
var masses: [pDomain] tup3;

// in-intent, tup3
timer.clear();
forall q in pDomain with (in positions) {
  if (q == 0) then
    snapTime();
}
completion = timer.elapsed();
reportTimings("tup3 in-intent");

// reduce intent, tup3
timer.clear();
forall q in pDomain with (+ reduce masses) {
  if (q == 0) then
    snapTime();
}
completion = timer.elapsed();
reportTimings("tup3 reduce intent");

/////////// done ///////////

writeln("done");
