proc g(f, x: 1*real) {
  return 2.0*f(x);
}

proc h(x: 1*real) {
  return x(0)**2;
}

var x: 1*real;
x(0) = 2.0;
writeln( g(h,x) );

