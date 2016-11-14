use Time;

config param printTime = false;
config const tolerance = 2.0;

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
if timer.elapsed() < 1e-6*100 then
  halt("Slept short on TimeUnits.microseconds", ": ", timer.elapsed());
timer.clear();

timer.start();
sleep(100, TimeUnits.milliseconds);
timer.stop();
if printTime then
  stderr.writeln(timer.elapsed());
if timer.elapsed() < 1e-3*100 then
  halt("Slept short on TimeUnits.milliseconds: ", timer.elapsed());
timer.clear();

timer.start();
sleep(3);
timer.stop();
if printTime then
  stderr.writeln(timer.elapsed());
if timer.elapsed() < 3 then
  halt("Slept short on TimeUnits.seconds: ", timer.elapsed());
timer.clear();

writeln("Done");
