var x: real*2;

writeln("x is: ", x);

proc *(type t, param x: int) type {
  return x*t;
}
