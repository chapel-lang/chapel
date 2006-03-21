function foo() {
  var s: seq(int) = (/ 1, 2, 3 /);

  writeln("s is: ", s);

  return s;
}

var a = foo();
writeln("a is: ", a);
