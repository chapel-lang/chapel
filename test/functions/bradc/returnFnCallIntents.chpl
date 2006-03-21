fun foo(inout x: int): int {
  x += 100;
  writeln("In foo, x is: ", x);
  return 2*x;
}

fun bar(inout x: int): int {
  x += 1000;
  writeln("In bar, x is: ", x);
  return foo(x);
}

fun main() {
  var a: int = 10;
  var r: int;
  r = bar(a);
  writeln("In main, a is: ", a, ", r is: ", r);
}
