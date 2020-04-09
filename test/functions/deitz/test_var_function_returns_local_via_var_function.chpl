proc foo() ref {
  var x: (int, int);
  return x(1);
}

foo() = 2;
writeln(foo());
