use Random;
use Time;

config const printTime = false;

config const n = 100000;
config const m = 40960;
config const seed = 11;

var coeffs:[0..n] real;
var xs:[1..m] real;
var ys:[1..m] real;

proc initialize() {
  var R = new randomStream(real, seed=seed);
  R.fill(coeffs);
  R.fill(xs);
  R.fill(ys);
}

proc clenshaw() {
  var t: stopwatch;
  t.start();
  foreach i in 1..m {
    var x:real = xs[i];
    var u0:real = 0;
    var u1:real = 0;
    var u2:real = 0;
    for k in 1..n by -1 {
      u2 = u1;
      u1 = u0;
      u0 = 2*x*u1-u2+coeffs[k];
    }
    ys[i] = 0.5*(coeffs[0]+u0-u2);
  }
  t.stop();
  if printTime then writeln("Elapsed: ", t.elapsed());
}

initialize();
clenshaw();

var total:real = 0;
for i in 1..m {
  total += ys[i];
}
writeln(total);
