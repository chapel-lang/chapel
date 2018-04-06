class Parent {
  proc foo(elts: int ... ?nElts) {
    writeln("Parent");
    for e in elts do writeln(e);
  }
}

class Child: Parent {
  proc foo(elts: int ... ?nElts) {
    writeln("Child");
    for e in elts do writeln(e);
  }
}

proc main {
  var p: Parent = new Child();
  p.foo(1);
}
