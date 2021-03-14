// Timers

/*
   This primer demonstrates the use of a Timer from the ``Time`` module.
*/

//
// Use the ``Time`` standard module to bring its symbols into scope.
//
use Time;

//
// The ``quiet`` configuration constant is set to ``false`` when testing
// (via start_test) so that this test is deterministic.
//
config const quiet: bool = false;

//
// A ``Timer`` can be used like a stopwatch to time portions of code.
//
var t: Timer;

//
// To time a procedure, start the timer before calling the
// procedure and stop it afterwards.  Here, we will time the ``sleep``
// procedure, also defined in the ``Time`` module.
//
t.start();
sleep(1);
t.stop();

//
// To report the time, use the ``elapsed()`` method.  By default, the
// ``elapsed`` method reports time in seconds.
//
if !quiet then
  writeln("A. ", t.elapsed(), " seconds");

//
// The elapsed time can also be checked in units other than
// seconds. The supported units are: ``microseconds``, ``milliseconds``,
// ``seconds``, ``minutes``, ``hours``.
//
if !quiet then
  writeln("B. ", t.elapsed(TimeUnits.milliseconds), " milliseconds");

//
// The ``Timer`` can be started again to accumulate additional time.
//
t.start();
sleep(1);
t.stop();
if !quiet then
  writeln("C. ", t.elapsed(TimeUnits.microseconds), " microseconds");

//
// To start the ``Timer`` over at zero, call the ``clear`` method.
//
t.clear();
writeln("D. ", t.elapsed(), " seconds");

//
// The timer can be checked while still running. This can be used to time
// multiple events. Here, the time taken by each iteration of the loop
// is saved into the ``iterationTimes`` array.
//
config const n = 5;
var iterationTimes: [1..n] real;
t.start();
for i in 1..n {
  var startTime = t.elapsed(TimeUnits.microseconds);
  //
  // This code will be timed n times.
  //
  iterationTimes(i) = t.elapsed(TimeUnits.microseconds) - startTime;
}
t.stop();

//
// Finally, a lighter-weight, but less flexible, way of doing timings
// is to use the procedure: ``getCurrentTime(unit: TimeUnits): real``
//
// It returns the number of units (seconds by default) that have
// passed since midnight as a floating-point value.  (Note that this
// makes it not particularly useful for timing things that might run
// across midnight).  As a simple example, we can use the following
// idiom to time the number of seconds something will take:
//

const start = getCurrentTime();
sleep(1);
const elapsed = getCurrentTime() - start;

if !quiet then
  writeln("E. ", elapsed, " seconds");

//
// For more information on Chapel's timing capabilities, refer to the
// :mod:`Time` section of the Standard Modules online documentation.
//
