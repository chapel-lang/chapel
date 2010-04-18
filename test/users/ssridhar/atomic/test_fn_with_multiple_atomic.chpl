var x: uint;
var y: uint;

def main() {
  atomic x = x + 1;
  atomic foo(x);
  writeln("x = ", x);
  writeln("y = ", y);
}

def foo(x: uint) {
  y = x;
  y = y + 1;
}