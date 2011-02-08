proc g(f, x: 1*real) {
  return 2.0*f(x);
}

proc h(x: 1*real) {
  return x(1)**2;
}

var x: 1*real;
x(1) = 2.0;
writeln( g(h,x) );

