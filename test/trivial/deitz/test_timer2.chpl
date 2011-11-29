use Time;

var t: Timer;

t.start();
sleep(5);
if t.elapsed() < 4.9 then
  writeln("too short of a time");
else if t.elapsed() > 5.2 then
  writeln("too long of a time");
else
  writeln("time value is okay");
