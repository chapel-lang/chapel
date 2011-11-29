var x: 3*real;

x(1) = 1.0;
x(2) = 1.2;
x(3) = 1.4;

on Locales((here.id + 1)%numLocales) {
  writeln("x is: ", x);
}