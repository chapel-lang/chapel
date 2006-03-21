var x: int = 2;

fun foo(x: int) {
  var x = 4;
  writeln("then x is: ", x);
}

fun bar() {
  var x = 5;
  writeln("then x is: ", x);
}

fun baz() {
  writeln("x in baz() is: ", x);
}

fun main() {
  foo(3);
  writeln();
  bar();
  writeln();
  baz();
  writeln();
}
