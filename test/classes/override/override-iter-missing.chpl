class Parent {
  iter myiter() {
    yield 1;
  }
  iter these() {
    yield 1;
  }
}

class Child : Parent {
  iter myiter() {
    yield 2;
  }
  iter these() {
    yield 2;
  }
}

proc main() {
  var child = new Child();
  var p:Parent = child.borrow();
  for x in p.myiter() do
    writeln("x ", x);
  for y in p do
    writeln("y ", y);
}
