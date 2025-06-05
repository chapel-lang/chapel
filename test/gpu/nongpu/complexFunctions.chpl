// ensure that complex functions and math can be done using
// the GPU locale model from a CPU
use Math;
use IO, PrecisionSerializer;

// print with a lower precision to prevent machine issues
config const precision = 3;
var ps = stdout.withSerializer(new precisionSerializer(precision));

proc test(type t) {
  writeln("Testing ", t:string);
  var x: t = 1 + 1i;
  var y: t = -2 - 3i;
  ps.writeln("  x: ", x);
  ps.writeln("  x.im: ", x.im:imag, " x.re: ", x.re);
  ps.writeln("  y: ", y);
  ps.writeln("  y.im: ", y.im:imag, " y.re: ", y.re);
  ps.writeln("  x * y + x / y: ", x * y + x / y);

  ps.writeln("  abs(x): ", abs(x));
  ps.writeln("  phase(x): ", phase(x)); // carg
  ps.writeln("  conj(x): ", conj(x)); 
  ps.writeln("  riemProj(x): ", riemProj(x)); //cproj
  ps.writeln("  sqrt(x): ", sqrt(x));
  ps.writeln("  exp(x): ", exp(x));
  ps.writeln("  log(x): ", log(x));
  ps.writeln("  ln(x): ", ln(x));
  ps.writeln("  cos(x): ", cos(x));
  ps.writeln("  cosh(x): ", cosh(x));
  ps.writeln("  sin(x): ", sin(x));
  ps.writeln("  sinh(x): ", sinh(x));
  ps.writeln("  tan(x): ", tan(x));
  ps.writeln("  tanh(x): ", tanh(x));
  ps.writeln("  acos(x): ", acos(x));
  ps.writeln("  acosh(x): ", acosh(x));
  ps.writeln("  asin(x): ", asin(x));
  ps.writeln("  asinh(x): ", asinh(x));
  ps.writeln("  atan(x): ", atan(x));
  ps.writeln("  atanh(x): ", atanh(x));

  writeln();
}

test(complex(64));
test(complex(128));

