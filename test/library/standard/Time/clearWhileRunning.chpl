use Time;

config const printTiming = true;

var timer: Timer;
timer.start();
sleep(1);
const time = timer.elapsed();
if printTiming then writeln(time);

timer.clear();
sleep(1);
const time2 = timer.elapsed();
if printTiming then writeln(time2);

timer.clear();
if timer.elapsed() > 1.0 then
  halt("timer.clear() failed");

timer.stop();

writeln("Done");
