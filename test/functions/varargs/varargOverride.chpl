class Parent {
  proc foo(elts: int ... ?nElts) {
    writeln("Parent");
    for e in elts do writeln(e);
  }
}

class Child: Parent {
  override proc foo(elts: int ... ?nElts) {
    writeln("Child");
    for e in elts do writeln(e);
  }
}

proc main {
  var ownP = new owned Child();
  var p: borrowed Parent = ownP.borrow();
  p.foo(1);
}
