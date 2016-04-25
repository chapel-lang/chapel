use Time;

config const n = 1;
config const timing = true;

var very_long: string;
for i in 1..n {
  very_long += "fish ";
}

var i: int;
var t: Timer;
t.start();
for s in very_long.split() {
  i += 1;
}
t.stop();

if i == n then
  writeln("SUCCESS");
if timing then
  writeln("Time: ", t.elapsed());
