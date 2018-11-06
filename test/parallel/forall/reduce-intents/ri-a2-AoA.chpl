// Timing of in- and reduce-intents for an array-of-arrays.
// Note that it may somewhat depend on how the loop body is optimized.

use Time;

config var n_bodies = 10000;
const pDomain = {0..#n_bodies};
type vec3 = [0..#3] real;

// The arrays' names are insignificant.
var forces: [pDomain] vec3;
var velocities: [pDomain] vec3;

var timer: Timer;
var snapshot, completion: real;
proc snapTime() { snapshot = timer.elapsed(); }
timer.start();

// in-intent, vec3
timer.clear();
forall q in pDomain with (in forces) {
  if (q == 0) then
    snapTime();
}
completion = timer.elapsed();
writeln("vec3 in-intent - startup: ", snapshot);
writeln("vec3 in-intent - completion: ", completion);

// reduce intent, vec3
timer.clear();
forall q in pDomain with (+ reduce velocities) {
  if (q == 0) then
    snapTime();
}
completion = timer.elapsed();
writeln("vec3 reduce intent - startup: ", snapshot);
writeln("vec3 reduce intent - completion: ", completion);

// The arrays' names are insignificant.
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
writeln("tup3 in-intent - startup: ", snapshot);
writeln("tup3 in-intent - completion: ", completion);

// reduce intent, tup3
timer.clear();
forall q in pDomain with (+ reduce masses) {
  if (q == 0) then
    snapTime();
}
completion = timer.elapsed();
writeln("tup3 reduce intent - startup: ", snapshot);
writeln("tup3 reduce intent - completion: ", completion);
