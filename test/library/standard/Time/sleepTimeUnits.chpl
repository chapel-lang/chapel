use Time, IO;

config param printTime = false;

// nanosecond
config const tolerance = 1e-9;

// Returns true if 'a' is less than 'b' by the above tolerance
proc lt(a : real, b : real) {
  return a < (b-tolerance);
}

var timer: Timer;

timer.start();
sleep(-5);
timer.stop();
if printTime then
  stderr.writeln(timer.elapsed());
timer.clear();

timer.start();
sleep(100, TimeUnits.microseconds);
timer.stop();
if printTime then
  stderr.writeln(timer.elapsed());
if lt(timer.elapsed(), 1e-6*100) then
  halt("Slept short on TimeUnits.microseconds", ": ", timer.elapsed());
timer.clear();

timer.start();
sleep(100, TimeUnits.milliseconds);
timer.stop();
if printTime then
  stderr.writeln(timer.elapsed());
if lt(timer.elapsed(), 1e-3*100) then
  halt("Slept short on TimeUnits.milliseconds: ", timer.elapsed());
timer.clear();

timer.start();
sleep(3);
timer.stop();
if printTime then
  stderr.writeln(timer.elapsed());
if lt(timer.elapsed(), 3) then
  halt("Slept short on TimeUnits.seconds: ", timer.elapsed());
timer.clear();

writeln("Done");
