use Time;

config const n = 2000000;
config const timing = true;

var very_long: string;
for i in 1..n {
  very_long += "this is a very long string ";
}

var i: int;
var t: Timer;
t.start();
for s in very_long.split() {
  i += 1;
}
t.stop();

if timing then
  writeln("Time: ", t.elapsed());
if i == 6 * n then
  writeln("SUCCESS");
