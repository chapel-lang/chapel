use LinearAlgebra;
use Time;

config const correctness = false; // disables output
config const size = 3;
config const pwr = 2;

var t: Timer;

var X = Matrix(size, size);
t.start();
var MM4 = matPow(X, pwr);
t.stop();
if !correctness {
  writeln('Time: ', t.elapsed());
}
