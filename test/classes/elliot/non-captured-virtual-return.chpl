class Parent {
  proc foo() { return (3, 3); }
}

class Child: Parent {
  override proc foo() { return (1,1); }
}

var child: borrowed Parent = new borrowed Child();
child.foo();
