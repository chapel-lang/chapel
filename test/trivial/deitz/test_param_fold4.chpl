proc foo() {
  writeln("hello world!");
}

param x = 12;
param y = 14;

if x + y > x - y then
  foo();
else
  bar();
