use Random;
use Time;

config const correctness = false; // disables output
config const size = 100000;

var t: Timer;

var data: [1..size] int;
t.start();
shuffle(data, seed=10);
t.stop();
if !correctness {
  writeln('Time: ', t.elapsed());
}
