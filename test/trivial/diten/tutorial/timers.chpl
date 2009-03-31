/*
 * Timer Example
 *
 * This example demonstrates the use of a Timer from the Time standard module.
 *
 */

//
// Use the Time standard module to bring its symbols into scope.
//
use Time;

//
// A Timer record can be used like a stopwatch to time portions of code.
//
var t: Timer;

//
// To time a function, start the timer before calling the function. Here,
// we will time the sleep function, also defined in the Time module.
//
t.start();
sleep(1);
t.stop();

//
// On some systems, a request to sleep for a given period of time could result
// in a delay slightly less than the requested amount of time, so when checking
// how much time has elapsed, we need to multiply the desired amount of time
// by a factor that is very nearly one.
//
const almostOne = 0.99996;

//
// By now, at least one second should have elapsed. By default, the elapsed
// method reports time in seconds.
//
if t.elapsed() < 1 * almostOne then
  halt("Didn't sleep for a full second");
else
  writeln("Slept for at least 1 second");
//
// The elapsed time can also be checked in units other than seconds. The
// supported units are: microseconds, milliseconds, seconds, minutes, hours.
//
if t.elapsed(TimeUnits.milliseconds) < 1000 * almostOne then
  halt("Didn't sleep for a full second");
else
  writeln("Slept for at least 1 thousand milliseconds");

//
// The Timer can be started again to accumulate additional time.
//
t.start();
sleep(1);
t.stop();
if t.elapsed(TimeUnits.microseconds) < 2000000 * almostOne then
  halt("Didn't accumulate enough time");
else
  writeln("Accumulated at least 2 million microseconds");

//
// To start the Timer over at zero, call the clear method.
//
t.clear();
if t.elapsed() != 0 then
  halt("Timer should be zero");
else
  writeln("The timer was reset to zero");

//
// The timer can be checked while still running. This can be used to time
// multiple events. Here, the time taken by each iteration of the loop
// is saved into the iterationTimes array.
// 
config const n = 5;
var iterationTimes: [1..n] real;
t.start();
for i in 1..n {
  var startTime = t.elapsed(TimeUnits.microseconds);
  //
  // This code will be timed n times.
  //
  iterationTimes(i) = t.elapsed(TimeUnits.microseconds);
}
t.stop();
