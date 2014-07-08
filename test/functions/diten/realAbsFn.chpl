var x: real = 2.0;

// create a number big enough that squaring it will be infinity
while !isinf(x*x) do
  x *= x;

writeln("      x: ", x);
writeln(" abs(x): ", abs(x));
writeln("fabs(x): ", fabs(x));
writeln(" abs(x:complex): ", abs(x:complex));
