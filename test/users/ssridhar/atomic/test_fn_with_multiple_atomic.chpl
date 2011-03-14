var x: uint;
var y: uint;

proc main() {
  atomic x = x + 1;
  atomic foo(x);
  writeln("x = ", x);
  writeln("y = ", y);
}

proc foo(x: uint) {
  y = x;
  y = y + 1;
}