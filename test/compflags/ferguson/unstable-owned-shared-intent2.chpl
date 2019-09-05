class Parent { var x: int; }

class Child : Parent { var y: int; }

proc foo(arg: owned Parent) { }

proc test1() {
  var x:owned Parent = new owned Child();
  var y = new owned Child();
  foo(x);
  foo(y);
}
test1();
