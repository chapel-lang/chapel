class Parent {
  proc foo() { return (3, 3); }
}

class Child: Parent {
  override proc foo() { return (1,1); }
}

var ownChild = new owned Child();
var child: borrowed Parent = ownChild.borrow();
child.foo();
