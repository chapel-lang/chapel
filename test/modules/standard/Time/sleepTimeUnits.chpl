use Time;

config const tolerance = 2.0;

var timer: Timer;

timer.start();
sleep(100, TimeUnits.microseconds);
timer.stop();
if abs((1e-6 * 100) - timer.elapsed()) > (tolerance*(1e-6*100)) then
  halt("Exceeded tolerance on TimeUnits.microseconds");

timer.start();
sleep(100, TimeUnits.milliseconds);
timer.stop();
if abs((1e-3 * 100) - timer.elapsed()) > (tolerance*(1e-3*100)) then
  halt("Exceeded tolerance on TimeUnits.milliseconds");
timer.clear();

timer.start();
sleep(3);
timer.stop();
if abs(3 - timer.elapsed()) > (tolerance*3) then
  halt("Exceeded tolerance on TimeUnits.seconds");
timer.clear();

writeln("Done");
