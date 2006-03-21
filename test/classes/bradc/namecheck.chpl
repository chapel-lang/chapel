class R {
  var x: int;
  var y: float;
}

fun foo() {
  var x: int = 1;
  var y: float = 2.3;

  var r = R(x = x, y = y);

  writeln("r is: ", r);
}

fun bar() {
  var x: float = 4.5;
  var y: int = 6;

  var r = R(x = y, y = x);

  writeln("r is: ", r);
}

fun main() {
  foo();
  bar();
}
