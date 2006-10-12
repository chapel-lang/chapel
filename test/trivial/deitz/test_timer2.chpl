use Time;

var t: Timer;

t.start();
sleep(5);
if t.value < 5.0 then
  writeln("too short of a time");
else if t.value > 5.1 then
  writeln("too long of a time");
else
  writeln("time value is okay");
