var x: real*2;

writeln("x is: ", x);

operator *(type t, param x: int) type {
  return x*t;
}
