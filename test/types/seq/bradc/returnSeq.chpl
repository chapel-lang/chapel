function foo() {
  var s: seq(integer) = (/ 1, 2, 3 /);

  writeln("s is: ", s);

  return s;
}

var a = foo();
writeln("a is: ", a);
