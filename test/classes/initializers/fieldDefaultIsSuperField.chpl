proc main() {
  var ownF = new owned Foo1();
  var f = ownF.borrow();
  writeln("f = ", f);
}

class Foo0 {
  var x = 10;

  proc init() {

  }
}

class Foo1 : Foo0 {
  var y =  x;              // This is a phase1 access of a parent field

  proc init() {

  }
}

