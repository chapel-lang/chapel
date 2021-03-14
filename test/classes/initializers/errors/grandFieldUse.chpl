
class GrandParent {
  var x : int;
}

class Parent : GrandParent {
  var y : int;
}

class Child : Parent {
  var z : real;

  proc init() {
    writeln(this.x);
    super.init(1, 2);
  }
}

var c = new borrowed Child();
