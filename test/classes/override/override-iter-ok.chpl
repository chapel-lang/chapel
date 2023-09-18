class Parent {
  iter myiter() {
    yield 1;
  }
  iter these() {
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
  var ownP = new owned Child();
  var p:Parent = ownP.borrow();
  for x in p.myiter() do
    writeln("x ", x);
  for y in p do
    writeln("y ", y);
}
