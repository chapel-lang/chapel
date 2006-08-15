var x: int = 2;

def foo(x: int) {
  var x = 4;
  writeln("then x is: ", x);
}

def bar() {
  writeln("x in bar() is: ", x);
  var x = 5;
  writeln("then x is: ", x);
}

def baz() {
  writeln("x in baz() is: ", x);
}

def main() {
  foo(3);
  writeln();
  bar();
  writeln();
  baz();
  writeln();
}
