class Parent {
  iter myiter(arg:int) {
    yield arg;
  }
  iter these(arg:int) {
    yield 1;
  }
}

class Child : Parent {
  override iter myiter() {
    yield 2;
  }
  override iter these() {
    yield 2;
  }
}

proc main() {
  var p = new borrowed Child();
  for x in p.myiter() do
    writeln("x ", x);
  for y in p do
    writeln("y ", y);
}
