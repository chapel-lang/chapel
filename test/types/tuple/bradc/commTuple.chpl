var x: 3*real;

x(0) = 1.0;
x(1) = 1.2;
x(2) = 1.4;

on Locales((here.id + 1)%numLocales) {
  writeln("x is: ", x);
}